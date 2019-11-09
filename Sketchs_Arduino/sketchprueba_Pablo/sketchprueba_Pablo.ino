
#include <LiquidCrystal.h>

//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//|PONER EN COM4, INTERFAZ PROGRAMADA EN COM4|
//''''''''''''''''''''''''''''''''''''''''''''
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

const int rs = 33, en = 31, d4 = 29, d5 = 27, d6 = 25, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//OTRAS VARIABLES
int temperaturaThreshold = 16; //Temperatura a la cual de enciende la calefaccion

//Run Inicial ----------------------------------
void setup() {
  
  
  
  lcd.begin(16, 2);
  
  
  
  //INPUTs/OUTPUTs
  //pinMode(pinSensorMov, INPUT);
  //Detección MOV
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //Buzzer
  pinMode(pinBuzzer, OUTPUT);
  //Luz
  pinMode(pinLuzEntrada, OUTPUT);
  
  //Comunicación con puerto serie
  Serial.begin(9600);

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
    imprPant(input);
      
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
      String nuevaTemperatura = input.substring(input.indexOf("9") + 1);
      //Cambiar la temperatura de encendido de calefacción por la indicada por el usuario desde la GUI
      temperaturaThreshold = nuevaTemperatura.toInt(); 
      imprPant(nuevaTemperatura);
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
  //Deteccion de movimiento
  if(distanciaObj() < 20){
    if (Serial.available()){
      Serial.println("check");      
    }
    if(modoControlEntrada == 0){} //Estado: desactivado, no hacer nada
    else if (modoControlEntrada == 1){ //Estado: modo luz, encender la luz 10 secs
      digitalWrite(pinLuzEntrada, HIGH);
      delay(1000); //Esperar 10 segundos
      digitalWrite(pinLuzEntrada, LOW);
    }
    else if (modoControlEntrada == 2){ //Estado: modo alarma, pitar 3 veces
      for(int i = 0; i < 3; i++){
        tone(pinBuzzer, 1000); 
        delay(1000);
        noTone(pinBuzzer);
        delay(1000);
      }
    }   
    //delay(2000);
  }

  //... Resto de detecciones
  
  
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

//Debug Print Pantalla
void imprPant(String input){
  lcd.clear();
  lcd.print(input);
}
