# ParallelBFS
Parallel Breadth First Search with OpenMP

## Build & Run
```sh
# Install dependencies
sudo apt install meson ninja-build
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
OpenMP   | libomp 5

## Optimizations
### Atomic bitmap
### Prefix sum frontier allocation
### Hybrid (top down / bottom up) traversal
### OpenMP scheduling strategy tuning

## Discussion

## Reference
1. [Beamer, Scott, Krste Asanovic, and David Patterson. "Direction-optimizing breadth-first search."](https://downloads.hindawi.com/journals/sp/2013/702694.pdf)
2. [Hong, Sungpack, Tayo Oguntebi, and Kunle Olukotun. "Efficient parallel graph exploration on multi-core CPU and GPU."](https://ppl.stanford.edu/papers/pact11-hong.pdf)
3. [Leiserson, Charles E., and Tao B. Schardl. "A work-efficient parallel breadth-first search algorithm (or how to cope with the nondeterminism of reducers)."](http://supertech.csail.mit.edu/papers/pbfs.pdf)
4. [MIT 6.172 Performance Engineering of Software Systems, Lecture 22](https://www.youtube.com/watch?v=IT_4fw6gfJw&list=PLUl4u3cNGP63VIBQVWguXxZZi0566y7Wf&index=22)
