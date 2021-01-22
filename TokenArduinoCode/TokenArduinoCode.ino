void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  char _signal;
    //Serial.print(Serial.available());
    if (Serial.availableForWrite() > 0)
    {
      _signal = Serial.read();
      //Serial.print(sig);

      //if (sig == '1')
      //{
        Serial.write("1234");
      //}
    }
}
