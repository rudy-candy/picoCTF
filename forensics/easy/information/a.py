import base64
cipher = "cGljb0NURnt0aGVfbTN0YWRhdGFfMXNfbW9kaWZpZWR9"
plain = base64.b64decode(cipher).decode()
print(plain)