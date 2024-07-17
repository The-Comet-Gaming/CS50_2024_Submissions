# # do it as a str and iterate over it cast each number to an int for the math


# import req modules
from cs50 import get_int


# get card number input
CardNum = str(get_int("Number: "))

# check if
    # 1. card starting digits are correct
    # 2. if card length is correct
CardLen = len(CardNum)


def main():
    global CardNum
    global CardLen

    # it's messing up here because even if one is correct the other may not be so it returns invalid anyway
    check_card_len(13)
    check_card_len(16)

    # print AMEX\n / MASTERCARD\n / VISA\n / INVALID\n
    print(CardNumber)
    exit(0)


def check_card_len(n):
    global CardLen
    if CardLen != n:
        print(CardLen)
        print("INVALID")
        exit(1)
    else:
        print("Match Found")
        return


main()
