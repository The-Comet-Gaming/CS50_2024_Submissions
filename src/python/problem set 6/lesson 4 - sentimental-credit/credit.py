from re import findall, search

from cs50 import get_int


card_string = str(get_int("Number: "))


def main():
    card_length = len(card_string)

    check_card(card_length, "AMEX", [15], ["34", "37"])
    check_card(card_length, "VISA", [13, 16], ["4"])
    check_card(card_length, "MASTERCARD", [16], ["51", "52", "53", "54", "55"])

    print("INVALID")
    exit(0)


def check_card(card_length, card_type, allowable_lengths, allowable_starting_numbers):
    if card_length not in allowable_lengths:
        return

    for prefix in allowable_starting_numbers:
        if card_string.startswith(prefix):
            checksum(card_length, card_type)


def checksum(card_length, card_type):
    card_digits = findall("[0-9]", card_string)

    index_to_multiply = card_length - 2
    loop = card_length - 1

    checksum = 0

    while loop >= 0:
        card_digits[loop] = int(card_digits[loop])

        if loop == index_to_multiply:
            multiplied_digit = card_digits[loop] * 2

            if multiplied_digit > 9:
                checksum += multiplied_digit - 9

            else:
                checksum += multiplied_digit

            index_to_multiply -= 2

        else:
            checksum += card_digits[loop]

        loop -= 1

    if checksum % 10 == 0:
        print(card_type)
        exit(0)

    else:
        print("INVALID")
        exit(0)


main()
