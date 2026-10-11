const int lamp = 2;

// Define pins for the ultrasonic sensor
const int sensor1EchoPin = 14;
const int sensor1TrigPin = 27;

const int sensor2EchoPin = 25;
const int sensor2TrigPin = 26;

const float maxDistance = 20;

bool lightOn = false;
unsigned long lightOnMaxTime = 5000;
unsigned long lightOnTimePrevious = 0;

// Cooldown variables
unsigned long sensor1LastTriggerTime = 0;
unsigned long sensor2LastTriggerTime = 0;
const unsigned long sensorCooldown = 1000; // 1 segundo

// Function prototype for reading sensor data
float readSensorData(String sensor);
bool verifySignal(String sensor);

void setup() {
  Serial.begin(115200);

  pinMode(sensor1EchoPin, INPUT);
  pinMode(sensor1TrigPin, OUTPUT);

  pinMode(sensor2EchoPin, INPUT);
  pinMode(sensor2TrigPin, OUTPUT);

  pinMode(lamp, OUTPUT);
  digitalWrite(lamp, LOW); 
  
  Serial.println("Ultrasonic sensor:");  
}










void loop() {
  unsigned long currentMillis = millis();

  // 1. Si la luz está encendida y se detecta nueva señal → apagarla
  if (lightOn && (verifySignal("sensor1") || verifySignal("sensor2"))) {
    lightOn = false;
    digitalWrite(lamp, LOW); 
    Serial.println("Light turned off by new signal!");
    delay(200);  // para evitar rebote inmediato
    return;
  }

  // 2. Si la luz está encendida y pasó el tiempo → apagarla
  if (lightOn && currentMillis - lightOnTimePrevious >= lightOnMaxTime) {
    lightOn = false;
    digitalWrite(lamp, LOW); 
    Serial.println("Light turned off by timeout!");
    return;
  }

  // 3. Si la luz está apagada y se detecta señal → encenderla
  if (!lightOn && (verifySignal("sensor1") || verifySignal("sensor2"))) {
    lightOn = true;
    digitalWrite(lamp, HIGH); 
    lightOnTimePrevious = currentMillis;
    Serial.println("Light turned on!");
    delay(200);  // para evitar rebote inmediato
    return;
  }

  delay(50);  // loop más fluido
}















//  if (verifySignal("sensor1") || verifySignal("sensor2")){
//      Serial.println("Signal detected...");
//      toggleTheLight();
//  }




//  delay(200);
//}



if (verifySignal("sensor1") || verifySignal("sensor2")){
    Serial.println("Signal detected...");
    turnOnTheLight();  // enciende solo si está apagada
}

void turnOnTheLight(){
  if (!lightOn) {
      lightOn = true;
      digitalWrite(lamp, HIGH); 
      lightOnTimePrevious = millis();
      Serial.println("Light turned on!");
  }
}

void toggleTheLight(){
  if (lightOn) {
      lightOn = false;
      digitalWrite(lamp, LOW); 
      Serial.println("Light turned off!");
  }
  else{
      lightOn = true;
      digitalWrite(lamp, HIGH); 
      lightOnTimePrevious = millis();
      Serial.println("Light turned on!");
  }
}

bool verifySignal(String sensor) {
  unsigned long currentMillis = millis();

  if (sensor == "sensor1" && currentMillis - sensor1LastTriggerTime < sensorCooldown) return false;
  if (sensor == "sensor2" && currentMillis - sensor2LastTriggerTime < sensorCooldown) return false;

  float distance = readSensorData(sensor);
  if (distance < maxDistance) {
    if (sensor == "sensor1") sensor1LastTriggerTime = currentMillis;
    if (sensor == "sensor2") sensor2LastTriggerTime = currentMillis;
    return true;
  }

  return false;
}

float readSensorData(String sensor) {
  int trigPin, echoPin;
  
  if (sensor == "sensor1") {
    trigPin = sensor1TrigPin;
    echoPin = sensor1EchoPin;
  } else if (sensor == "sensor2") {
    trigPin = sensor2TrigPin;
    echoPin = sensor2EchoPin;
  }

  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  unsigned long microsecond = pulseIn(echoPin, HIGH);
  float distance = microsecond / 29.00 / 2;
  return distance;
}
