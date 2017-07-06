/* This code controls the drive consisting of two motors powered by sabertooth
   two friglie (i.e), Small LA for AA box one DC Motor which provides yaw for
   the analog camera

   Controls

   Drive:
   Forward  :w
   Right    :a
   Backword :s
   Left     :d

            D1        D2
   SLA1     q        ,ctrl + q
   SLA2     e        ,ctrl + e
   Cam      t        ,ctrl + t
*/

//SPI variables
int data;
int ss = 53;

int speed = 0;        //speed variable
int filter = 0.1;      //fileter variable
int limit = 80;
int accelUp = 15;    // acceleration variable
int accelDown = 12;
//Filter
int f_map[] = {0, 0};

//Decode and find the direction
int bits[] = {0, 0, 0, 0, 0, 0, 0, 0};

//Friglie and Camera motor pins
int LA1[] = {25, 27, 8};
int LA2[] = {46, 48, 7};
int CAM[] = {47, 49, 3};
int pwm = 200;

void setup () {
  //SPI
  pinMode(MISO, OUTPUT);
  pinMode(ss, OUTPUT);
  SPCR |= _BV(SPE);

  for (int i = 0; i < 3; i++) {
    pinMode(LA1[i], OUTPUT);
    pinMode(LA2[i], OUTPUT);
    pinMode(CAM[i], OUTPUT);
  }

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.println("Setup");
}

void loop () {
  digitalWrite(ss, LOW);
  if ((SPSR & (1 << SPIF)) != 0) {
    data = SPDR;
//    Serial.println(data);
    process(data);
    digitalWrite(ss, HIGH);
  }
}

void updateBits(int val) {
  for (int i = 0;i<8;i++) 
    bits[i] = getBit(val, i);
//  display();
}

void process(int input) {
  if (input) {
    updateBits(input);
    Serial.println(speed);
    speed += accelUp;    // sq function
    speed = (speed > limit) ? limit : speed;
    Serial.println(speed);
    drive(speed, pwm);
  } else {
    while (speed > 0) {
      Serial.println(speed);
      speed = (speed < accelDown) ? 0 : speed-accelDown;
      drive(speed, 0);
    }
  }
}

bool getBit(int n, int pos) {
  return (n >> pos) & 1;
}

void drive(int spd, int pwm) {
  algo(bits[7] - bits[5], bits[4] - bits[6], spd);
  act(LA1, abs(bits[0] - bits[3]), bits[0], pwm);               //  I  Cntrl    (Cntrl - I) Cntrl
  act(LA2, abs(bits[0] - bits[2]), bits[0], pwm);               //  0    1         1         1       //stop
  act(CAM, abs(bits[0] - bits[1]), bits[0], pwm);               //  1    0         abs(-1)   0
}                                                               //  1    1          0        1

void display() {
  for (int i = 0; i < 8; i++) {
    Serial.print(bits[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void algo(int a, int b, int spd) {
  int mapper1 = spd * round((a + b) / 2.0);
  int mapper2 = spd * round((a - b) / 2.0);
  f_map[0] = f_map[0] * filter + (1 - filter) * mapper1;
  f_map[1] = f_map[1] * filter + (1 - filter) * mapper2;
  Left(f_map[0]);
  Right(f_map[1]);
}

void Left(int t) {
  int x = map(t, -100, 100, 1, 127); // forward
  Serial.print("Left:");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(t);
  command(x);
}

void Right(int t) {
  int x = map(t, -100, 100, 129, 256); // backward
  Serial.print(" Right:");
  Serial.print(x);
  Serial.print(" ");
  Serial.println(t);
  command(x-1);
  command(x);
}

void act(int arr[], boolean A, boolean B, int pwm) {
  digitalWrite(arr[0], A);
  digitalWrite(arr[1], B);
  analogWrite(arr[2], pwm);
}

void command(int x) {                  //  High  Low  Latest 
  Serial1.write(x);                    //R 114   51    77.6 
  delay(5);                            //L 55.4  27.6  52.9 
  Serial2.write(x);                    //R 40.2  11.2  40.2   
  delay(5);                            //L 69.1  2.6   68.6  
  Serial3.write(x);                    //R 79.5  28.9  79.5   
  delay(5);                            //L 80    64.2  65.8  
}

