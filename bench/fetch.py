import subprocess
import os.path

dataset_url = "https://snap.stanford.edu/data/"
benches = {
    "web-Stanford.txt",
    "roadNet-CA.txt",
    "com-Orkut.all.cmty.txt",
    "soc-LiveJournal1.txt",
}

for b in filter(lambda x: not os.path.exists(x), benches):
    name = b + ".gz"
    url = dataset_url + name
    subprocess.run(["wget", "-nc", url]).check_returncode()
    subprocess.run(["gzip", "-dfk", name]).check_returncode()
