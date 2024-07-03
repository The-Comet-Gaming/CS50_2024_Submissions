from cs50 import get_int


while True:
    n = get_int("Height: ")
    if n >= 1 and n <= 8:
        break


for i in range(n):
    blocks = i + 1
    spaces = n - blocks
    print(" " * spaces, end="")
    print("#" * blocks, end="")
    print("  ", end="")
    print("#" * blocks)
