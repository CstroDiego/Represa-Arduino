#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "LowPower.h"

// Definimos los pines para los LEDs y el pushbutton
const int LED_ROJO = 8;
const int LED_AZUL = 9;
const int PUSHBUTTON = 3;

SoftwareSerial hc(11, 10);  // Definimos el objeto de SoftwareSerial para el módulo HC-05

StaticJsonDocument<1280> doc;                    // Definimos el objeto de StaticJsonDocument con un tamaño de 1280 bytes
JsonArray data = doc.createNestedArray("data");  // Definimos el objeto de JsonArray para guardar los datos del sensor

unsigned long lastDataReceivedMillis = 0;   // Variable to store the last time the '3' data was received
const unsigned long dataTimeout = 5000;     // Timeout for not receiving '3' data (6 seconds)
unsigned long lastSenMillis = 0;            // Definimos la variable para guardar el último tiempo que se leyó el sensor
const unsigned long senInterval = 10;       // Definimos el intervalo de tiempo para leer el sensor (10 ms)
unsigned long lastDocMillis = 0;            // Definimos la variable para guardar el último tiempo que se envió el documento JSON
const unsigned long docInterval = 5000;     // Definimos el intervalo de tiempo para enviar el documento JSON (5 segundos)
unsigned long lastButtonMillis = 0;         // Definimos la variable para guardar el último tiempo que se presionó el botón
const unsigned long buttonInterval = 100;   // Definimos el intervalo de tiempo para leer el botón (100 ms)
const unsigned long sleepInterval = 10000;  // Definimos el intervalo de tiempo para entrar en modo ahorro de energía (60 segundos)
bool activo = true;                         // Definimos la variable para saber el sistema está activo o no
bool estadoBluetooth = false;               // Definimos la variable para saber si el sistema está conectado a un dispositivo
bool sendData = false;                      // Definimos la variable para saber si enviamos informacion o no


void setup() {
  pinMode(A3, INPUT);                 // Configuramos el pin A3 como entrada para leer el sensor
  pinMode(PUSHBUTTON, INPUT_PULLUP);  // Configuramos el pin del pushbutton como entrada con resistencia pull-up

  // Configuramos los pines de los LEDs y su estado inicial
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  ledActivo();

  Serial.begin(9600);
  Serial.println("Listo");

  hc.begin(38400);  // Inicializamos el objeto de SoftwareSerial para el módulo HC-05 con una velocidad de 38400 bps
}

void loop() {
  unsigned long currentDocMillis = millis();
  unsigned long currentSenMillis = millis();
  unsigned long currentButtonMillis = millis();
  unsigned long currentDataMillis = millis();

  // Cuando se recibe un dato que sea mayor a 0 se empieza a enviar datos
  if (hc.available()) {
    activo = true;
    estadoBluetooth = true;
    char data = hc.read();
    // Si el dato que recibe es '1' procede a enviar el json cada 5 seg
    if (data == '1') {
      sendData = true;
    }
    // Si lo que recibe es '2' deja de enviar el Json
    if (data == '2') {
      sendData = false;
    }
    // Keepalive que manda la aplicacion Android
    if (data == '3') {
      lastDataReceivedMillis = currentDataMillis;
    }
  }

  if (estadoBluetooth) {
    // Si se ha establecido la comunicación Bluetooth, se lee el sensor y se agrega el valor al JsonArray
    if (sendData && currentSenMillis - lastSenMillis > senInterval) {
      lastSenMillis = currentSenMillis;
      int sensorValue = analogRead(A3);  // Almacena el ultimo valor del sensor
      data.add(sensorValue);             // Anade el valor al documento
    }

    // Si han pasado 5 segundos desde el último envío del documento JSON y se ha establecido la comunicación Bluetooth, se envía el documento JSON
    if (sendData && currentDocMillis - lastDocMillis > docInterval) {
      lastDocMillis = currentDocMillis;
      serializeJson(doc, hc);                // Imprime el Json al modulo hc
      serializeJson(doc, Serial);            // Imprime el Json en el Serial
      hc.println();                          // Anade un salto de linea en el hc
      Serial.println();                      // Anade un solto de linea en el serial
      doc.clear();                           // Limpia el formato del Json
      data = doc.createNestedArray("data");  // Vuelve a crear el array data en el Json
    }
  } else {
    // Despues de haber pasado 60 segundos de inactividad entra el modo de ahorro de energia
    if (currentButtonMillis - lastButtonMillis > sleepInterval) {
      lastButtonMillis = currentButtonMillis;
      ledInactivo();
      activo = false;  // El estado de actividad se asigna como falso para entrar al modo de ahorro de energia
      Serial.println("Sistema entro en el modo de ahorro de energía.");
      delay(100);  // Entramos al modo de ahorro de energia
      goToSleep();
    }
  }

  // Cuando el dispositivo deje de recibir el keepalive por 3 segundos pasa el estado del bluetooth a desconectado
  if (currentDataMillis - lastDataReceivedMillis > dataTimeout) {
    lastDataReceivedMillis = currentDataMillis;
    estadoBluetooth = false;
  }

  // Se cambia el estado correspondiente de los leds
  if (activo) {
    ledActivo();
  } else {
    ledInactivo();
  }
}

void ledActivo() {
  // Cambio de estado de leds para el estado activo
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_AZUL, HIGH);
}

void ledInactivo() {
  // Cambio de estado de leds para el estado inactivo
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_AZUL, LOW);
}


void goToSleep() {
  attachInterrupt(digitalPinToInterrupt(PUSHBUTTON), wakeUp, LOW);  // Asignamos el boton para salir del modo de ahorro de energia
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);              // Entramos al modo de ahorro de energia hasta que el boton sea presionado
  detachInterrupt(digitalPinToInterrupt(PUSHBUTTON));               // Desasignamos el boton para salir del ahorro de energia
}

// Funcion para devolver el estado a activo cuando se sale del modo de bajo recurso
void wakeUp() {
  Serial.println("El sistema salio del modo de ahorro de energia");
  activo = true;
}
