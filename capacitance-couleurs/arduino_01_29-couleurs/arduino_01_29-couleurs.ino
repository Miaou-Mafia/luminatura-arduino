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
}

// FUNCTION THAT WILL BE CALLED WHEN AN OSC MESSAGE IS RECEIVED:
void myOscMessageParser(MicroOscMessage& receivedOscMessage) {
  // ADD MESSAGE PARSING CODE HERE
  if (receivedOscMessage.checkOscAddress("/address")) {
    // MESSAGE ADDRESS IS "/address"
  }
  int32_t intArgument = receivedOscMessage.nextAsInt();
}

void sendColor(int r, int g, int b) {
    for (int i = 1; i <= 6; i++) {
        String channel = "/dmx/channel" + String(i);
        Serial.print("Sending OSC message: ");
        Serial.println(channel);

        myMicroOsc.sendInt((channel + "/r").c_str(), r);
        myMicroOsc.sendInt((channel + "/g").c_str(), g);
        myMicroOsc.sendInt((channel + "/b").c_str(), b);
    }
}


void loop() {
    M5.update();

    long current_millis = millis();

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

    Serial.println(cycles);

    while (millis() - current_millis < 50)
        ;

    myMicroOsc.onOscMessageReceived(myOscMessageParser);

    if (millis() - myChronoStart >= 50) {
        myChronoStart = millis();
        int myIntToSend = 100;
        myMicroOsc.sendInt("/address", myIntToSend);
    }

    if (cycles >= 400) {
        plaque01 = true;
        //plaque02 = true;
        //plaque03 = true;
    }

    if (plaque01 && plaque02 && plaque03) {
        Serial.println("All plaques activated, sending blue color");
        sendColor(0, 71, 171);  // Blue
    } else if (plaque01 && plaque02) {
        Serial.println("Plaque 1 and 2 activated, sending red color");
        sendColor(244, 112, 0);  // Red/Orange
    } else if (plaque02 && plaque03) {
        Serial.println("Plaque 2 and 3 activated, sending green color");
        sendColor(154, 237, 40);  // Green
    } else if (plaque01 && plaque03) {
        Serial.println("Plaque 1 and 3 activated, sending purple color");
        sendColor(128, 31, 219);  // Purple
    } else if (plaque01) {
        Serial.println("Plaque 1 activated, sending magenta color");
        sendColor(255, 102, 196);  // Magenta
    } else if (plaque02) {
        Serial.println("Plaque 2 activated, sending yellow color");
        sendColor(255, 222, 95);  // Yellow
    } else if (plaque03) {
        Serial.println("Plaque 3 activated, sending cyan color");
        sendColor(92, 225, 230);  // Cyan
    }
}
