import base64
cipher = ["cGljb0NURg==","ezF0X3c0cw==","bnRfdGg0dA==","XzM0c3lfdA==","YmhfNHJfOQ==","NjZkMGJmYg==","fQ=="]
plain = ""
for c in cipher:
    decord = base64.b64decode(c).decode()
    plain += decord
print(plain)