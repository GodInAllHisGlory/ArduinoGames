#include <cstdlib>
#include "WiFiS3.h"
#include "Arduino_LED_Matrix.h"
#include "arduino_secrets.h" 
#include "pages.h"
#include "matrix.h"

const int PORT = 80;
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;             // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(PORT);

ArduinoLEDMatrix matrix;
const uint32_t blankFrame[] = {0x0,0x0,0x0};

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void response(WiFiClient client, String uri){
  const char* page;
  if (uri == "/") {
    page = indexHtml;
  } else {
    page = indexHtml;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.print(page);
  client.println();
}

void setup() {
  Serial.begin(9600);      // initialize serial communication
  matrix.begin();

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(5000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String request = "";                // make a String to hold incoming data from the client
    char c = client.read();
    while(c != '\n'){    // Only the verb and uri are needed which are found int the first line
      request += c;
      c = client.read(); 
    }  
    Serial.println(request);
    while (client.connected()) {
      if (client.available()) {
        const String uri = request.substring(request.indexOf("/"), request.lastIndexOf(" "));
        if (request.startsWith("GET")){
          response(client, uri);
          matrix.loadFrame(blankFrame);
        } else if(request.startsWith("POST")){
          matrix.loadFrame(matrixParser(uri).data());
          server.println("{success: true}");
        }
        }
        break;
      }
    client.stop();
    Serial.println("client disconnected");
    }
}