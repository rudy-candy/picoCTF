import string

candidate = string.ascii_uppercase + string.digits + '_'
arr = [104, 372, 110, 436, 262, 173, 354, 393, 351, 297, 241, 86, 262, 359, 256, 441, 124, 154, 165, 165, 219, 288, 42 ]

for a in arr:
     print(candidate[pow(a,-1,41)-1], end='')