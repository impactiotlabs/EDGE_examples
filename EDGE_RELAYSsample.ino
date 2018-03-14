void setup() 
{
  Serial.begin (9600);
  pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  delay(1000);
}

void loop() 
{
  digitalWrite(9,HIGH);// relay1
  delay(1000);
  digitalWrite(9,LOW);
  delay(2000);
  
  digitalWrite(10,HIGH);// realy2
  delay(1000);
  digitalWrite(10,LOW);
  delay(2000);
  
}
