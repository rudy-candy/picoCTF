import base64
cipher = "cGljb0NURntNRTc0RDQ3QV9ISUREM05fYjMyMDQwYjh9Cg=="
plain = base64.b64decode(cipher).decode()
print(plain)