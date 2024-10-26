#include <Servo.h>

int pulse;
int pulsestop;
int neutralpulse;
int setpoint;
int pulse_min = 1000;
int pulse_max = 2000;
Servo esc_prop; /*
Servo esc2_prop;
Servo esc3_prop;
Servo esc4_prop;
Servo esc5_prop;
Servo esc6_prop;
Servo esc7_prop;
Servo esc8_prop;*/

void setup() {
  Serial.begin(9600);
  //--------------------------------------------------------
  //escs_prop_attach(2,6,3,7,4,8,5,9);
  esc_prop_attach(2 /*,6,3,7,4,8,5,9*/);
  //--------------------------------------------------------

  //Calibrando
  Serial.println("Calibrando....");
  calibrateESC(1, pulse_min, pulse_max);  //calibrateESC(option, pulse_min, pulse_max) => option = 1(unidirectional) or  => option = 2(bidirectional)
  //  calibrateESCs();
}

void loop() {
  neutralpulse = pulse_min;
  pulsestop = pulse_min;
  delay(20000);
  //  neutralpulse = (pulse_max + pulse_min) / 2;
  float gain = 1.75;  // "Setar" valores maiores de 1.
  setpoint = gain * pulse_min;
  int opt = 1;//1 - Rampa Linear//2 - Rampa em saltos//3 - Degrau//4 - Impulso
  test_option(opt, setpoint);
  //  test_unidirectional3(setpoint);
  esc_stop(pulsestop);
  delay(1000);
}

void esc_prop_run(int pulse) {
  //--------------------------------------------------------
  esc_prop.writeMicroseconds(pulse);  //attatch the right motor to pin 2
}

void calibrateESC(int option, int pulse_min, int pulse_max) {
  //--------------------------------------------------------
  if (option = 1) {
    neutralpulse = pulse_min;
    esc_prop.writeMicroseconds(pulse_max);  //attatch the right motor to pin 2
    delay(2000);
    //--------------------------------------------------------
    esc_prop.writeMicroseconds(neutralpulse);  //attatch the right motor to pin 2
    delay(2000);
    //--------------------------------------------------------
  } else if (option = 2) {
    neutralpulse = (pulse_max + pulse_min) / 2;
    esc_prop.writeMicroseconds(pulse_max);  //attatch the right motor to pin 2
    delay(2000);
    //--------------------------------------------------------
    esc_prop.writeMicroseconds(neutralpulse);  //attatch the right motor to pin 2
    delay(2000);
    //--------------------------------------------------------
    esc_prop.writeMicroseconds(pulse_min);  //attatch the right motor to pin 2
    delay(2000);
    //--------------------------------------------------------
    esc_prop.writeMicroseconds(neutralpulse);  //attatch the right motor to pin 2
    delay(2000);
  }
}
/*
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
*/
void esc_prop_attach(int a) {
  esc_prop.attach(a);
}
/*
void escs_stop(int pulsestp) {
	esc1_prop.writeMicroseconds(pulsestp);   //the right motor to pin 2
  esc2_prop.writeMicroseconds(pulsestp);   //the left motor to pin 4
  esc3_prop.writeMicroseconds(pulsestp);   //the right motor to pin 6
  esc4_prop.writeMicroseconds(pulsestp);   //the left motor to pin 8
  esc5_prop.writeMicroseconds(pulsestp);   //the right motor to pin 3
  esc6_prop.writeMicroseconds(pulsestp);   //the left motor to pin 5
  esc7_prop.writeMicroseconds(pulsestp);   //the right motor to pin 7
  esc8_prop.writeMicroseconds(pulsestp);   //the left motor to pin 9
}
*/
// Parar motor
void esc_stop(int pulsestp) {
  esc_prop.writeMicroseconds(pulsestp);  //the right motor to pin 2
}
// Opções de testes
void test_option(int option, int value) {
  if (option == 1) {
    test_unidirectional1(value);
  }
  if (option == 2) {
    test_unidirectional2(value);
  }
  if (option == 3) {
    test_unidirectional3(value);
  }
  if (option == 4) {
    test_unidirectional4(value);
  }
}
// Rampa linear
void test_unidirectional1(int value) {  //value = setpoint
  esc_stop(pulsestop);
  // "Status: Rampa linear. Acelerando...."
  Serial.println("Status: Rampa linear. Acelerando....");
  for (pulse = pulse_min; pulse <= value; pulse = pulse + 5) {
    esc_prop_run(pulse);
    Serial.print("Pulso de saida: ");
    Serial.println(pulse);
    delay(50);
  }
  Serial.println("Status: patamar....");
  esc_prop_run(pulse);
  Serial.print("Pulso de saida: ");
  Serial.println(pulse);
  delay(10000);
  // "Status: desacelerando...."
  Serial.println("Status: desacelerando....");
  for (pulse = value; pulse >= pulse_min; pulse = pulse - 5) {
    esc_prop_run(pulse);
    Serial.print("Pulso de saida: ");
    Serial.println(pulse);
    delay(50);
  }
  esc_stop(pulsestop);
  delay(20000);
}

// Rampa em degraus
void test_unidirectional2(int value) {  //value = setpoint
  esc_stop(pulse_min);
  // "Status: Rampa em saltos. Acelerando...."
  Serial.println("Status: Rampa em saltos. Acelerando....");
  for (pulse = pulse_min; pulse <= value; pulse = pulse + 5) {
    esc_prop_run(pulse);
    Serial.print("Pulso de saida: ");
    Serial.println(pulse);
    delay(5000);
  }
  Serial.println("Status: patamar....");
  esc_prop_run(pulse);
  Serial.print("Pulso de saida: ");
  Serial.println(pulse);
  delay(10000);
  // "Status: desacelerando...."
  Serial.println("Status: desacelerando....");
  for (pulse = value; pulse >= pulse_min; pulse = pulse - 5) {
    esc_prop_run(pulse);
    Serial.print("Pulso de saida: ");
    Serial.println(pulse);
    delay(5000);
  }
}
// Degrau
void test_unidirectional3(int value) {  //value = setpoint
  esc_stop(pulsestop);
  // "Status: Degrau. Saindo da inércia...."
  Serial.println("Status: Degrau. Saindo da inércia....");
  int intvalue = 1.10 * pulse_min;
  esc_prop_run(intvalue);
  Serial.print("Pulso de saida: ");
  Serial.println(intvalue);
  delay(5000);
  // "Status: acelerando...."
  Serial.println("Status: salto up....");
  esc_prop_run(value);
  Serial.print("Pulso de saida: ");
  Serial.println(value);
  // "Status: patamar...."
  Serial.println("Status: patamar....");
  delay(10000);
  // "Status: desacelerando...."
  Serial.println("Status: salto down....");
  esc_prop_run(intvalue);
  Serial.print("Pulso de saida: ");
  Serial.println(intvalue);
  // "Status: desacelerando...."
  esc_stop(pulsestop);
  delay(20000);
}
// Impulso
void test_unidirectional4(int value) {  //value = setpoint
  esc_stop(pulsestop);
  // "Status: Impulso. Saindo da inércia...."
  Serial.println("Status: Impulso. Saindo da inércia....");
  int intvalue = 1.10 * pulse_min;
  esc_prop_run(intvalue);
  Serial.print("Pulso de saida: ");
  Serial.println(intvalue);
  delay(5000);
  // "Status: Impulso...."
  Serial.println("Status: Impulso. Valor de saida:");
  esc_prop_run(value);
  Serial.println(value);
  delay(500);
  // "Status: desacelerando...."
  Serial.println("Status: desacelerando....");
  esc_prop_run(intvalue);
  Serial.print("Pulso de saida: ");
  Serial.println(intvalue);
  // "Status: desacelerando...."
  esc_stop(pulsestop);
  delay(25000);
}
