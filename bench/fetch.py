import subprocess
import os.path

bench_urls = [
    "https://snap.stanford.edu/data/web-Stanford.txt.gz",
    "https://snap.stanford.edu/data/roadNet-CA.txt.gz",
    "https://snap.stanford.edu/data/soc-LiveJournal1.txt.gz",
    "https://snap.stanford.edu/data/bigdata/communities/com-orkut.ungraph.txt.gz",
]

benches = list(map(lambda s: s.split("/")[-1][:-7], bench_urls))

print(benches)

for (url, b) in zip(bench_urls, benches):
    if os.path.exists(b + ".txt"):
        continue
    subprocess.run(["wget", "-nc", url]).check_returncode()
    subprocess.run(["gzip", "-dfk", b + ".txt.gz"]).check_returncode()

for b in benches:
    if os.path.exists(b + ".in"):
        continue

    v = 0
    e = 0
    s = ""
    with open(b + ".txt", "r") as f:
        for l in f.read().splitlines():
            if not l.startswith("#"):
                v = max(*map(int, l.split()), v)
                e += 1
                s += l + "\t" + "1" + "\n"

    v += 1

    with open(b + ".in", "w") as f:
        f.write(f"{v}\t{v}\t{e}\n")
        f.write(s)
