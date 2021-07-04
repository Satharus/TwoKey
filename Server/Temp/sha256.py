import hashlib
password="test1234"
encoded_password = password.encode()
hash = hashlib.sha256(encoded_password)
print(hash.hexdigest())
