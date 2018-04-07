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

float ttfilter = 0.5, a = 0.8;

int s[] = {1500, 1500, 1500};                    //servo controllers
Servo yaw, pitch, roll;                          //Servo variables

int speed = 0, pwm = 200 , grpwm = 100;

int limit  = 50;
int accelUp = 1;
int lfilter, gfilter,tt;                         //linear actuator filter, gripper filter

int yawstep = 5,prstep = 1;                      //Servo steps 

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
  Serial3.write(64);
  Serial.println("Setup");
}

void loop() {
  digitalWrite(ss, LOW);
  if ((SPSR & (1 << SPIF)) != 0) {
    data = SPDR;
//    Serial.println(data);
    process(data);
    digitalWrite(ss, HIGH);
  }
}

void process(int input) {
  if (input) {
    updateBits(data);
    arm(1, pwm, grpwm);
    ypr();
  } else {
      arm(0, 0, 0);
      speed = 0;
  }
}


void arm(int val, int pwm, int grpwm) {
    
    turntable(bits[0] * val * (bits[6] - bits[7]));
    
    lfilter = !bits[0] * (lfilter * a + (1 - a) * pwm);
    gfilter = bits[0] * (gfilter * a + (1 - a) * grpwm);
    
    act(LA1, (bits[5] && (!bits[2])), (bits[4] && (!bits[2])), lfilter);
    act(LA2, (bits[7] && (!bits[3]) && (!bits[1])), (bits[6] && (!bits[3]) && (!bits[1])), lfilter);

    act(GRP, (bits[0] && bits[1] && bits[2] && bits[3]), (bits[4] && bits[5] && bits[6] && bits[7]), gfilter);
}

int safeservo(int x){
   if(x>2300) return 2400;
   else if(x<800) return 800;
   else return x;
}

void ypr() {
  if(bits[1]&&bits[2]&&bits[3]&&(!bits[0])){
    initial();
  }
  else if (bits[3]) {
     s[0] += yawstep*(bits[7]-bits[6]);
     s[0] = safeservo(s[0]);
//     Serial.print("yaw ");
     yaw.writeMicroseconds(s[0]);
  }
  else if (bits[2]) {
     s[1] += prstep*(bits[4]-bits[5]);
     s[2] += prstep*(bits[5]-bits[4]);
//     Serial.print("pitch ");
     s[1] = safeservo(s[1]);
     s[2] = safeservo(s[2]);
     pitch.writeMicroseconds(s[1]);
     roll.writeMicroseconds(s[2]);
  }
  else if (bits[1]) {
     s[1] += prstep*(bits[7]-bits[6]);
     s[2] += prstep*(bits[7]-bits[6]);
     s[1] = safeservo(s[1]);
     s[2] = safeservo(s[2]);
     pitch.writeMicroseconds(s[1]);
     roll.writeMicroseconds(s[2]);
  }
  
//  Serial.print(s[0]);
//  Serial.print(" ");
//  Serial.print(s[1]);
//  Serial.print(" ");
//  Serial.print(s[2]);
//  Serial.print(" ");
//  Serial.println();
}

void turntable(int x) {  
  
  speed = (speed >= limit) ? limit : speed + accelUp;   
  
  int y = map(speed, -100, 100, 1, 127);
  tt = (x)?tt * ttfilter + (1 - ttfilter) * y:64;
  Serial.print(x);
  Serial.print(" ");
  Serial.println(tt);
  Serial3.write(tt);
  delay(5);
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
//     display();
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
  s[0] = 1500,s[1] = 1200,s[2] = 1800;
  yaw.writeMicroseconds(s[0]);
  pitch.writeMicroseconds(s[1]);
  roll.writeMicroseconds(s[2]);
}

