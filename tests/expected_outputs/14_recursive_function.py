def fatorial(n):
    if n <= 1:
        return 1
    return n * fatorial(n - 1)

def main():
    f = fatorial(5)
    return 0