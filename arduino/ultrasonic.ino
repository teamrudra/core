#include <SnapFilter.h>

SnapFilter s( 0.01, 3, 255);

const int trigPin = 7;
const int echoPin = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int d, in, cm;

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(2);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  d = pulseIn(echoPin, HIGH);
  s.Smooth(d);

  in = microToInch(d);
  cm = microToCm(d);

  Serial.print(in);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println("");

  delay(100);
}

int microToInch(int m)
{
  return m / 74 / 2;
}

int microToCm(int m)
{
  return m / 29 / 2;
}

