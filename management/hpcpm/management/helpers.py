from itertools import tee


def pairwise(iterable):
    first, second = tee(iterable)
    next(second, None)
    return zip(first, second)
