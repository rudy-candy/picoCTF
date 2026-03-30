import hashlib

bUsername_trial = b"PRITCHARD"
user_key = 'picoCTF{1n_7h3_|<3y_of_'
arry = [4,5,3,6,2,7,1,8]

for i in arry:
    user_key += hashlib.sha256(bUsername_trial).hexdigest()[i]
user_key += '}'
print(user_key)