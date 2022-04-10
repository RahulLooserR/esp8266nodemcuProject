/* my code and variables
 * utility functions to drive automation 
 */
#include <IRremote.h>
#define IR_RECV_PIN 23  
#define N    4  // Number of devies

#define WIFI_LED 2    // active high, builtin led for esp32 dev board
#define BRIGHTNESS  120    // 0-255 led brightness value

const uint8_t relayPins[N] = {21, 22, 32, 33};
const uint8_t switchPins[N] = {12, 14, 27, 26};

/* remote button 0(0x11D930CF) 1 2 3 4 */
const uint remoteBtns[N] = {0xFF30CF, 0xFF18E7, 0xFF7A85, 0xFF10EF};
const uint8_t virtualPins[N] = {V1, V2, V3, V4};
volatile bool toggleState[N] = {0};
bool previousState[N] = {0};

IRrecv irrecv(IR_RECV_PIN);
decode_results results;
BlynkTimer timer1, timer2; 

BLYNK_WRITE_DEFAULT()
{
  uint8_t pin = request.pin;
  toggleState[pin-1] = param.asInt();
  digitalWrite(relayPins[pin-1], toggleState[pin-1]);
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
}

void sync()
{
  for(uint8_t i = 0; i < N; i++){ 
    if(toggleState[i] != previousState[i]){
      Blynk.virtualWrite(virtualPins[i], toggleState[i]);
      delay(50);
      previousState[i] = toggleState[i];
    } 
  }
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
   uint interval_connected = 3000;
    uint interval_disconnected = 500;
    timer1.setInterval(interval_connected, blink_wifi_led);
    timer2.setInterval(interval_disconnected, blink_wifi_led);
//   blynk_setup_state = true;
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
    digitalWrite(relayPins[i], HIGH);
  }
}

void btn_handler()
{
  for(uint8_t i = 0; i < N; i++){
    if(digitalRead(switchPins[i]) == LOW) {
      toggle_relay(i);
       delay(500);
    }
  }
}
