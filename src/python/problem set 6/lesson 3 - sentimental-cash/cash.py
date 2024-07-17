from cs50 import get_float


while True:
    Change = get_float("Change: ") * 100
    if Change > 0:
        break
TotalCoins = 0


def main():
    CalculateCoins(25)
    CalculateCoins(10)
    CalculateCoins(5)
    CalculateCoins(1)
    print(TotalCoins)


def CalculateCoins(coinValue):
    global Change
    if Change < coinValue:
        return
    else:
        global TotalCoins
        n = floor(Change / coinValue)
        TotalCoins = TotalCoins + n
        Change = Change - coinValue * n


main()
