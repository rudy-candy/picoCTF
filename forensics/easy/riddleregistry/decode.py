import base64
cipher = "cGljb0NURntwdXp6bDNkX20zdGFkYXRhX2YwdW5kIV9lZTQ1NDk1MH0="
plain = base64.b64decode(cipher).decode()
print(plain)