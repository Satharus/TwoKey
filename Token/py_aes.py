import pyaes

key = "rskDSkocuB6YuLxO"
plaintext = "ThisIsAGPTest123"
iv = "AAAAAAAAAAAAAAAA" #IV for CBC

aes = pyaes.AESModeOfOperationCBC(bytes(key.encode('utf-8')), iv = bytes(iv.encode('utf-8')))
ciphertext = aes.encrypt(bytes(plaintext.encode('utf-8')))

print("Encrypted: ", end='')
for a in ciphertext:
    print(hex(a), end=' ')
print()

aes = pyaes.AESModeOfOperationCBC(bytes(key.encode('utf-8')), iv = bytes(iv.encode('utf-8')))
decrypted = aes.decrypt(bytes(ciphertext))

print("Decrypted: " + str(decrypted.decode('utf-8')))

print(bytes(plaintext.encode('utf-8')) == decrypted)
