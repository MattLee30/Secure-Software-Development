
def ensure(condition, message):
    if not condition:
        raise ValueError(message)