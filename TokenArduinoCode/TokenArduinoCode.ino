void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(3000);
}
  char message[17];
  int shift = 0;
void loop() 
{
    if (Serial.available() > 0)
    {
      Serial.readBytes(message, 16);
      for (int i = 0; i < 16; i++)
      {
        message[i] += shift;
      }
      shift = (shift+1)%15;
      Serial.write(message);
      Serial.flush();
    }
}
