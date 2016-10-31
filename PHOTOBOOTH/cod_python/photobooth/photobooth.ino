/*
Interaccion entre Arduino y OpenCV
Codigo para Arduino
 
Por Glar3
*/
 
#define LED 13
#define SWITCH 4

void setup()
{
   //Iniciamos el Serial:
   Serial.begin(9600);
 
   //Inicializamos el pin 13
   pinMode(LED, OUTPUT);
   randomSeed(analogRead(0));
}
 
void loop()
{
  if(digitalRead(SWITCH)==HIGH){
    Serial.write('s');
   // digitalWrite(LED, HIGH);
    
  }else{
    //digitalWrite(LED, LOW);
    Serial.write('o');
  }
  delay(1000);
}
