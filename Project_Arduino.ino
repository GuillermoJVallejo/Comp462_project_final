#include <Stepper.h>
#include <Wire.h>

#define echo_pin 2
#define trig_pin 3

const byte slave_addr = 0x55;                               //Sets the I2C address of the Arduino
int register_addr;
byte data[2];
int rotate = 3;

const int stepsPerRevolution = 512;                         //Defines one rotation as 512 steps (90% for 28BYJ-48 stepper motor)

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);        //Sets order of steps using the pins it's connected to

void setup() {
  myStepper.setSpeed(60);                                   // in RPM
  Serial.begin(115200);
  pinMode(trig_pin, OUTPUT);                                //Initializes the ultrasonic sensor
  pinMode(echo_pin, INPUT);
  Wire.setClock(100000L);
  Wire.begin(slave_addr);                                   //Initializes the I2C connection
  Wire.onReceive(recieve_register);
  Wire.onRequest(respond_data);
}

void recieve_register(int x){                               //Reads information every time the RPi sends information
  while(Wire.available()){
    register_addr = Wire.read();
    rotate = register_addr;                                 //Sets the rotate bit as the information passed on the last bit sent from the RPi
  }
}

void respond_data(){                                        //Sends the distance measurement back to the RPi
  byte data_value = 0x00;
  if (register_addr == 0x00){
    data_value = data[register_addr];
  }
  Wire.write(data_value);
}

void ultrasonic(){                                          //Takes distance measurement, converts it to cm, and saves it as a data value to be sent
  float duration;
  float distance_cm;
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);

  distance_cm = duration / 58.0;
  data[0] = (int) distance_cm;
  Serial.println(distance_cm);
  delay(500);
}

void loop() {
  ultrasonic();                                             //Initiates the measurement function
  if(rotate == 1){                                          //Rotates left if a 1 was passed through
    myStepper.step(stepsPerRevolution);
    rotate = 3;
  }
  else if(rotate == 2){                                     //Rotates right if a 2 was passed through
    myStepper.step(-stepsPerRevolution);
    rotate = 3;
  }
}
