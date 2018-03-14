#include <NewPing.h>
int trigPin = 6;    //Trig 
int echoPin = 7;    //Echo 

void setup() 
{
  
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  

}

void loop() 
{ 
 
  int i=0;

  int total = 200;   //max distance is 400cm for this particular sensor
   
  long duration, cm;
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = (duration/2) / 29.1;
  //inches = (duration/2) / 74; 
 
  delay(1000);
  
 Serial.println(cm);
 

}
