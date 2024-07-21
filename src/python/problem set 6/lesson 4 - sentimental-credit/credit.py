from re import findall, search

from cs50 import get_int


# Get user to input card number to a str
CardStr = str(get_int("Number: "))
Digits = findall("[0-9]", CardStr)
Len = len(Digits)
CardType = "INVALID"


def main():
    check_card("AMEX", [15], [34, 37])
    check_card("VISA", [13, 16], [4])
    check_card("MASTERCARD", [16], [51, 52, 53, 54, 55])

    print(CardType)


def check_card(name, lengths, startNums):
    global CardType

    for i in range(len(lengths)):
        for j in range(len(startNums)):
            if Len == lengths[i] and search('(^)'+str(startNums[j])+'()', CardStr) != None:
                CheckSumTotal = 0
                if Len % 2 == 0:
                    for k in range(Len):
                        if k % 2 == 0:
                            CheckSumTotal = CheckSumTotal + multiplier(k)
                        else:
                            CheckSumTotal = CheckSumTotal + int(Digits[k])
                else:
                    for k in range(Len):
                        if k % 2 == 0:
                            CheckSumTotal = CheckSumTotal + int(Digits[k])
                        else:
                            CheckSumTotal = CheckSumTotal + multiplier(k)
                if CheckSumTotal % 10 == 0:
                    CardType = name


def multiplier(k):
    MultipliedNum = int(Digits[k]) * 2
    if MultipliedNum > 9:
        MultipliedNum = MultipliedNum - 9
    return MultipliedNum


main()
