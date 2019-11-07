#include <WiFi.h>
#include <HTTPClient.h>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;
const char* ssid     = "Estrada";
const char* password = "orfa0702";

const int LED = 18;
const int BOTON = 2;


const int BOTON2 = 4;
int val;
int val2;


WiFiServer server(80);

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BOTON, INPUT);
  Serial.begin(115200);
  pinMode(5, OUTPUT);      // set the LED pin mode

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

}

int value = 0;

void loop() {
  val = digitalRead(BOTON);
  if ( val == HIGH) {
    digitalWrite(LED, HIGH);
    HTTPClient http;

    http.begin("http://192.168.43.169/get.php?texto=hayunvisitanteenelparqueadero&extension=1001");
    http.addHeader("Content-Type", "application/x-www-form-urlencode");
    //char _post[40];
    //sprintf(_post, "texto=hola%extension=1001");
    //int httpResponseCode = http.POST("extension=1001&texto=holaatodos");
    int httpCode = http.GET();

    if (httpCode > 0) {

      String response = http.getString();
      Serial.println(httpCode);
      Serial.println(response);

    }
    http.end();
  }
  else {
    digitalWrite(LED, LOW);
  }


  val2 = digitalRead(BOTON2);
  if ( val2 == HIGH) {
    //digitalWrite(LED, HIGH);
    HTTPClient http;

    http.begin("http://192.168.43.169/get.php?texto=Hayunvisitanteenlaporteriapeatonal&extension=1002");
    http.addHeader("Content-Type", "application/x-www-form-urlencode");
    //char _post[40];
    //sprintf(_post, "texto=hola%extension=1001");
    //int httpResponseCode = http.POST("extension=1001&texto=holaatodos");
    int httpCode = http.GET();

    if (httpCode > 0) {

      String response = http.getString();
      Serial.println(httpCode);
      Serial.println(response);

    }
    http.end();
  }
  else {
    digitalWrite(LED, LOW);
  }



  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      myservo.attach(15);
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);               // GET /H turns the LED on
          for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(50);                       // waits 15ms for the servo to reach the position
          }

          delay(2000); 
          for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(50);                       // waits 15ms for the servo to reach the position
          }


        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
