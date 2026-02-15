import base64
cipher = "cGljb0NURntyM2RfMXNfdGgzX3VsdDFtNHQzX2N1cjNfZjByXzU0ZG4zNTVffQ=="
plain = base64.b64decode(cipher).decode()
print(plain)