/*This code controls an arm which includes two Linear actuators,
  One power window for the turntable 3 servos for ypr and a dc motor
  for the gripper
  Controls:
         Direction1  Direction2
  LA1     UP         ,DOWN arrows
  LA2     Shift+UP   ,Shift+DOWN arrows
  TT      RIGHT      ,LEFT arrows

          D1      D2
  Gripper J      ,L
  Yaw     J+LEft ,J+Right
  PITCH   K+LEft ,K+Right
  ROLL    L+LEft ,L+Right
*/

#include <Servo.h>

//SPI variables
int data;
int ss = 53;
bool flag = false;

int bits[] = {0, 0, 0, 0, 0, 0, 0, 0};

int LA2[] =  { 43, 45, 9};                      // A, B, PWM short 0-1 up
int LA1[] =  { 35, 36, 7};                      // A, B, PWM long 0-1 up

int GRP[] =  { 39, 41, 8};                      // A, B, PWM grp 0-1 up

float a = 0.5, a1 = 0.7;

int s[] = {1500, 1500, 1500};                    //servo controllers
Servo yaw, pitch, roll;                          //Servo variables

int speed = 0, pwm = 200 , grpwm = 150;

int limit  = 50;
int accelUp = 10, accelDown = 10;
int lfilter, gfilter;                             //linear actuator filter, gripper filter

void setup() {
  //SPI
  pinMode(MISO, OUTPUT);
  pinMode(ss, OUTPUT);
  SPCR |= _BV(SPE);

  // LA outputs
  for (int i = 0; i < 3; i++) {
    pinMode(LA1[i], OUTPUT);
    pinMode(LA2[i], OUTPUT);
    pinMode(GRP[i], OUTPUT);
  }

  Serial.begin(9600);   //Debug
  Serial3.begin(9600);  // Turn Table

  //YPR pins
  yaw.attach(6);
  pitch.attach(5);
  roll.attach(4);
  initial();
  Serial.println("Setup");
}

void loop() {
  digitalWrite(ss, LOW);
  if ((SPSR & (1 << SPIF)) != 0) {
    data = SPDR;
    process(data);
    digitalWrite(ss, HIGH);
  }
}

void process(int input) {
  if (input) {
    updateBits(data);
    speed = (speed >= limit) ? limit : speed + accelUp;
    //    Serial.print(speed);
    //    Serial.print(" ");
    arm(speed, pwm, grpwm);
    ypr();
  } else {
      speed = 0;
      arm(speed, 0, 0);
  }
}


void arm(int speed, int pwm, int grpwm) {
  if (bits[0]) {
    turntable(speed * (bits[7] - bits[6]));
  }
  else {
    turntable(0 * (bits[7] - bits[6]));
    lfilter = lfilter * a + (1 - a) * pwm;
    gfilter = gfilter * a + (1 - a) * grpwm;
    act(LA1, (bits[5] && (!bits[2])), (bits[4] && (!bits[2])), lfilter);
    act(LA2, (bits[7] && (!bits[3]) && (!bits[1])), (bits[6] && (!bits[3]) && (!bits[1])), lfilter);
    act(GRP, (bits[3] && (!bits[7]) && (!bits[6])), (bits[1] && (!bits[7]) && (!bits[6])), gfilter);
  }
}

void ypr() {
  if (bits[3]) {
     s[0] += (bits[7]-bits[6]);
     Serial.print("yaw ");
     yaw.writeMicroseconds(s[0]);
  }
  else if (bits[2]) {
     s[1] += (bits[4]-bits[5]);
     s[2] += (bits[5]-bits[4]);
     Serial.print("pitch ");
     pitch.writeMicroseconds(s[1]);
     roll.writeMicroseconds(s[2]);
  }
  else if (bits[1]) {
     s[1] += (bits[7]-bits[6]);
     s[2] += (bits[7]-bits[6]);
     Serial.print("roll ");
     pitch.writeMicroseconds(s[1]);
     roll.writeMicroseconds(s[2]);
  }
  else if(bits[1]&&bits[2]&&bits[3]){
    initial();
  }
  Serial.print(s[0]);
  Serial.print(" ");
  Serial.print(s[1]);
  Serial.print(" ");
  Serial.print(s[2]);
  Serial.print(" ");
  Serial.println();
}

void turntable(int x) {
  int y = map(x, -100, 100, 1, 127);
//  Serial.print(x);
//  Serial.print(" ");
//  Serial.println(y);
  delay(10);
  Serial3.write(y);
  delay(10);
}

void act(int arr[], boolean A, boolean B, int pwm) {
  digitalWrite(arr[0], A);
  digitalWrite(arr[1], B);
  analogWrite(arr[2], pwm);
//  Serial.print("   ");
//  Serial.print(arr[0]);
//  Serial.print("   ");
//  Serial.print(arr[1]);
//  Serial.print("   ");
//  Serial.print(A);
//  Serial.print("   ");
//  Serial.println(B);
}

void updateBits(int val) {
  for (int i = 0; i < 8; i++)
    bits[i] = getBit(val, i);
  //   display();
}

bool getBit(int n, int pos) {
  return (n >> pos) & 1;
}

void display() {
  for (int i = 0; i < 8; i++) {
    Serial.print(bits[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void initial()
{
  //Initialize YPR
  Serial.println("Intializing YPR");
  yaw.writeMicroseconds(1500);
  pitch.writeMicroseconds(1500);
  roll.writeMicroseconds(1500);
  for (int i = 0; i < 3; i++) {
    s[i] = 1500;
  }
}

