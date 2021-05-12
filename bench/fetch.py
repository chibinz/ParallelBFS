import subprocess
import os.path

dataset_url = "https://snap.stanford.edu/data/"
bench_urls = [
    "https://snap.stanford.edu/data/web-Stanford.txt.gz",
    "https://snap.stanford.edu/data/roadNet-CA.txt.gz",
    "https://snap.stanford.edu/data/bigdata/communities/com-orkut.all.cmty.txt.gz",
    "https://snap.stanford.edu/data/soc-LiveJournal1.txt.gz",
]

benches = list(map(lambda s: s.split("/")[-1][:-7], bench_urls))

print(benches)

for (url, b) in zip(bench_urls, benches):
    if os.path.exists(b + ".txt"):
        continue
    subprocess.run(["wget", "-nc", url).check_returncode()
    subprocess.run(["gzip", "-dfk", b + ".txt.gz"]).check_returncode()

for b in benches[:1]:
    with open(b + ".txt", "r") as f:
        s = "\n".join(map(lambda s: s + "\t" + "1", filter(lambda l: not l.startswith("#"), f.read().splitlines())))
    with open(b + ".in", "w") as f:
        f.write(s)
