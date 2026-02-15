import base64
cipher = "cEF6endvcmQ="
plain = base64.b64decode(cipher).decode()
print(plain)