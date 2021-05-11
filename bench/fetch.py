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

for i in range(len(benches)):
    if os.path.exists(benches[i] + ".txt"):
        continue
    subprocess.run(["wget", "-nc", bench_urls[i]]).check_returncode()
    subprocess.run(["gzip", "-dfk", benches[i] + ".txt.gz"]).check_returncode()
