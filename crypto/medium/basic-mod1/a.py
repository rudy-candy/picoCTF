import string

candidate = string.ascii_uppercase + string.digits + '_'
arr = [91, 322, 57, 124, 40, 406, 272, 147, 239, 285, 353, 272, 77, 110, 296, 262, 299, 323, 255, 337, 150, 102]

for a in arr:
     print(candidate[a%37], end='')