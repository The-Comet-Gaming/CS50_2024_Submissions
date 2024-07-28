from cs50 import get_int


while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break

for row in range(height):
    binary_offset = 1
    blocks = row + binary_offset
    spaces = height - blocks
    print(" " * spaces, "#" * blocks, sep="")
