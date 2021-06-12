# ParallelBFS
Parallel Breadth First Search with OpenMP

## Build & Run
```sh
# Install dependencies
sudo apt install meson ninja-build python3
meson setup build --buildtype=release
meson compile -C build

cd bench

# Fetch benchmark
python3 fetch.py

# Run benchmark
python3 bench.py
```

## Benchmark
Config   | Value
-------- | -----
CPU      | Intel Xeon E5-2690 v4 @ 2.60GHz (14 Cores / 28 Threads)
Memory   | 252 GiB
Distro   | Ubuntu 18.04
Kernel   | 3.10.0
Compiler | Clang 11.0
Opt      | -O3
OpenMP   | libomp 5.0

## Benchmark
All performance figures are reported in MTEPS, or million traversals of edges per seconds.

Benchmark       | Nodes       | Edges         | Top-down 1t | Top-down / nt    | Hybrid / nt      | StdDev  | Speedup
--------------- | ----------- | ------------- | ----------- | -------------    | ---------------- | ------- | ------
web-Stanford    | 281,903     | 2,312,497     | 141.10      | 469.47 / 13      | **490.02 / 4**   | 2.11    | 3.47
roadNet-CA      | 1,965,206   | 2,766,607     | 41.75       | **161.27 / 13**  | 153.48  / 12     | 1.67    | 3.92
com-Orkut       | 3,072,441   | 117,185,083   | 242.88      | **2365.44 / 25** | 2361.95 / 14     | 101.67  | 9.73
soc-LiveJournal | 4,847,571   | 68,993,773    | 60.17       | 692.73 / 27      | **1056.64 / 14** | 51.60   | 17.56
RMAT 1          | 100,000,000 | 1,000,000,000 | 308.22      | **3894.68 / 19** | 2931.37 / 25     | 1263.54 |
RMAT 2          | 100,000,000 | 1,000,000,000 | x           | 1109.15          | **2004.01 / 24** | 1018.85 |
RMAT 3          | 100,000,000 | 1,000,000,000 | x           | 1709.12          | **3528.19 / 25** |         |


## Optimizations

### Atomic bitmap
```C
u32 next = adj->c[j]
// Frequent cache miss due to random access by `parent[next]`
if (parent[next] = -1) {
  queue_push(q, next);
  parent[next] = v;
  distance[next] = distance[v] + 1;
}

// Reduces memory footprint by a factor of 32
// (vertex id is represented as u32)
if (!bitmap_test_set(visited, next) {
    // ...
}
```
Note that the asymptotic complexity for breadth first search is O(E + V), which linear in the number of inputs. Therefore, considering that the memory access patterns for graph traversals are usually not sequential, performance will ultimiately be bounded by memory throughput and cache misses. Using the `parent` array to mark whether a vertex is visited or not is not a very good idea because a vertex takes 32 bit or bytes. A single bit would be enough to store such information, reducing cache misses and memory footprint (random accesses) by a factor of 32.
```C
bool bitmap_test_set(bitmap *b, u32 n) {
  return !!((atomic_fetch_or(&b->map[index], 1 << offset) >> offset) & 1);
}
```
In the concurrent setting, accesses to bitmap must be performed atomically to prevent generating duplicate vertices in the next frontier. This is done using `atomic_fetch_or` from `<stdatomic.h>`. In theory, using `atomic_compare_and_swap` on the parent array should introduce fewer contentions compared to the atomic bitmap where 32 vertices share a single `atomic_uint`. In practice, contention rarely happens and performance is limited by memory accesses instead.

### Prefix sum frontier allocation

In the serial version of bfs, we visit all nodes in the current frontier, iterating over all of its edges to check if there any vertices not yet visited. If there is one, the new vertex is pushed the next frontier (queue). Simply adding `#pragma omp parallel for` to the out most for loop will not work due to data race in incrementing the queue size. A straight forward solution is to use a concurrent queue, or increment the queue size atomically. Experiments show that this approach is far from ideal, especially when the graph is small. Instances like `web-Stanford` and `roadNet-CA` report a *slowdown* of 10x ~ 100x.

Another way is allocate a sub frontier for each thread, and merge the sub frontiers after each iteration is done. The only question  remains is: How much to allocate? Allocating a fixed size sub frontier may not work because new vertices may not necessarily be load balanced. Using a dynamically sized vector hurts performance especially when the number of new vertices is small.

To tackle this problem, we exploit a special property of the csr matrix. First, out going degrees of all vertices in the frontier is calculated and stored in a `degree` array. This can be calculated in O(1) * |F| time using `mat->r[v + 1] - mat->r[v]`. Then, the we perform prefix sum on the `degree` array as starting index for new vertices discovered. The last value of the `degree` array is equal to the total number of edges from the frontier, or the maximum number of new vertices might be discovered. The next frontier is then allocated with a capacity of that value.

When inserting new vertices into the next frontier, the index is calculated by `degree[i] + j` where `degree[i]` is the starting index and `j` is the offset from the column vector bounded by `mat->r[v + 1] - mat->r[v]`. When discovering a vertex that has been already visited before, a sentinel value (-1) is inserted and later discarded in the merge process.

Merging once again makes use of the prefix sum operation. A `1` is set in the index array if a new vertex is found, a `0` if a sentinel is found otherwise. Computing prefix sum on the index array gives new indices with sentinels removed. All of the operations introduced including allocation, insertion and merging can be done in parallel with no contention. This is the key to achieving parallelism in top-down bfs.

### Hybrid (top down / bottom up) traversal
An interesting observation was made by Beamer et al. in their paper "Direction-optimizing breadth-first search" that the conventional top-down bfs visits a lot of traversed peers and parents on graphs with large degree and small diameter. Instead of traversing the frontier and checking whether a vertex is visited or not, which yields a lot of failed attempts, they proposed to iterate over all the vertices and check if their parents are in the visited set. Another benefit of the bottom approach is that no atomics are needed. Do note however, that this optimization is only applicable to undirected graph. Applying such technique to directed graphs would require calculating a inverse adjacency matrix, incurring high runtime and memory overhead.

A heuristic can be used to switch between bottom-up and top-down bfs to achieve the best of both world. We follow the authors' suggestion on parameter tuning. As per request of the professor, all graphs are treated as undirected even though some actually aren't (web-Stanford). Note some of the results might be inaccurate due to this requirement though.

### OpenMP scheduling strategy tuning
After experimenting with various scheduling strategy (static, dynamic, guide) and chunk size (1, 2, 4, 8, 16, auto). We've found out that the `guided` strategy with no chunk size specified performs consistly well in almost all benchmarks. The `static` strategy is heavily load imbalanced and scales poorly. The `dynamic` strategy, while having lower overhead and best load balancing when compared to `guided` has poor cache locality, underperforming ~20% in most cases. We empirically the set the scheduing strategy to guided for both top-down and bottom-up bfs.

## Reference
1. [Beamer, Scott, Krste Asanovic, and David Patterson. "Direction-optimizing breadth-first search."](https://downloads.hindawi.com/journals/sp/2013/702694.pdf)
2. [Hong, Sungpack, Tayo Oguntebi, and Kunle Olukotun. "Efficient parallel graph exploration on multi-core CPU and GPU."](https://ppl.stanford.edu/papers/pact11-hong.pdf)
3. [Leiserson, Charles E., and Tao B. Schardl. "A work-efficient parallel breadth-first search algorithm (or how to cope with the nondeterminism of reducers)."](http://supertech.csail.mit.edu/papers/pbfs.pdf)
4. [MIT 6.172 Performance Engineering of Software Systems, Lecture 22](https://www.youtube.com/watch?v=IT_4fw6gfJw&list=PLUl4u3cNGP63VIBQVWguXxZZi0566y7Wf&index=22)
