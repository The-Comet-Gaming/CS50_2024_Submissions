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


    # 16 = 15 in binary
        # 14
        # 12
        # 10
        # 8
        # 6
        # 4
        # 2
        # 0
    # 15 = 14 in binary
        # 13
        # 11
        # 9
        # 7
        # 5
        # 3
        # 1
    # 13 = 12 in binary
        # 11
        # 9
        # 7
        # 5
        # 3
        # 1


'''
    card_digits = findall("[0-9]", card_string)
    index_to_multiply = card_length - 2
    checksum = 0

    loop = card_length - 1

    for loop in range(len(card_digits)):
        card_digits[loop] = int(card_digits[loop])
        index_to_multiply = (divided_length * 2) - 2

        if loop == index_to_multiply:
            divided_length -= 1
            multiplied_digit = card_digits[loop] * 2

            if multiplied_digit > 9:
                checksum += multiplied_digit - 9
            else:
                checksum += multiplied_digit
        else:
            checksum += card_digits[loop]

    if checksum % 10 == 0:
        print(card_type)
        exit(0)
    else:
        print("INVALID")
        exit(0)




    if card_length % 2 == 0:
        for k in range(card_length):
            if k % 2 == 0:
                checksum += multiplier(int(card_digits[k]))
            else:
                checksum += int(card_digits[k])
    else:
        for k in range(card_length):
            if k % 2 == 0:
                checksum += int(card_digits[k])
            else:
                checksum += multiplier(int(card_digits[k]))
'''



'''
def multiplier(digit):
    multiplied_number = digit * 2
    if multiplied_number > 9:
        multiplied_number = multiplied_number - 9
    return multiplied_number
'''

'''
    card_digits = findall("[0-9]", card_string)
    current_loop = 0
    n = card_length // 2
    checksum = 0

    for digit in card_digits:
        digit = int(digit)
        digit_to_multiply = card_length - (n * 2)

        if current_loop == digit_to_multiply:
            multiplied_digit = digit * 2

            if multiplied_digit > 9:
                checksum += multiplied_digit - 9
            else:
                checksum += multiplied_digit

        else:
            checksum += digit

    print(checksum)

    if checksum % 10 == 0:
        print(card_type)
        exit(0)
'''




    #n = 0
    #loop = c_len
    #checksum = 0

    #for i in range(len(c_nums)):
        #for j in range(len(start_nums)):
            #if c_len == lens[i] and search('(^)'+str(start_nums[j])+'()', card_str) != None:
                #while loop >= 0:
                    #index_to_multiply = c_len - 2 - (n * 2)
                    #if loop == index_to_multiply:
                        #n += 1
                        #multiplied_num = int(c_nums[i]) * 2
                        #if multiplied_num > 9:
                            #checksum += multiplied_num - 9
                        #else:
                            #checksum += multiplied_num
                    #else:
                        #checksum += int(c_nums[i])
                    #loop -= 1
            #else:
                #return

    #if checksum % 10 == 0:
        #print(c_name)
        #return
    #else:
        #return






'''
#from re import findall, search

#from cs50 import get_int


# Get user to input card number to a str
#CardStr = str(get_int("Number: "))
#Digits = findall("[0-9]", CardStr)
#Len = len(Digits)
#CardType = "INVALID"


#def main():
    #check_card("AMEX", [15], [34, 37])
    #check_card("VISA", [13, 16], [4])
    #check_card("MASTERCARD", [16], [51, 52, 53, 54, 55])

    #print(CardType)


#def check_card(name, lengths, startNums):
    #global CardType

    #for i in range(len(lengths)):
        #for j in range(len(startNums)):
            #if Len == lengths[i] and search('(^)'+str(startNums[j])+'()', CardStr) != None:
                #CheckSumTotal = 0
                #if Len % 2 == 0:
                    #for k in range(Len):
                        #if k % 2 == 0:
                            #CheckSumTotal = CheckSumTotal + multiplier(k)
                        #else:
                            #CheckSumTotal = CheckSumTotal + int(Digits[k])
                #else:
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
'''
