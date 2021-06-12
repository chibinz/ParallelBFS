import subprocess
from glob import glob
from time import sleep

bfs = "../build/bfs"
ncpu = 28
repeat = 4

for r in range(repeat):
    print(f"*** Repetition {r} ***")
    for f in glob("*.in"):
        for c in range(ncpu):
            print(f"{f}\t{c}\t", end='', flush=True)
            sleep(1.0)
            subprocess.run(["taskset", "-c", f"0-{c}", bfs, f])


    for f in glob("rmat*.txt"):
        for c in range(ncpu):
            print(f"{f}\t{c}\t", end='', flush=True)
            subprocess.run(["taskset", "-c", f"0-{c}", bfs, f, "100000000", "1000000000"])
