
/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 
 */

#include <SPI.h>
#include "src/Ethernet.h"
#include "src/M24C64.h"

M24C64 eep;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0x98, 0x01
};
IPAddress ip(192, 168, 100, 240);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

bool isI2C(uint8_t address)
{
  Wire.beginTransmission(address);
  if (Wire.endTransmission() == 0)
  {
    return true;
  }
  return false;
}

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet


  //for SPRESENSE using SPI5 & W5500_Eth board.
  digitalWrite(21, LOW);//W5500_Eth RESET# = HIGH
  delay(500);
  digitalWrite(21, HIGH);//W5500_Eth RESET# = HIGH
  Serial.println("W5500 reset done.");
  Ethernet.init(19);// use I2S_DIN pin for W5500 CS pin

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // I2C device scan
  Serial.print(F("I2C Devices(0x) : "));
  Wire.begin();
  //I2C spec. have reserved address!! these scanning escape it address.
  for (uint8_t ad = 0x08; ad < 0x77; ad++)
  {
    if (isI2C(ad))
    {
      Serial.print(ad, HEX);
      Serial.write(' ');
    }
  }
  Serial.write('\n');
  
  //EEPROM MAC ADDRESS read
  eep.init(0x57);
  Serial.println("MAC read from on board eeprom.");
  for(int i=0;i<6;i++)
  {
    mac[i] = eep.read(i);
    Serial.print(mac[i],HEX);
    Serial.print(":");
  }
  Serial.println("");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
#ifdef ARDUINO_ARCH_SPRESENSE
            int sensorReading = analogRead(A0 + analogChannel);
#else /* !ARDUINO_ARCH_SPRESENSE */
            int sensorReading = analogRead(analogChannel);
#endif /* ARDUINO_ARCH_SPRESENSE */
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("I'm SPRESENSE with W5500 Eth board.<br>");
          client.println("My MAC ADDRESS is");
          client.println( String((char)mac[0],HEX) );
          client.println(" : ");
          client.println( String((char)mac[1],HEX) );
          client.println(" : ");
          client.println( String((char)mac[2],HEX) );
          client.println(" : ");
          client.println( String((char)mac[3],HEX) );
          client.println(" : ");
          client.println( String((char)mac[4],HEX) );
          client.println(" : ");
          client.println( String((char)mac[5],HEX) );
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
