#include <SPI.h>
#include <Ethernet.h>
#include <esp_mac.h>
#include <MicroOscUdp.h>

EthernetUDP myUdp;

IPAddress myDestinationIp(192, 168, 1, 140); // QLC+ IP
unsigned int myDestinationPort = 8001;      // Port to send OSC to

IPAddress myIp(192, 168, 1, 201);  // ESP32 Static IP
unsigned int myPort = 8000;        // Listening port (not used in your case, just for sending)

MicroOscUdp<1024> myMicroOsc(&myUdp, myDestinationIp, myDestinationPort);

int myChronoStart;

void setup() {
  Serial.begin(115200);  // Start serial communication for debugging

  SPI.begin(22, 23, 33, 19);  // Initialize SPI for Ethernet
  Ethernet.init(19);  // Initialize Ethernet using the provided pin (Ethernet CS pin)
  
  uint8_t myMac[6];
  esp_efuse_mac_get_default(myMac);  // Get ESP32 MAC address

  // Start Ethernet with static IP
  Ethernet.begin(myMac, myIp);
  myUdp.begin(myPort);  // Start UDP communication

  Serial.println();
  Serial.println(__FILE__);
  Serial.print("myDestinationIp: ");
  Serial.println(myDestinationIp);
  Serial.print("myIp: ");
  Serial.println(Ethernet.localIP());
  Serial.print("myPort: ");
  Serial.println(myPort);
}

void loop() {
  long current_millis = millis();

  int cycles = 0;

  // Pin configuration for reading capacitance
  pinMode(26, OUTPUT);
  digitalWrite(26, LOW);
  pinMode(32, OUTPUT);
  digitalWrite(32, LOW);
  delayMicroseconds(10);
  
  pinMode(32, INPUT);  
  digitalWrite(26, HIGH);

  // Read capacitance sensor (basic approach)
  while (cycles < 1000) {
    if (digitalRead(32) == HIGH) break;
    cycles++;
  }

  // Send OSC message with capacitance reading
  if (millis() - myChronoStart >= 50) {
    myChronoStart = millis();
    myMicroOsc.sendInt("/cap", cycles);  // Send the capacitance value to QLC+
  }

  delay(50);  // Delay to avoid flooding the network with messages
}
