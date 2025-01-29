# TODO
from cs50 import get_string
text = get_string("Text: ")
n = len(text)
L = 0
S = 0
w = 1
for c in text:
    if c.isalpha():
        L += 1
    elif c in ['.', '?', '!']:
        S += 1
    elif c == ' ':
        w += 1
L = (L * 100) / w
S = (S * 100) / w
index = round(0.0588 * L - 0.296 * S - 15.8)
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
