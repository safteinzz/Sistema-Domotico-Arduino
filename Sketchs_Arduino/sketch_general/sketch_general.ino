
/* Debug
#include <LiquidCrystal.h>
*/
#include <Servo.h>
//#include "DHT.h"
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//|PONER EN COM3, INTERFAZ PROGRAMADA EN COM3|
//''''''''''''''''''''''''''''''''''''''''''''
//DECLARACIÓN DE FUNCIONES (Declarar aqui las funciones creadas abajo)
int distanciaObj();

//ESTADO CONTROLES (Desactivados por defecto)
int modoControlEntrada = 0; //0 desactivado, 1 modo luz, 2 modo alarma
int modoControlTemperatura = 0; //0 desactivado, 1 activado | Si recibe 22: Posteriormente recibe temperatura nueva
int modoAlarmaIncendios = 0; //0 desactivado, 1 activado
int modoControlPersianas = 0; //0 desactivado, 1 activado

//DECLARACION DE PINES
const int pinLuzEntrada = 51;
const int pinBuzzer = 52;
const int trigPin = 50;
const int echoPin = 48;
const int sensorLuz = A0;
const int pinServo = 9;
const int redLed = 12;
const int greenLed = 11;
const int buzzer = 10;
const int smokeA0 = A5;

const int pinFrio = 5;
const int pinCalor = 6;
const int SENSOR=2;
//DHT dht (SENSOR, DHT11);
 
//DECLARACIONES HARDWARE
Servo servo;
/* SOLO PARA DEBUG, NO CONSTA EN EL PROYECTO FINAL
//LCD
const int rs = 33, en = 31, d4 = 29, d5 = 27, d6 = 25, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
*/

//OTRAS VARIABLES
int temperaturaThreshold = 16; //Temperatura a la cual de enciende la calefaccion
int sensorThres = 400;

//Run Inicial ----------------------------------
void setup() {

  /* Debug
  //Cargar LCD
  lcd.begin(16, 2);
  */
 
  //INPUTs/OUTPUTs
  //Detección de humo
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  
  //Detección de luz
  pinMode(pinServo,OUTPUT);
  pinMode(sensorLuz, INPUT);
  
  //pinMode(pinSensorMov, INPUT);
  //Detección MOV
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //Buzzer
  pinMode(pinBuzzer, OUTPUT);
  //Luz
  pinMode(pinLuzEntrada, OUTPUT);

  // Deteccion temperatura
  pinMode(pinFrio, OUTPUT);
  pinMode(pinCalor, OUTPUT);
  

  
  
  //Comunicación con puerto serie
  Serial.begin(9600);
  //dht.begin();
  servo.attach(pinServo);

  while(!Serial){
    ; //Esperar a que se realice la conexion
  }
}

//RUNTIME ---------------------------------------
void loop() {
  //Variables Adicionales
  //int sensorMov = digitalRead(pinSensorMov);

  //Lectura de Serial -----------------------------------
  if(Serial.available() > 0){ 
    String input = Serial.readString();
    //imprPant(input); //DEBUG
      
    if(input == "10"){ //Control entrada desactivado
      modoControlEntrada = 0;
    }
    else if(input == "11"){ //Control entrada modo Luz
      modoControlEntrada = 1;
    }
    else if(input == "12"){ //Control entrada modo alarma
      modoControlEntrada = 2;
    }
    else if(input == "20"){ //Control temperatura desactivado
      modoControlTemperatura = 0;
    }
    else if(input == "21"){ //Control temperatura activado
      modoControlTemperatura = 1;
    }
    else if(input.startsWith("229")){ //Cambio de temperatura
      //Serial.println(dht.readTemperature());
      String nuevaTemperatura = input.substring(input.indexOf("9") + 1);
      //Cambiar la temperatura de encendido de calefacción por la indicada por el usuario desde la GUI
      temperaturaThreshold = nuevaTemperatura.toInt(); 
   
      //imprPant(nuevaTemperatura); //DEBUG
    }
    else if(input == "30"){ //Alarma incendios desactivada
      modoAlarmaIncendios = 0;
    }
    else if(input == "31"){ //Alarma incendios activada
      modoAlarmaIncendios = 1;
    }
    else if(input == "40"){ //Control persianas desactivado
      modoControlPersianas = 0;
    }
    else if(input == "41"){ //Control persianas activado
      modoControlPersianas = 1;
    }
  }

  
  //Detecciones en sensores -----------------------------------
  //Deteccion de movimiento (Control Entrada)
  if(distanciaObj() < 20){
    if(modoControlEntrada == 0){} //Estado: desactivado, no hacer nada
    else if (modoControlEntrada == 1){ //Estado: modo luz, encender la luz 10 secs
      digitalWrite(pinLuzEntrada, HIGH);
      delay(1000); //Esperar 10 segundos
      digitalWrite(pinLuzEntrada, LOW);
    }
    else if (modoControlEntrada == 2){ //Estado: modo alarma, pitar 3 veces, enviar log a DB
      Serial.println("1"); //Guardar en BD fecha y hora de alarma  
      for(int i = 0; i < 3; i++){
        tone(pinBuzzer, 1000); 
        delay(1000);
        noTone(pinBuzzer);
        delay(1000);
      }
    }   
  }

  //Detección de humos
  if (modoAlarmaIncendios == 1)
  {
    int analogSensor = analogRead(smokeA0);

    if (analogSensor > sensorThres)
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      tone(buzzer, 1000, 200);
    }
    else
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      noTone(buzzer);
    }
  }
  
  //Detección luces control persianas
  if (modoControlPersianas == 1)
  {
   float l = analogRead(sensorLuz);
   int v = map(l,0,1024,0,179);
   servo.write(v);
  }


 // Control de temperatura
 if (modoControlTemperatura == 1)
 {
   //int temp = dht.readTemperature();
  int temp = 1;
   if(temp < temperaturaThreshold){
     digitalWrite(pinCalor,HIGH); 
     digitalWrite(pinFrio,LOW); 
    }
   else{
     digitalWrite(pinFrio,HIGH);
     digitalWrite(pinCalor,LOW);
   }
 }
 //FIN DE CONTROLES  
}


//Funciones Adicionales (Si se necesitaran) ------------------------------------

//Sacar distancia de objeto
int distanciaObj(){
  // Vars
  int distance;
  long duration;
  // Limpiar trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Lanzar sonar
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Leer sonar
  duration = pulseIn(echoPin, HIGH);
  // Calcular Distancia
  distance= duration*0.034/2;
  return distance;
} 

/*
//Debug Print Pantalla
void imprPant(String input){
  lcd.clear();
  lcd.print(input);
}
*/
