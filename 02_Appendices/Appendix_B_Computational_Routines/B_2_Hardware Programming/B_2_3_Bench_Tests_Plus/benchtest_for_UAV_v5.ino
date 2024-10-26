#include <Servo.h>

int pulse;
Servo esc1_prop;
Servo esc2_prop;
Servo esc3_prop;
Servo esc4_prop;
Servo esc5_prop;
Servo esc6_prop;
Servo esc7_prop;
Servo esc8_prop;

void setup() {
  Serial.begin(9600);
  //--------------------------------------------------------
  escs_prop_attach(2,6,3,7,4,8,5,9);
  //--------------------------------------------------------
  calibrateESC();
}

void loop() {
  int inputpulse = 1675;
  int neutralpulse = 1500;
  Serial.println("Status: decolando...");
  for (pulse = neutralpulse; pulse <= inputpulse; pulse = pulse + 1) {
    esc_running_throttleZ_pos(pulse);   //throttleZ Up
    //--------------------------------------------------------------------
	Serial.print("Saida aplicada nos ESCs: ");
    Serial.println(pulse);
    delay(50);
  }
    //--------------------------------------------------------------------
  delay(5000);
    //--------------------------------------------------------------------
  Serial.println("Status: deslocamento para frente...");
    //--------------------------------------------------------------------
  esc_running_throttleX_pos(inputpulse);
    //--------------------------------------------------------------------
  delay(5000);
    //--------------------------------------------------------------------
  esc_running_throttleX_neg(inputpulse);
    //--------------------------------------------------------------------
  delay(5000);
    //--------------------------------------------------------------------
  Serial.println("Status: deslocamento para o lado...");
    //--------------------------------------------------------------------
  esc_running_throttleY_pos(inputpulse);
    //--------------------------------------------------------------------
  delay(5000);
    //--------------------------------------------------------------------
  esc_running_throttleY_neg(inputpulse);
    //--------------------------------------------------------------------

  Serial.println("Status: pousando...");
    //--------------------------------------------------------------------
  for (pulse = inputpulse; pulse >= neutralpulse; pulse = pulse - 1) {
    esc_running_throttleZ_pos(pulse);
    Serial.print("Saida aplicada nos ESCs:");
    Serial.println(pulse);
  }
  delay(10000);
}

void escs_prop_attach(int a,int b,int c,int d,int e,int f,int g,int h) {
  esc1_prop.attach(a);
  esc2_prop.attach(b);
  esc3_prop.attach(c);
  esc4_prop.attach(d);
  esc5_prop.attach(e);
  esc6_prop.attach(f);
  esc7_prop.attach(g);
  esc8_prop.attach(h);
}

void calibrateESC() {
  //--------------------------------------------------------
  esc1_prop.writeMicroseconds(2000);  //attatch the right motor to pin 2
  esc2_prop.writeMicroseconds(2000);   //attatch the left motor to pin 4
  esc3_prop.writeMicroseconds(2000);   //attatch the right motor to pin 6
  esc4_prop.writeMicroseconds(2000);   //attatch the left motor to pin 8
  esc5_prop.writeMicroseconds(2000);  //attatch the right motor to pin 3
  esc6_prop.writeMicroseconds(2000);   //attatch the left motor to pin 5
  esc7_prop.writeMicroseconds(2000);   //attatch the right motor to pin 7
  esc8_prop.writeMicroseconds(2000);   //attatch the left motor to pin 9
  delay(2000);
  //--------------------------------------------------------
  esc1_prop.writeMicroseconds(1500);  //attatch the right motor to pin 2
  esc2_prop.writeMicroseconds(1500);   //attatch the left motor to pin 4
  esc3_prop.writeMicroseconds(1500);   //attatch the right motor to pin 6
  esc4_prop.writeMicroseconds(1500);   //attatch the left motor to pin 8
  esc5_prop.writeMicroseconds(1500);  //attatch the right motor to pin 3
  esc6_prop.writeMicroseconds(1500);   //attatch the left motor to pin 5
  esc7_prop.writeMicroseconds(1500);   //attatch the right motor to pin 7
  esc8_prop.writeMicroseconds(1500);   //attatch the left motor to pin 9
  delay(2000);
  //--------------------------------------------------------
  esc1_prop.writeMicroseconds(1000);  //attatch the right motor to pin 2
  esc2_prop.writeMicroseconds(1000);   //attatch the left motor to pin 4
  esc3_prop.writeMicroseconds(1000);   //attatch the right motor to pin 6
  esc4_prop.writeMicroseconds(1000);   //attatch the left motor to pin 8
  esc5_prop.writeMicroseconds(1000);  //attatch the right motor to pin 3
  esc6_prop.writeMicroseconds(1000);   //attatch the left motor to pin 5
  esc7_prop.writeMicroseconds(1000);   //attatch the right motor to pin 7
  esc8_prop.writeMicroseconds(1000);   //attatch the left motor to pin 9
  delay(2000);
  //--------------------------------------------------------
  esc1_prop.writeMicroseconds(1500);  //attatch the right motor to pin 2
  esc2_prop.writeMicroseconds(1500);   //attatch the left motor to pin 4
  esc3_prop.writeMicroseconds(1500);   //attatch the right motor to pin 6
  esc4_prop.writeMicroseconds(1500);   //attatch the left motor to pin 8
  esc5_prop.writeMicroseconds(1500);  //attatch the right motor to pin 3
  esc6_prop.writeMicroseconds(1500);   //attatch the left motor to pin 5
  esc7_prop.writeMicroseconds(1500);   //attatch the right motor to pin 7
  esc8_prop.writeMicroseconds(1500);   //attatch the left motor to pin 9
  delay(2000);
}

void esc_running_throttleZ_pos(int pulse) {
	esc1_prop.writeMicroseconds(pulse);   //the right motor to pin 2
    esc2_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 4
    esc3_prop.writeMicroseconds(3000-pulse);        //the right motor to pin 6
    esc4_prop.writeMicroseconds(pulse);   //the left motor to pin 8
    esc5_prop.writeMicroseconds(pulse);   //the right motor to pin 3
    esc6_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 5
    esc7_prop.writeMicroseconds(3000-pulse);        //the right motor to pin 7
    esc8_prop.writeMicroseconds(pulse);   //the left motor to pin 9
}

void esc_running_throttleX_pos(int pulse) {
	  esc1_prop.writeMicroseconds(1500);         //the right motor to pin 2
    esc2_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 4
    esc3_prop.writeMicroseconds(3000-pulse);        //the right motor to pin 6
    esc4_prop.writeMicroseconds(pulse);   //the left motor to pin 8
    esc5_prop.writeMicroseconds(pulse);   //the right motor to pin 3
    esc6_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 5
    esc7_prop.writeMicroseconds(3000-pulse);        //the right motor to pin 7
    esc8_prop.writeMicroseconds(pulse);         //the left motor to pin 9
}

void esc_running_throttleX_neg(int pulse) {
	  esc1_prop.writeMicroseconds(pulse);   //the right motor to pin 2
    esc2_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 4
    esc3_prop.writeMicroseconds(3000-pulse);        //the right motor to pin 6
    esc4_prop.writeMicroseconds(1500);         //the left motor to pin 8
    esc5_prop.writeMicroseconds(pulse);         //the right motor to pin 3
    esc6_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 5
    esc7_prop.writeMicroseconds(3000-pulse);        //the right motor to pin 7
    esc8_prop.writeMicroseconds(pulse);   //the left motor to pin 9
}

void esc_running_throttleY_pos(int pulse) {
    esc1_prop.writeMicroseconds(pulse);   //the right motor to pin 2
    esc2_prop.writeMicroseconds(1500);         //the left motor to pin 4
    esc3_prop.writeMicroseconds(3000-pulse);        //the right motor to pin 6
    esc4_prop.writeMicroseconds(pulse);   //the left motor to pin 8
    esc5_prop.writeMicroseconds(pulse);   //the right motor to pin 3
    esc6_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 5
    esc7_prop.writeMicroseconds(3000-pulse);         //the right motor to pin 7
    esc8_prop.writeMicroseconds(pulse);   //the left motor to pin 9
}

void esc_running_throttleY_neg(int pulse) {
    esc1_prop.writeMicroseconds(pulse);   //the right motor to pin 2
    esc2_prop.writeMicroseconds(3000-pulse);         //the left motor to pin 4
    esc3_prop.writeMicroseconds(1500);        //the right motor to pin 6
    esc4_prop.writeMicroseconds(pulse);   //the left motor to pin 8
    esc5_prop.writeMicroseconds(pulse);   //the right motor to pin 3
    esc6_prop.writeMicroseconds(3000-pulse);        //the left motor to pin 5
    esc7_prop.writeMicroseconds(3000-pulse);         //the right motor to pin 7
    esc8_prop.writeMicroseconds(pulse);   //the left motor to pin 9
}

void esc_normal_test(int pulse) {
	esc1_prop.writeMicroseconds(pulse);   //the right motor to pin 2
    delay(15000);
    esc1_prop.writeMicroseconds(1500);   //the right motor to pin 2
    esc2_prop.writeMicroseconds(pulse);   //the left motor to pin 4
    delay(15000);
    esc2_prop.writeMicroseconds(1500);   //the left motor to pin 4
    esc3_prop.writeMicroseconds(pulse);   //the right motor to pin 6
    delay(15000);
    esc3_prop.writeMicroseconds(1500);   //the right motor to pin 6
    esc4_prop.writeMicroseconds(pulse);   //the left motor to pin 8
    delay(15000);
    esc4_prop.writeMicroseconds(1500);   //the left motor to pin 8
    esc5_prop.writeMicroseconds(pulse);   //the right motor to pin 3
    delay(15000);
    esc5_prop.writeMicroseconds(1500);   //the right motor to pin 3
    esc6_prop.writeMicroseconds(pulse);   //the left motor to pin 5
    delay(15000);
    esc6_prop.writeMicroseconds(1500);   //the left motor to pin 5
    esc7_prop.writeMicroseconds(pulse);   //the right motor to pin 7
    delay(15000);
    esc7_prop.writeMicroseconds(1500);   //the right motor to pin 7
    esc8_prop.writeMicroseconds(pulse);   //the left motor to pin 9
    delay(15000);
    esc8_prop.writeMicroseconds(1500);   //the left motor to pin 9
}

void esc_stop() {
	esc1_prop.writeMicroseconds(1500);   //the right motor to pin 2
    esc2_prop.writeMicroseconds(1500);   //the left motor to pin 4
    esc3_prop.writeMicroseconds(1500);   //the right motor to pin 6
    esc4_prop.writeMicroseconds(1500);   //the left motor to pin 8
    esc5_prop.writeMicroseconds(1500);   //the right motor to pin 3
    esc6_prop.writeMicroseconds(1500);   //the left motor to pin 5
    esc7_prop.writeMicroseconds(1500);   //the right motor to pin 7
    esc8_prop.writeMicroseconds(1500);   //the left motor to pin 9
}
