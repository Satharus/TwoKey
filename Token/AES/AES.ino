#include <AESLib.h>

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  uint8_t key[] = "rskDSkocuB6YuLxO";
  char data[17]   = "ThisIsAGPTest123";
  uint8_t iv[]  = "AAAAAAAAAAAAAAAA";
  
  aes128_cbc_enc(key, iv, data, 16);
  Serial.print("Encrypted: ");
  for (int i = 0; i < 16; i++)
  {
    char by[4]; 
    sprintf(by, "%x", (unsigned char) data[i]);
    Serial.print(by);
    Serial.print(" ");
  }
  Serial.println();
  aes128_cbc_dec(key, iv, data, 16);
  Serial.print("Decrypted: ");
  Serial.println(data);
}
