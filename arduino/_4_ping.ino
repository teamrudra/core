#include <SnapFilter.h>

SnapFilter s( 0.01, 3, 255);

int trigPin[] = {22, 24, 26, 28};
int echoPin[] = {23, 25, 27, 29};

int i;
int d[4], in[4], cm[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(i = 0; i < 4; i++)
  {
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  d[0] = ping(trigPin[0], echoPin[0]);
  d[1] = ping(trigPin[1], echoPin[1]);
  d[2] = ping(trigPin[2], echoPin[2]);
  d[3] = ping(trigPin[3], echoPin[3]);

  for(i=0;i<4;i++)
  {
    in[i] = microToInch(d[i]);
    cm[i] = microToCm(d[i]);
  }
  
  print();

  delay(100);
}


int ping(int tPin, int ePin)
{
  int dis;
  digitalWrite(tPin, LOW);
  delay(2);
  digitalWrite(tPin, HIGH);
  delay(2);
  digitalWrite(tPin, LOW);
  
  dis = pulseIn(ePin, HIGH);
  dis = s.Smooth(dis);
  return dis;
}

int microToInch(int m)
{
  return m / 74 / 2;
}

int microToCm(int m)
{
  return m / 29 / 2;
}

void print()
{
 for(i=0;i<4;i++)
  {
    Serial.print(in[i]);
    Serial.print("in, ");
    Serial.print(cm[i]);
    Serial.print("cm");
    Serial.print("\t");
  } 
  Serial.println("");
}
