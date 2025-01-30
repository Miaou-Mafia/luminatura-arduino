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

int magenta[] = { 255, 102, 196 };
int yellow[] = { 255, 222, 95 };
int cyan[] = { 92, 225, 230 };
int red[] = { 244, 112, 0 };
int green[] = { 154, 237, 40 };
int purple[] = { 128, 31, 219 };
int blue[] = { 0, 71, 171 };

IPAddress myDestinationIp(192, 168, 1, 150);
unsigned int myDestinationPort = 8001;

IPAddress myIp(192, 168, 1, 96);
unsigned int myPort = 8000;

#include <MicroOscUdp.h>
// The number 1024 between the < > below  is the maximum number of bytes reserved for incomming messages.
// Outgoing messages are written directly to the output and do not need more reserved bytes.
MicroOscUdp<1024> myMicroOsc(&myUdp, myDestinationIp, myDestinationPort);

int myChronoStart;

void setup() {
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
void myOscMessageParser(MicroOscMessage& receivedOscMessage) {
  // ADD MESSAGE PARSING CODE HERE
  if (receivedOscMessage.checkOscAddress("/address")) {
    // MESSAGE ADDRESS IS "/address"
  }
  int32_t intArgument = receivedOscMessage.nextAsInt();
}

void loop() {
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
  pinMode(26, OUTPUT);
  digitalWrite(26, LOW);
  pinMode(32, OUTPUT);
  digitalWrite(32, LOW);
  delayMicroseconds(10);
  pinMode(32, INPUT);
  digitalWrite(26, HIGH);

  while (cycles < 1000) {
    if (digitalRead(32) == HIGH) break;
    cycles++;
  }

  // Print the result of the sensor reading
  // Note that the capacitance value is an arbitrary number
  // See: https://playground.arduino.cc/Main/CapacitiveSensor/ for details
  Serial.println(cycles);

  // Wait for 50 milliseconds
  while (millis() - current_millis < 50)
    ;

  myMicroOsc.onOscMessageReceived(myOscMessageParser);

  // EVERY 50 MILLISECONDS :
  if (millis() - myChronoStart >= 50) {
    myChronoStart = millis();  // RESTART CHRONO

    // SEND OSC MESSAGES HERE
  }

  int myIntToSend = 100;
  myMicroOsc.sendInt("/address", myIntToSend);

  //MAGENTA
  if (cycles >= 400) {
    plaque01 = true;
    Serial.print("Plaque 1 activée: ");
    Serial.println(plaque01);
    Serial.print("Couleur magenta: ");
    Serial.print(" R: ");
    Serial.print(magenta[0]);
    Serial.print(" G: ");
    Serial.print(magenta[1]);
    Serial.print(" B: ");
    Serial.println(magenta[2]);

    for (int i = 1; i <= 6; i++) {
      Serial.print("Sending OSC message: /dmx/channel");
      Serial.println(i);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/r", magenta[0]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/g", magenta[1]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/b", magenta[2]);
    }
  }

  //YELLOW
  if (cycles >= 400) {
    plaque02 = true;
    Serial.print("Plaque 2 activée: ");
    Serial.println(plaque02);
    Serial.print("Couleur jaune: ");
    Serial.print(" R: ");
    Serial.print(yellow[0]);
    Serial.print(" G: ");
    Serial.print(yellow[1]);
    Serial.print(" B: ");
    Serial.println(yellow[2]);

    for (int i = 1; i <= 6; i++) {
      Serial.print("Sending OSC message: /dmx/channel");
      Serial.println(i);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/r", yellow[0]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/g", yellow[1]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/b", yellow[2]);
    }
  }

  // CYAN
  if (cycles >= 400) {
    plaque03 = true;
    Serial.print("Plaque 3 activée: ");
    Serial.println(plaque03);
    Serial.print("Couleur cyan: ");
    Serial.print(" R: ");
    Serial.print(cyan[0]);
    Serial.print(" G: ");
    Serial.print(cyan[1]);
    Serial.print(" B: ");
    Serial.println(cyan[2]);

    for (int i = 1; i <= 6; i++) {
      Serial.print("Sending OSC message: /dmx/channel");
      Serial.println(i);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/r", cyan[0]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/g", cyan[1]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/b", cyan[2]);
    }
  }

  // RED/ORANGE
  if (plaque01 && plaque02) {
    Serial.print("Plaque 1 et Plaque 2 activées: ");
    Serial.println(true);
    Serial.print("Couleur rouge: ");
    Serial.print(" R: ");
    Serial.print(red[0]);
    Serial.print(" G: ");
    Serial.print(red[1]);
    Serial.print(" B: ");
    Serial.println(red[2]);

    for (int i = 1; i <= 6; i++) {
      Serial.print("Sending OSC message: /dmx/channel");
      Serial.println(i);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/r", red[0]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/g", red[1]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/b", red[2]);
    }
  }

  // GREEN
  if (plaque02 && plaque03) {
    Serial.print("Plaque 2 et Plaque 3 activées: ");
    Serial.println(true);
    Serial.print("Couleur verte: ");
    Serial.print(" R: ");
    Serial.print(green[0]);
    Serial.print(" G: ");
    Serial.print(green[1]);
    Serial.print(" B: ");
    Serial.println(green[2]);

    for (int i = 1; i <= 6; i++) {
      Serial.print("Sending OSC message: /dmx/channel");
      Serial.println(i);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/r", green[0]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/g", green[1]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/b", green[2]);
    }
  }

  // VIOLET
  if (plaque01 && plaque03) {
    Serial.print("Plaque 1 et Plaque 3 activées: ");
    Serial.println(true);
    Serial.print("Couleur violet: ");
    Serial.print(" R: ");
    Serial.print(purple[0]);
    Serial.print(" G: ");
    Serial.print(purple[1]);
    Serial.print(" B: ");
    Serial.println(purple[2]);

    for (int i = 1; i <= 6; i++) {
      Serial.print("Sending OSC message: /dmx/channel");
      Serial.println(i);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/r", purple[0]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/g", purple[1]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/b", purple[2]);
    }
  }

  // BLUE
  if (plaque01 && plaque02 && plaque03) {
    Serial.print("Plaque 1, Plaque 2 et Plaque 3 activées: ");
    Serial.println(true);
    Serial.print("Couleur bleue: ");
    Serial.print(" R: ");
    Serial.print(blue[0]);
    Serial.print(" G: ");
    Serial.print(blue[1]);
    Serial.print(" B: ");
    Serial.println(blue[2]);

    for (int i = 1; i <= 6; i++) {
      Serial.print("Sending OSC message: /dmx/channel");
      Serial.println(i);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/r", blue[0]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/g", blue[1]);
      myMicroOsc.sendInt("/dmx/channel" + String(i) + "/b", blue[2]);
    }
  }
}