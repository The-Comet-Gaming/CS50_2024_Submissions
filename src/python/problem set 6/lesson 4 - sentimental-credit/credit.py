# # do it as a str and iterate over it cast each number to an int for the math


# PSUEDO-CODE
# imports
import re
from cs50 import get_int


# Get user to input card number to a str
CardNum = str(get_int("Number: "))

x = re.findall("[0-9]", CardNum)

CardType = "INVALID"

if len(x) == 13 or 16:
    CardType = "BOOP"

print(CardType)



# PSUEDO-CODE
# Get user to input card number to a str

# get length of card string
# intialise variable for the str of the card type as BLANK

# check_identifiers
# # Check if card length and identifier digits are valid
    # if false then exit() while printing("INVALID")

    # else assign Identified card type (AMERICAN EXPRESS, MASTERCARD, VISA) to a variable

# use re to get a list of the digits in the str

# check_sum
# # perform req math to check checksum iterating over the list while casting the chars to ints
    # if the checksum is valid then print the card type variable and exit(0)

    # else exit() and print ("INVALID")




# import req modules
#from cs50 import get_int


# get card number input and length
#CardNum = get_int("Number: ")
#CardLen = len(CardNum)


#def main():
    #global CardNum
    #global CardLen

    # it's messing up here because even if one is correct the other may not be so it returns invalid anyway
    #if check_card_len(13)
    #check_card_len(13)
    #check_card_len(16)

    # print AMEX\n / MASTERCARD\n / VISA\n / INVALID\n
    #print(CardNumber)
    #exit(0)


#def check_card_len(n):
    #global CardLen
    #if CardLen != n:
        #print(CardLen)
        #print("INVALID")
        #exit(1)
    #else:
        #print("Match Found")
        #return


#main()
