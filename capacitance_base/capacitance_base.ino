#include <M5Atom.h>
//#include <CapacitiveSensor.h>

//CapacitiveSensor sensor = CapacitiveSensor(26,32);

void setup()
{
  M5.begin(false, false, false);
  Serial.begin(115200);

  // Disable the automatic re-calibration feature of the
  // capacitive sensor library
  //sensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
}

void loop()
{
  M5.update();

  long current_millis = millis();
  //long capacitance = sensor.capacitiveSensor(30);


/*
	DIRECT_WRITE_LOW(sReg, sBit);	// sendPin Register low
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to input (pullups are off)
	DIRECT_MODE_OUTPUT(rReg, rBit); // receivePin to OUTPUT
	DIRECT_WRITE_LOW(rReg, rBit);	// pin is now LOW AND OUTPUT
	delayMicroseconds(10);
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to input (pullups are off)
	DIRECT_WRITE_HIGH(sReg, sBit);	// sendPin High
    interrupts();

	while ( !DIRECT_READ(rReg, rBit) && (total < CS_Timeout_Millis) ) {  // while receive pin is LOW AND total is positive value
		total++;
	}
  */

  int cycles = 0;
  pinMode(26,OUTPUT);
  digitalWrite(26,LOW);
  pinMode(32, OUTPUT);
  digitalWrite(32,LOW);
  delayMicroseconds(10);
  pinMode(32, INPUT);
  digitalWrite(26,HIGH);
  
  while(cycles < 1000 ) {
    if ( digitalRead(32) == HIGH) break;
    cycles ++;
  }

  // Print the result of the sensor reading
  // Note that the capacitance value is an arbitrary number
  // See: https://playground.arduino.cc/Main/CapacitiveSensor/ for details
  Serial.println(cycles);

  // Wait for 50 milliseconds
  while(millis() - current_millis < 50);
}