#include "DHT.h"
 int SENSOR=2;
 int temp, humedad;
 int calor= 8;
 int frio= 9;
 int tempJava = 30;
 int activado = false;

 DHT dht (SENSOR, DHT11);
 
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(calor,OUTPUT);
  pinMode(frio,OUTPUT);
  digitalWrite(calor,LOW); 
  digitalWrite(frio,LOW); 
}

void loop() {

  humedad = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.println(temp);
  
  if(Serial.available() > 0){ 
    String recibido = Serial.readString();
    int temporal = recibido.toInt();
    
    if (temporal == 8){
      activado = false;
    } else {
      if (temporal == 9){
        activado = true;
      } else {
        if (activado == true){
          tempJava = temporal;
          if(temp>tempJava){
           digitalWrite(calor,HIGH); 
           digitalWrite(frio,LOW); 
          }
         else{
           digitalWrite(frio,HIGH);
           digitalWrite(calor,LOW);
         }
        }
      }
    }
    
    Serial.println(tempJava);
  }
 
  delay(5000);

}
