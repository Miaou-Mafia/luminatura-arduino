#include <M5Atom.h>
//#include <CapacitiveSensor.h>

//CapacitiveSensor sensor = CapacitiveSensor(26,32);

#include <SPI.h>
#include <Ethernet.h>
#include <esp_mac.h>
EthernetUDP myUdp;

bool plaque01 = false;
bool plaque02 = false;
bool plaque03 = false;

int magenta[] = {255, 102, 196};
int yellow[] = {255, 222, 95};
int cyan[] = {92, 225, 230};
int red[] = {244, 112, 0};
int green[] = {154, 237, 40};
int purple[] = {128, 31, 219};

IPAddress myDestinationIp(192,168,1,150);
unsigned int myDestinationPort = 8001;

IPAddress myIp(192,168,1,96);
unsigned int myPort = 8000;

#include <MicroOscUdp.h>
// The number 1024 between the < > below  is the maximum number of bytes reserved for incomming messages.
// Outgoing messages are written directly to the output and do not need more reserved bytes.
MicroOscUdp<1024> myMicroOsc(&myUdp, myDestinationIp, myDestinationPort);

int myChronoStart;

void setup()
{
  M5.begin(false, false, false);
  Serial.begin(115200);

  // Disable the automatic re-calibration feature of the
  // capacitive sensor library
  //sensor.set_CS_AutocaL_Millis(0xFFFFFFFF);

  // CONFIGURE ETHERNET HARDWARE :
  // THE NUMBERS ARE THE HARDWARE PINS FOR THE ATOM POE.
  SPI.begin(22, 23, 33, 19);
  Ethernet.init(19);
  // GET FACTORY DEFINED ESP32 MAC :
  uint8_t myMac[6];
  esp_efuse_mac_get_default(myMac);
  // START ETHERNET WITH STATIC IP
  Ethernet.begin(myMac, myIp);
  myUdp.begin(myPort);

  Serial.println();
  Serial.println(__FILE__);
  Serial.print("myDestinationIp: ");
  Serial.println(myDestinationIp);
  Serial.print("myDestinationPort: ");
  Serial.println(myDestinationPort);
  Serial.print("myIp: ");
  Serial.println(Ethernet.localIP());
  Serial.print("myPort: ");
  Serial.println(myPort);

  unsigned long myChronoStart = 0;  // VARIABLE USED TO LIMIT THE SPEED OF THE SENDING OF OSC MESSAGES
}

// FUNCTION THAT WILL BE CALLED WHEN AN OSC MESSAGE IS RECEIVED:
void myOscMessageParser( MicroOscMessage& receivedOscMessage) {
  // ADD MESSAGE PARSING CODE HERE
  if ( receivedOscMessage.checkOscAddress("/address") ) {
	// MESSAGE ADDRESS IS "/address"
}
int32_t intArgument = receivedOscMessage.nextAsInt();

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

  myMicroOsc.onOscMessageReceived( myOscMessageParser );

  // EVERY 50 MILLISECONDS :
  if (millis() - myChronoStart >= 50) {  
      myChronoStart = millis();           // RESTART CHRONO

      // SEND OSC MESSAGES HERE
  }

  int myIntToSend = 100;
  myMicroOsc.sendInt("/address", myIntToSend);

  if (cycle >= 400) {
    plaque01 = true;
    magenta[];
  }


}