void setup() 
{
  Serial.begin(9600);
}
  char message[17];
void loop() 
{
    if (Serial.available() >= 15)
    {
      Serial.readBytes(message, 16);
      for (int i = 0; i < 16; i++)
      {
        message[i] += 1;
      }
      Serial.print(message);
    }
}
