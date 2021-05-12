import subprocess
import os.path
import re

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
    subprocess.run(["wget", "-nc", url]).check_returncode()
    subprocess.run(["gzip", "-dfk", b + ".txt.gz"]).check_returncode()

for b in benches[:1]:
    s = ""
    with open(b + ".txt", "r") as f:
        for l in f.read().splitlines():
            if l.startswith("#"):
                if "Nodes" in l and "Edges" in l:
                    s += re.sub(r"# Nodes: (\d+) Edges: (\d+)", r"\1\t\1\t\2\n", l)
            else:
                s += l + "\t" + "1" + "\n"

    with open(b + ".in", "w") as f:
        f.write(s)
