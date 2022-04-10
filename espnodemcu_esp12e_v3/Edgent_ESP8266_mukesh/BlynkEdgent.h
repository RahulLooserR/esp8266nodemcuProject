
extern "C" {
  #include "user_interface.h"

  void app_loop();
}

#include "Settings.h"
#include <BlynkSimpleEsp8266_SSL.h>

#ifndef BLYNK_NEW_LIBRARY
#error "Old version of Blynk library is in use. Please replace it with the new one."
#endif

#if !defined(BLYNK_TEMPLATE_ID) || !defined(BLYNK_DEVICE_NAME)
#error "Please specify your BLYNK_TEMPLATE_ID and BLYNK_DEVICE_NAME"
#endif

#include "BlynkState.h"
#include "ConfigStore.h"
#include "ResetButton.h"
#include "ConfigMode.h"
#include "Indicator.h"
#include "OTA.h"
#include "Console.h"
#include <IRremote.h>

inline
void BlynkState::set(State m) {
  if (state != m && m < MODE_MAX_VALUE) {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;

    // You can put your state handling here,
    // i.e. implement custom indication
  }
}

void printDeviceBanner()
{
  Blynk.printBanner();
  DEBUG_PRINT("--------------------------");
  DEBUG_PRINT(String("Product:  ") + BLYNK_DEVICE_NAME);
  DEBUG_PRINT(String("Firmware: ") + BLYNK_FIRMWARE_VERSION " (build " __DATE__ " " __TIME__ ")");
  if (configStore.getFlag(CONFIG_FLAG_VALID)) {
    DEBUG_PRINT(String("Token:    ...") + (configStore.cloudToken+28));
  }
  DEBUG_PRINT(String("Device:   ") + BLYNK_INFO_DEVICE + " @ " + ESP.getCpuFreqMHz() + "MHz");
  DEBUG_PRINT(String("MAC:      ") + WiFi.macAddress());
  DEBUG_PRINT(String("Flash:    ") + ESP.getFlashChipRealSize() / 1024 + "K");
  String coreVer = ESP.getCoreVersion();
  coreVer.replace("_", ".");
  DEBUG_PRINT(String("ESP core: ") + coreVer);
  DEBUG_PRINT(String("ESP SDK:  ") + ESP.getSdkVersion());
  DEBUG_PRINT(String("Boot Ver: ") + ESP.getBootVersion());
  DEBUG_PRINT(String("Boot Mode:") + ESP.getBootMode());
  DEBUG_PRINT(String("FW info:  ") + ESP.getSketchSize() + "/" + ESP.getFreeSketchSpace() + ", MD5:" + ESP.getSketchMD5());
  DEBUG_PRINT(String("Free mem: ") + ESP.getFreeHeap());
  DEBUG_PRINT("--------------------------");
}

void runBlynkWithChecks() {
  Blynk.run();
  if (BlynkState::get() == MODE_RUNNING) {
    if (!Blynk.connected()) {
      if (WiFi.status() == WL_CONNECTED) {
        BlynkState::set(MODE_CONNECTING_CLOUD);
      } else {
        BlynkState::set(MODE_CONNECTING_NET);
      }
    }
  }
}

class Edgent {

public:
  void begin()
  {
    indicator_init();
    button_init();
    config_init();
    console_init();

    printDeviceBanner();

    if (configStore.getFlag(CONFIG_FLAG_VALID)) {
      BlynkState::set(MODE_CONNECTING_NET);
    } else if (config_load_blnkopt()) {
      DEBUG_PRINT("Firmware is preprovisioned");
      BlynkState::set(MODE_CONNECTING_NET);
    } else {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  void run() {
    app_loop();
    switch (BlynkState::get()) {
    case MODE_WAIT_CONFIG:       
    case MODE_CONFIGURING:       enterConfigMode();    break;
    case MODE_CONNECTING_NET:    enterConnectNet();    break;
    case MODE_CONNECTING_CLOUD:  enterConnectCloud();  break;
    case MODE_RUNNING:           runBlynkWithChecks(); break;
    case MODE_OTA_UPGRADE:       enterOTA();           break;
    case MODE_SWITCH_TO_STA:     enterSwitchToSTA();   break;
    case MODE_RESET_CONFIG:      enterResetConfig();   break;
    default:                     enterError();         break;
    }
  }

};

Edgent BlynkEdgent;
BlynkTimer edgentTimer;

void app_loop() {
    edgentTimer.run();
    edgentConsole.run();
}


// my code and variables
#include <IRremote.h>

#define DEBUG                                    //If you comment this line, the PRINT & PRINTLN lines are defined as blank.
#ifdef DEBUG                                     //Macros are usually in all capital letters.
#define PRINT(...) Serial.print(__VA_ARGS__)     //PRINT is a macro, debug print
#define PRINTLN(...) Serial.println(__VA_ARGS__) //PRINTLN is a macro, debug print with new line
#else
#define PRINT(...)   //now defines a blank line
#define PRINTLN(...) //now defines a blank line
#endif

#define IR_RECV_PIN D5
#define N    4  // Number of devies

#define TOKEN "B8_XOpfrNpZanMRu_U16JrfHQd0PZuQk"
#define CLOUD "blynk.cloud"
#define PORT  "443"

// #define WIFI_SSID "WIFI-L00seR_Home"            //Enter Wifi Name
// #define WIFI_PASS "home@421"

#define WIFI_SSID "Monsterwifi"            //Enter Wifi Name
#define WIFI_PASS "muk983517"

#define WIFI_LED LED_BUILTIN

bool blynk_setup_state = false;

const uint8_t relayPins[N] = {D0, D1, D3, D8};
// const uint8_t relayPins[N] = {D1, D2, D7, D6};

/*                                SD3 Rx, tX */
const uint8_t switchPins[N] = {D7, 9, 3, 1};
// const uint8_t switchPins[N] = {1, D3, 13, 3};

/* remote button 0(0x11D930CF) 1 2 3 4 */
const uint remoteBtns[N] = {0x1FE50AF, 0x1FED827, 0x1FEF807, 0x1FE30CF};
const uint8_t virtualPins[N] = {V1, V2, V3, V4};
bool toggleState[N] = {1, 1, 1, 1};

IRrecv irrecv(IR_RECV_PIN);
decode_results results;
BlynkTimer timer1, timer2; 

BLYNK_WRITE_DEFAULT()
{
  uint8_t pin = request.pin;
  toggleState[pin-1] = param.asInt();
  digitalWrite(relayPins[pin-1], toggleState[pin-1]);
  // PRINTLN("pin: " + String(pin) + ", digitalPin: " + String(relayPins[pin-1]) + ", flag: " + String(toggleState[pin-1]));
}


BLYNK_CONNECTED()
{
  // Request the latest state from the server
  for (uint8_t i = 0; i < N; i++)
  {
    Blynk.syncVirtual(virtualPins[i]);
  }
}

void toggle_relay(uint8_t relay_no)
{
  toggleState[relay_no] = (toggleState[relay_no] + 1) % 2;
  digitalWrite(relayPins[relay_no], toggleState[relay_no]);
  // PRINTLN("relay no: " + String(relay_no) + ", togglestate: " + String(toggleState[relay_no]));
  Blynk.virtualWrite(virtualPins[relay_no], toggleState[relay_no]);
  delay(500);
}

void ir_remote()
{
  if (irrecv.decode(&results))
  {
    // PRINT("value: ");
    // PRINTLN(results.value, HEX);
    for (uint8_t i = 0; i < N; i++) {
        if (remoteBtns[i] == results.value) {
            toggle_relay(i);
        }
    }
    irrecv.resume();
  }
}

void blink_wifi_led()
{
  static bool led_flag = false;
  led_flag = (led_flag + 1) % 2;
  digitalWrite(WIFI_LED, led_flag);
}

void blynk_setup()
{ 
  Blynk.begin(TOKEN, CLOUD, PORT);

   for(uint8_t i = 0; i < N; i++) {
    Blynk.virtualWrite(virtualPins[i], 0);
   }
   blynk_setup_state = true;
}

void serialSetup()
{
  Serial.begin(115200);
  delay(100);
}

void pinSetup()
{
  irrecv.enableIRIn();
  pinMode(WIFI_LED, OUTPUT);

  for (uint8_t i = 0; i < N; i++)
  {
    pinMode(relayPins[i], OUTPUT);
    pinMode(switchPins[i], INPUT_PULLUP);
  }
}

void btn_handler()
{
  for(uint8_t i = 0; i < N; i++){
    if(digitalRead(switchPins[i]) == LOW) {
        toggle_relay(i);
    }
  }
}

bool wifi_setup(uint8_t timeout)
{
    uint8_t t = 0;
    uint interval_connected = 3000;
    uint interval_disconnected = 500;
    timer1.setInterval(interval_connected, blink_wifi_led);
    timer2.setInterval(interval_disconnected, blink_wifi_led);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    PRINT("\nConnecting ");
    while (WiFi.status() != WL_CONNECTED){
        if (timeout == t){
            return false;
        }
        t++;
        delay(500);
        PRINT(". ");
    }
    PRINTLN();
    return true;
}

