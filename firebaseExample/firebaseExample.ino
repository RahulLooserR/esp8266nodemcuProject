#include <ESP8266WiFi.h>

#include <FirebaseArduino.h>

#define WIFI_SSID "Ghar"
#define WIFI_PASSWORD "nahimalum"
#define FIREBASE_HOST "https://automationtest-689f5.firebaseio.com/"
#define FIREBASE_AUTH "OonBM0O4qyQp6Fday9J1W5dWbCY1uDDMgSHkTw4i"

int LED1 = 15;
String n;
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//  FirebaseObject obj = Firebase.get("/");
void setup()
{
Serial.begin(115200);

pinMode(LED1, OUTPUT);

  delay(2000);
  Serial.println('\n');
 
  wifiConnect();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  delay(10);
}

void loop()
{  
Firebase.pushString("/LED2", "on");
if(Firebase.failed()){
  Serial.print(Firebase.error());
}

Serial.print(Firebase.getString("/LED1") + "\n");

analogWrite(LED1, Firebase.getString("LED1").toInt());
delay(10);
//  n = obj.getString("LED1");

if(WiFi.status() != WL_CONNECTED)
{
  wifiConnect();
}
delay(10);

}

void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED)
  {                                       // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++teller); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
