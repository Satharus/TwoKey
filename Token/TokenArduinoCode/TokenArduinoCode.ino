#include <AESLib.h>

void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(3000);
}
  uint8_t message[17];
  uint8_t key[] = "rskDSkocuB6YuLxO";
  uint8_t iv[]  = "AAAAAAAAAAAAAAAA";
void loop() 
{
    if (Serial.available() > 0)
    {
      Serial.readBytes(message, 16);
      aes128_cbc_enc(key, iv, message, 16);
      for (int i = 0; i < 16; i++)
      {
        if (message[i] < 16) Serial.print("0");      
        Serial.print(message[i], HEX);
      }
      Serial.flush();
    }
}
