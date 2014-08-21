#pragma config(Sensor, S1,     PROTO_PORT,          sensorI2CCustom9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
    Name: RBC_Exp4.c
    Author: Xander Soldaat
    Date: 28/01/2009
    Version: 0.1
    Contact: mightor_at_gmail.com

(c) Xander Soldaat & HiTechnic , 2009
*/

int inputdata;
byte outputdata;
byte cmdbuff[4];
byte respbuff[2];

/*
  42,43 - A0 input
  44,45 - A1 input
  46,47 - A2 input
  48,49 - A3 input
  4A,4B - A4 input
  4C    - B inputs
  4D    - B ouputs
  4E    - B controls
*/

// Convert an unsigned byte to an int.  RobotC doesn't currently
// support unsigned variables. This function is a work-around.
int ubyteToInt(byte _byte) {
	return (_byte & 0x80) ? (_byte & 0x7F) + 0x80 : _byte;
}

void setup() {
  cmdbuff[0] = 1;                           // Number of bytes in I2C command
  cmdbuff[1] = 0x02;                        // I2C address of protoboard

  for(int i = 0; i < 5; i++) {
    sendI2CMsg(PROTO_PORT, &cmdbuff[0], 0);  // Send command to protoboard
    wait1Msec(5);
  }
}

void readdata() {
  cmdbuff[0] = 2;                           // Number of bytes in I2C command
  cmdbuff[1] = 0x02;                        // I2C address of protoboard
  cmdbuff[2] = 0x42;                        // Set read address to A0

  sendI2CMsg(PROTO_PORT, &cmdbuff[0], 2);    // Send command to protoboard
  wait1Msec(10);                            // Wait for response
  readI2CReply(PROTO_PORT, &respbuff[0], 2); // Read the response

  inputdata = ubyteToInt(respbuff[0]) * 4 + ubyteToInt(respbuff[1]);
}

void writedata() {

  cmdbuff[0] = 3;                           // Number of bytes in I2C command
  cmdbuff[1] = 0x02;                        // I2C address of protoboard
  cmdbuff[2] = 0x4D;                        // Set write address to B outputs
  cmdbuff[3] = outputdata;                  // The data to be written

  sendI2CMsg(PROTO_PORT, &cmdbuff[0], 0);    // Send command to protoboard
  wait1Msec(5);
}

task main() {
  setup();

  cmdbuff[0] = 3;                           // Number of bytes in I2C command
  cmdbuff[1] = 0x02;                        // I2C address of protoboard
  cmdbuff[2] = 0x4E;                        // Set write address to B outputs
  cmdbuff[3] = 0x01;                        // The data to be written
                                            // 0x01 = 000001 binary

  sendI2CMsg(PROTO_PORT, &cmdbuff[0], 0);    // Send command to protoboard
  wait1Msec(5);

  while(true) {
    readdata();
    eraseDisplay();
    nxtDisplayTextLine(1, "%d", inputdata);
    wait1Msec(50);
  }
}
