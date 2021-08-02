#include <Servo.h>

/*Station Leds & Sensors Control*/
#define trigPinB 22 //Trigpin for ultrasonic before train 
#define echoPinB 23
Servo servo;
int buzzer = 24; // station buzzer
int sred = 25; // station red
int syellow = 26; // station yellow
int sgreen = 27; // station green

/*Street Leds Control*/
#define trigPinB 28 //Trigpin for ultrasonic at street start 
#define echoPinB 29
#define trigPinA 30 //Trigpin for ultrasonic after at street end
#define echoPinA 31
int ledcontrol = 3;

/*Emergancy Switches & Leds Control*/
int ambulancePin = A0;
int ambulanceValue;
int controlambulance = 32;
int firePin = A1;
int fireValue;
int controlfire = 33;

/*Traffic Leds Control*/
int tr = 36; // traffic red led
int ty = 37; // traffic yellow led
int tg = 38; // traffic green led

void setup() {
  Serial.begin (9600);

  /*Station Leds & Sensors Pins set*/
  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sred, OUTPUT);
  pinMode(syellow, OUTPUT);
  pinMode(sgreen, OUTPUT);
  servo.attach(2);

  /*Street Leds & Sensors Pins set*/
  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);
  pinMode(trigPinA, OUTPUT);
  pinMode(echoPinA, INPUT);
  pinMode(ledcontrol, OUTPUT);
  analogWrite( ledcontrol, 10);

  /*Emergancy Switches & Leds Pins set*/
  pinMode(controlambulance, OUTPUT);
  pinMode(controlfire, OUTPUT);
  digitalWrite(controlambulance, LOW);
  digitalWrite(controlfire, LOW);

  /*Traffic Pins set*/
  pinMode(tr, OUTPUT);
  pinMode(ty, OUTPUT);
  pinMode(tg, OUTPUT);
}


void autoStation()
{
  /*Station gates closes if there is a train and turn on led light*/
  long durationS, distanceS;
  digitalWrite(sgreen, HIGH);
  digitalWrite(trigPinB, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinB, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinB, LOW);
  durationS = pulseIn(echoPinB, HIGH);
  distanceS = durationS * 0.034 / 2;
  if (distanceS < 70) {
    digitalWrite( buzzer, HIGH);  //buzzer works
    digitalWrite(sgreen, LOW);
    digitalWrite(syellow, HIGH);
    delay(1000); //distance between servo and sensor
    digitalWrite(syellow, LOW);
    digitalWrite(sred, HIGH);
    servo.write(90);//servo close
  }
  delay(7000);// till train passes
  digitalWrite( buzzer, LOW);
  digitalWrite(sred, LOW);
  digitalWrite(sgreen, HIGH);
  servo.write(0);
  delay(500);
}

void autoStreetLights()
{
  /*Turn on street leds only if there is a car */
  long durationB, distanceB;
  digitalWrite(trigPinB, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinB, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinB, LOW);
  durationB = pulseIn(echoPinB, HIGH);
  distanceB = durationB * 0.034 / 2;
  if (distanceB < 5) {
    analogWrite( ledcontrol, 255);
    delay(1000);
  }
  long durationA, distanceA;
  digitalWrite(trigPinA, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinA, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinA, LOW);
  durationA = pulseIn(echoPinA, HIGH);
  distanceA = durationA * 0.034 / 2;
  if (distanceA < 5) {
    analogWrite( ledcontrol, 20);
    delay(1000);
  }
}

void checkEmergancyAlarm()
{
  /*Emergancy box*/
  ambulanceValue = analogRead(ambulancePin); // read the value from the abulance switch
  Serial.println(ambulanceValue);
  if (ambulanceValue < 100) {
    digitalWrite(controlambulance, HIGH);
    delay(5000);
  }
  digitalWrite(controlambulance, LOW);

  fireValue = analogRead(firePin); // read the value from the abulance switch
  Serial.println(fireValue);
  if (fireValue < 100) {
    digitalWrite(controlfire, HIGH);
    delay(5000);
  }
  digitalWrite(controlfire, LOW);
}
void trafficLights()
{
  /*Traffic Lights work in Periodic form*/
  digitalWrite(tr, HIGH);
  delay(9000);
  digitalWrite(tr, LOW);
  delay(300);

  digitalWrite(ty, HIGH);
  delay(3000);
  digitalWrite(ty, LOW);
  delay(300);

  digitalWrite(tg, HIGH);
  delay(9000);
  digitalWrite(tg, LOW);
  
}

void loop() {
  autoStation();
  autoStreetLights();
  checkEmergancyAlarm();
  trafficLights();
}
