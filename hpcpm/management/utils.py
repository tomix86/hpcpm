def find(func, seq):
    for index, item in enumerate(seq):
        if func(item):
            return item, index
    return None, None
