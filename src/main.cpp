#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// =======Network settings===========
const char* ssid = "SSID";
const char* password = "PASSWORD";

IPAddress localIp(192,168,12,201);
unsigned int localPort = 8888;

WiFiUDP Udp;

byte packetBuffer[256];
char ReplyBuffer[] = "acknowledged\n";
// ==================================



// ========CAN settings==============
uint8_t sgnfcntDgt = 5;
// ==================================


// ========Function prototypes=======
void printWifiStatus(void);
// ==================================


void setup() {
  Serial.begin(115200);
  Serial.println("Setup");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  printWifiStatus();

  Udp.begin(localPort);
}

void loop() {
  typedef union {
    float val;
    byte binary[4];
  } value;
  value val1;

  byte canAddress;
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);

    if ((packetBuffer[0] == 255 && packetBuffer[1] == 255) && packetBuffer[7] == 255) {
        Serial.println("Sim Data");
        canAddress = packetBuffer[2];
        val1.binary[0] = packetBuffer[3];
        val1.binary[1] = packetBuffer[4];
        val1.binary[2] = packetBuffer[5];
        val1.binary[3] = packetBuffer[6];

        Serial.println(canAddress);
        Serial.println(val1.val);
        Serial.println(floor(val1.val * sgnfcntDgt));
    }

    if (len > 0) {
      packetBuffer[len] = 0;
    }
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

