from cs50 import get_float


while True:
    change = get_float("Change: ") * 100
    if change > 0:
        break

total_coins = 0
coin_value = [25, 10, 5, 1]

for i in range(len(coin_value)):
    n = change // coin_value[i]
    total_coins += n
    change -= coin_value[i] * n

print(int(total_coins))
exit(0)
