# imports,
from re import findall

from cs50 import get_string


# get str
Text = get_string("Text: ")

# findalls
Letters = findall("[a-zA-Z]", Text)
Words = findall("[ ]", Text)
Sentences = findall("[.!?]", Text)
WordCount = len(Words) + 1


Grade = round((0.0588 * (len(Letters) / WordCount * 100)) -
              (0.296 * (len(Sentences) / WordCount * 100)) - 15.8)


if Grade < 1:
    print("Before Grade 1")
elif Grade > 16:
    print("Grade 16+")
else:
    print(f"Grade: {Grade}")
