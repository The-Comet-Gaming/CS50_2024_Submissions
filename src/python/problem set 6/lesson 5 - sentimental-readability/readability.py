import re

from cs50 import get_string


def main():
    text = get_string("Text: ")

    letters = len(re.findall("[a-zA-Z]", text))
    words = len(re.split(" ", text))
    sentences = len(re.findall("[.!?]", text))

    grade = round(div(0.0588, letters, words) - div(0.296, sentences, words) - 15.8)

    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade: {grade}")

    exit(0)


def div(n, l_or_s, words):
    return n * (L_or_S / words * 100)


main()
