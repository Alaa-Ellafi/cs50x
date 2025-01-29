# TODO
card = input("Number: ")
n = len(card)
s1 = 0
s2 = 0
for i in range(n//2):
    s1 += int(card[n-1-2*i])
    tmp = 2*int(card[n-2-2*i])
    s2 += tmp//10 + tmp % 10
if n % 2 == 0:
    s = s1 + s2
else:
    s = s1+s2+int(card[0])
if (n == 15) and (card[0] == "3") and (card[1] == "4" or card[1] == "7") and (s % 10 == 0):
    print("AMEX")
elif (n == 16) and (card[0] == "5") and (card[1] in ["1", "2", "3", "4", "5"]) and (s % 10 == 0):
    print("MASTERCARD")
elif ((n == 13) or (n == 16)) and (card[0] == "4") and (s % 10 == 0):
    print("VISA")
else:
    print("INVALID")
