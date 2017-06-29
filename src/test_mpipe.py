"""Test mpipe."""
import mpipe


if __name__ == "__main__":
    proc = mpipe.open(["./build/test_mpipe"])
    mpipe.write(proc, (1, 0))
    res = mpipe.read(proc)
    print(res)
    mpipe.write(proc, (0,))
