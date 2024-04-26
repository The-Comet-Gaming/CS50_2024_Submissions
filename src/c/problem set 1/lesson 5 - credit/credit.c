#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void check_digit(int length, int len_input, string card_str, string digit_check, int digit_amnt,
                 string card_type);
void check_math(long card_math_num, int length);
int multiply_math(int digit_val, int sum);

int Card_Valid = 1;
string Card_Output = ("INVALID");

int main(void)
{
    long card_num = get_long("Input Card Number: ");

    char card_str[17];
    sprintf(card_str, "%li", card_num);

    int length = strlen(card_str);

    for (int a = 0; a < 2; ++a)
    {
        int pos_len[] = {13, 16};
        check_digit(length, pos_len[a], card_str, "4", 1, "VISA");
    }

    for (int a = 0; a < 2; ++a)
    {
        string possible_nums[] = {"34", "37"};
        check_digit(length, 15, card_str, possible_nums[a], 2, "AMEX");
    }

    for (int a = 0; a < 5; ++a)
    {
        string possible_nums[] = {"51", "52", "53", "54", "55"};
        check_digit(length, 16, card_str, possible_nums[a], 2, "MASTERCARD");
    }
    check_math(card_num, length);

    printf("%s\n", Card_Output);
}

void check_digit(int length, int len_input, string card_str, string digit_check, int digit_amnt,
                 string card_type)
{
    if (Card_Valid != 0 && length == len_input)
    {
        Card_Valid = strncmp(card_str, digit_check, digit_amnt);
        if (Card_Valid == 0)
        {
            Card_Output = card_type;
        }
    }
}

void check_math(long card_math_num, int length)
{
    if (Card_Valid == 0)
    {
        int current_pow = length - 1;
        int digit_val = 0;
        int even_or_odd = 0;
        int sum = 0;

        if (length % 2 != 0)
        {
            even_or_odd = 1;
        }

        for (int i = 0; i < length; ++i)
        {
            if (card_math_num < pow(10, current_pow))
            {
                digit_val = 0;
            }
            else
            {
                digit_val = card_math_num / pow(10, current_pow);
                card_math_num = card_math_num - digit_val * pow(10, current_pow);
            }
            --current_pow;

            if (even_or_odd == 1)
            {
                if (i % 2 == 0)
                {
                    sum = sum + digit_val;
                }
                else
                {
                    sum = sum + multiply_math(digit_val, sum);
                }
            }
            else
            {
                if (i % 2 != 0)
                {
                    sum = sum + digit_val;
                }
                else
                {
                    sum = sum + multiply_math(digit_val, sum);
                }
            }
        }

        if (sum % 10 != 0)
        {
            Card_Output = "INVALID";
        }
    }
}

int multiply_math(int digit_val, int sum)
{
    int multiplier = digit_val * 2;
    if (multiplier > 9)
    {
        multiplier = multiplier - 9;
    }
    return multiplier;
}
