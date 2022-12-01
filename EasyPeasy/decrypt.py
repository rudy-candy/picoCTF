from pwn import *
from tqdm.auto import tqdm
import binascii
import string
KEY_LEN = 50000

encrypted = "51466d4e5f575538195551416e4f5300413f1b5008684d5504384157046e4959"


def get_message(slide, encrypted):
    io = remote("mercury.picoctf.net", 64260)
    for i in range(4):
        io.recvline()
    io.sendline("1"*(50000-slide))
    io.recvline()
    io.recvline()

    io.sendline(deconvert(encrypted))
    io.recvline()
    io.recvline()
    message = io.recvline().strip()
    return message


def convert(message):
    key = (chr(0)*len(message)).encode()
    result = list(map(lambda p, k: "{:02x}".format(ord(p) ^ k), message, key))
    return "".join(result)

def deconvert(message):
    dec = b""
    for i in range(0,len(message),2):
        dec+=chr(int(message[i:i+2],16)).encode()
    return dec

print(deconvert(encrypted))

message = get_message(32, encrypted)
message_dec = deconvert(message)
print(message_dec)