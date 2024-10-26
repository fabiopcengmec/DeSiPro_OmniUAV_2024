// https://github.com/QuadTinnakon/Arduino-Balance-a-Brushless-Motor-and-Propeller-Balancing-/blob/master/mpu6050.h
// https://forum.arduino.cc/t/esc-doesnt-go-full-capacity/981483
// https://www.makerhero.com/blog/tutorial-acelerometro-mpu6050-arduino/
// 
// SCL - Laranja D21
// SDA - Amarelo D20
//
// Olá para indentar no Notepad++ eu utilizo as teclas shift+tab 
// selecionando um trecho ou o código completo com as teclas ctrl+A.
// Espero ter ajudado.
// 
//------------------------------------------------------------------------------------------------------------------
// including the libraries
#include <Wire.h>
#include <Servo.h>
// including the libraries
Servo esc1;
Servo esc2;
//
/*MPU-6050 gives you 16 bits data so you have to create some 16int constants
 * to store the data for accelerations and gyro*/
int16_t Acc_rawX, Acc_rawY, Acc_rawZ, Gyr_rawX, Gyr_rawY, Gyr_rawZ;

float Acceleration_angle[2];
float Gyro_angle[2];
float Total_angle[2];

float elapsedTime, time, timePrev;
int i;
float rad_to_deg = 180 / 3.141592654;

float PID, pwmLeft, pwmRight, error, previous_error;
float pid_p = 0;
float pid_i = 0;
float pid_d = 0;
/////////////////PID CONSTANTS/////////////////
double kp = 6;      //3.55//.05778
double ki = 0.00031578;  //0.2003 //0.005
double kd = 0.07894;   //2.05
///////////////////////////////////////////////

// double throttle = 1300;   //initial value of throttle to the motors
double throttle = 1600;   //initial value of throttle to the motors
float desired_angle = 0;  //This is the angle in which we whant the
                          //balance to stay steady

/*int analogPulseMin = 115;//115
int analogPulseCenter = 127;
int analogPulseMax = 255;*/

void setup() {
  //---------------------------------------------------------------------------
/*  Serial.println("Program begin...");
  delay(1000);
  Serial.println("This program will start the ESC.");
  Serial.print("Now writing maximum output: (");
  Serial.print(2000);
  Serial.print(" us in this case)");
  Serial.print("\n");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
 */

  //---------------------------------------------------------------------------
  esc1.attach(23);  //attatch the right motor to pin 3
  esc2.attach(25);   //attatch the left motor to pin 5
/*
// Wait for input
  while (!Serial.available());
  Serial.read();

  // Send min output
  Serial.println("\n");
  Serial.println("\n");
  Serial.print("Sending minimum output: (");
  Serial.print(1000);
  Serial.print(" us in this case)");
  Serial.print("\n");
  motor.writeMicroseconds(1000);
  Serial.println("The ESC is calibrated");
  Serial.println("----");
  Serial.println("Now, type a values between 1000 and 2000 and press enter");
  Serial.println("and the motor will start rotating.");
  Serial.println("Send 1000 to stop the motor and 2000 for full throttle");

*/
  time = millis();  //Start counting time in milliseconds
                    /*In order to start up the ESCs we have to send a min value
   * of PWM to them before connecting the battery. Otherwise
   * the ESCs won't start up or enter in the configure mode.
   * The min value is 1000us and max is 2000us, REMEMBER!*/
  //right_prop.writeMicroseconds(1480);
  //left_prop.writeMicroseconds(1480);
  //delay(10000);
  esc1.writeMicroseconds(2000);
  esc2.writeMicroseconds(2000);
  delay(1000);
  esc1.writeMicroseconds(1500);
  esc2.writeMicroseconds(1500);
  delay(1000);
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
  delay(1000);
  esc1.writeMicroseconds(1500);
  esc2.writeMicroseconds(1500);
  delay(1000);
//////////////////////////////////////////////////////////////
  Wire.begin();  //begin the wire comunication
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(250000);  // Serial.begin(9600); 
}  //end of setup void

void loop() {

  /////////////////////////////I M U/////////////////////////////////////
  timePrev = time;  // the previous time is stored before the actual time read
  time = millis();  // actual time read
  elapsedTime = (time - timePrev) / 1000;

  /*The tiemStep is the time that elapsed since the previous loop. 
   * This is the value that we will use in the formulas as "elapsedTime" 
   * in seconds. We work in ms so we haveto divide the value by 1000 
   to obtain seconds*/

  /*Reed the values that the accelerometre gives.
   * We know that the slave adress for this IMU is 0x68 in
   * hexadecimal. For that in the RequestFrom and the 
   * begin functions we have to put this value.*/

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  //Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);

  /*We have asked for the 0x3B register. The IMU will send a brust of register.
    * The amount of register to read is specify in the requestFrom function.
    * In this case we request 6 registers. Each value of acceleration is made out of
    * two 8bits registers, low values and high values. For that we request the 6 of them  
    * and just make then sum of each pair. For that we shift to the left the high values 
    * register (<<) and make an or (|) operation to add the low values.*/

  Acc_rawX = Wire.read() << 8 | Wire.read();  //each value needs two registres
  Acc_rawY = Wire.read() << 8 | Wire.read();
  Acc_rawZ = Wire.read() << 8 | Wire.read();

  /*///This is the part where you need to calculate the angles using Euler equations///*/

  /* - Now, to obtain the values of acceleration in "g" units we first have to divide the raw   
     * values that we have just read by 16384.0 because that is the value that the MPU6050 
     * datasheet gives us.*/
  /* - Next we have to calculate the radian to degree value by dividing 180º by the PI number
    * which is 3.141592654 and store this value in the rad_to_deg variable. In order to not have
    * to calculate this value in each loop we have done that just once before the setup void.
    */

  /* Now we can apply the Euler formula. The atan will calculate the arctangent. The
     *  pow(a,b) will elevate the a value to the b power. And finnaly sqrt function
     *  will calculate the rooth square.*/
  /*---X---*/
  Acceleration_angle[0] = atan((Acc_rawY / 16384.0) / sqrt(pow((Acc_rawX / 16384.0), 2) + pow((Acc_rawZ / 16384.0), 2))) * rad_to_deg;
  /*---Y---*/
  Acceleration_angle[1] = atan(-1 * (Acc_rawX / 16384.0) / sqrt(pow((Acc_rawY / 16384.0), 2) + pow((Acc_rawZ / 16384.0), 2))) * rad_to_deg;

  /*Now we read the Gyro data in the same way as the Acc data. The adress for the
    * gyro data starts at 0x43. We can see this adresses if we look at the register map
    * of the MPU6050. In this case we request just 4 values. W don¡t want the gyro for 
    * the Z axis (YAW).*/
/*
  Wire.beginTransmission(0x68);
  Wire.write(0x43);  //Gyro data first adress
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 4, true);  //Just 4 registers
*/
  Gyr_rawX = Wire.read() << 8 | Wire.read();  //Once again we shif and sum
  Gyr_rawY = Wire.read() << 8 | Wire.read();

  /*Now in order to obtain the gyro data in degrees/seconda we have to divide first
   the raw value by 131 because that's the value that the datasheet gives us*/

  /*---X---*/
  Gyro_angle[0] = Gyr_rawX / 131.0;
  /*---Y---*/
  Gyro_angle[1] = Gyr_rawY / 131.0;

  /*Now in order to obtain degrees we have to multiply the degree/seconds
   *value by the elapsedTime.*/
  /*Finnaly we can apply the final filter where we add the acceleration
   *part that afects the angles and ofcourse multiply by 0.98 */

  /*---X axis angle---*/
  Total_angle[0] = 0.98 * (Total_angle[0] + Gyro_angle[0] * elapsedTime) + 0.02 * Acceleration_angle[0];
  /*---Y axis angle---*/
  Total_angle[1] = 0.98 * (Total_angle[1] + Gyro_angle[1] * elapsedTime) + 0.02 * Acceleration_angle[1];

  /*Now we have our angles in degree and values from -10º0 to 100º aprox*/
  //Serial.println(Total_angle[1]);

  /*///////////////////////////P I D///////////////////////////////////*/
  /*Remember that for the balance we will use just one axis. I've choose the x angle
to implement the PID with. That means that the x axis of the IMU has to be paralel to
the balance*/

  /*First calculate the error between the desired angle and 
*the real measured angle*/
  error = Total_angle[1] - desired_angle;

  /*Next the proportional value of the PID is just a proportional constant
*multiplied by the error*/

  pid_p = kp * error;

  /*The integral part should only act if we are close to the
desired position but we want to fine tune the error. That's
why I've made a if operation for an error between -2 and 2 degree.
To integrate we just sum the previous integral value with the
error multiplied by  the integral constant. This will integrate (increase)
the value each loop till we reach the 0 point*/
  if (-3 < error < 3) {
    pid_i = pid_i + (ki * error);
  }

  /*The last part is the derivate. The derivate acts upon the speed of the error.
As we know the speed is the amount of error that produced in a certain amount of
time divided by that time. For taht we will use a variable called previous_error.
We substract that value from the actual error and divide all by the elapsed time. 
Finnaly we multiply the result by the derivate constant*/

  pid_d = kd * ((error - previous_error) / elapsedTime);

  /*The final PID values is the sum of each of this 3 parts*/
  PID = pid_p + pid_i + pid_d;

  /*We know taht the min value of PWM signal is 1000us and the max is 2000. So that
tells us that the PID value can/s oscilate more than -1000 and 1000 because when we
have a value of 2000us the maximum value that we could sybstract is 1000 and when
we have a value of 1000us for the PWM sihnal, the maximum value that we could add is 1000
to reach the maximum 2000us*/
  
  if (PID < -1000) 
  {
    PID = -1000;
  }
  if (PID > 1000) 
  {
    PID = 1000;
  }

 /* if (PID < -500) {
    PID = -500;
  }
  if (PID > 500) {
    PID = 500;
  }
*/
  /*Finnaly we calculate the PWM width. We sum the desired throttle and the PID value*/
  pwmLeft = throttle + PID;
  pwmRight = throttle - PID;

  /*Once again we map the PWM values to be sure that we won't pass the min
and max values. Yes, we've already maped the PID values. But for example, for 
throttle value of 1300, if we sum the max PID value we would have 2300us and
that will mess up the ESC.*/

  //Right
  if (pwmRight < 1000) {
    pwmRight = 1000;
  }
  if (pwmRight > 2000) {
    pwmRight = 2000;
  }
  //Left
  if (pwmLeft < 1000) {
    pwmLeft = 1000;
  }
  if (pwmLeft > 2000) {
    pwmLeft = 2000;
  }

/*int maxval = 1750;
int minval = 1250;
  //Right
  if (pwmRight < minval) {
    pwmRight = minval;
  }
  if (pwmRight > maxval) {
    pwmRight = maxval;
  }
  //Left
  if (pwmLeft < minval) {
    pwmLeft = minval;
  }
  if (pwmLeft > maxval) {
    pwmLeft = maxval;
  }*/

  //Right
  /*if (pwmRight < 1500) 
  {
    pwmRight = 1500;
  }
  if (pwmRight > 2000) 
  {
    pwmRight = 2000;
  }
  //Left
  if (pwmLeft < 1500) {
    pwmLeft = 1500;
  }
  if (pwmLeft > 2000) {
    pwmLeft = 2000;
  }*/

  /*Finnaly using the servo function we create the PWM pulses with the calculated
width for each pulse*/
  esc1.writeMicroseconds(pwmLeft);
  esc2.writeMicroseconds(pwmRight);
/*  int val1;  
  int val2;  
    val1 = map(pwmLeft, 1000, 2000, analogPulseMin, analogPulseMax);
    val2 = map(pwmRight, 1000, 2000, analogPulseMin, analogPulseMax);
    analogWrite(3, val1);
    analogWrite(5, val2);*/
  previous_error = error;  //Remember to store the previous error.
  //Serial.print("{");
//  Serial.print("Angle_in_Degree");
//  Serial.print(",");
  Serial.print(Total_angle[1]);
  Serial.print(",");
//  Serial.print("PWM_Left");
//  Serial.print(",");
  Serial.print(pwmLeft);
  Serial.print(",");
//  Serial.print("PWM_Right");
//  Serial.print(",");
  Serial.println(pwmRight);
//  Serial.print(",");
  //Serial.print("},");
///////////////////////////////////////////////////////////////////  
//  Serial.print(";");
/*  Serial.print("Angle_in_Degree:");
  Serial.print(Total_angle[1]);
  Serial.print(",");
  Serial.print("PWM_Left:");
  Serial.print(pwmLeft);
  Serial.print(",");
  Serial.print("PWM_Right:");
  Serial.println(pwmRight);*/
  //delay(10);
}  //end of loop void
