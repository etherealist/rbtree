"""Test mpipe."""
import mpipe
from hypothesis import given
import hypothesis.strategies as st


@given(st.integers(
    max_value=2 ** 30,
    min_value=-2 ** 30,
))
def test_mpipe(num):
    """Test if test_mpipe retuns num + 1."""
    with mpipe.open_and_close(["./build/test_mpipe"]) as proc:
        mpipe.write(proc, (1, num))
        res = mpipe.read(proc)
        assert res[0] == num + 1


if __name__ == "__main__":
    proc = mpipe.open(["./build/test_mpipe"])
    mpipe.write(proc, (1, 0))
    res = mpipe.read(proc)
    print(res)
    mpipe.write(proc, (0,))
    mpipe.close(proc)
