/* my code and variables
 * utility functions to drive automation 
 */
#include <IRremote.h>
#define IR_RECV_PIN 23  
#define N    3  // Number of devies
#define FAN_PINS 3

#define WIFI_LED 2    // active high, builtin led for esp32 dev board
#define BRIGHTNESS  120    // 0-255 led brightness value
#define FAN_CONTROLLER 36   // potentiometer connected to pin 36
#define MAX_SENSOR_VALUE 1024 // potentiometer max value

void toggle_relay(uint8_t relay_no);
void sync();
void ir_remote();
void blink_wifi_led();
void blynk_setup();
void serial_setup();
void pin_setup();
void btn_handler();
void control_fan();
void change_fan_state(int fan_state);

const uint8_t relayPins[N] = {21, 22, 32}; // 33};
const uint8_t switchPins[N] = {12, 14, 27}; // 26};

/* remote button 0(0x11D930CF) 1 2 3 4 */
const uint remoteBtns[N] = {0x1FEE01F, 0x1FE10EF, 0x1FE906F}; // 0xFF10EF};
// plus = 0x1FEC03F
// minus = 0x1FE40BF

const uint8_t virtualPins[N] = {V1, V2, V3}; // V4};
volatile bool toggleState[N] = {0};
bool previousState[N] = {0};

/* controlling fan speed
 *  controll using 3 relays
 *  all relays are off for OFF condition
 *  only R1 relay is used for speed1
 *  only R1 relay is used for speed2
 *  both R1 and R2 is used for speed3
 *  only R3 is used for full speed (direct connection)
 */
enum fan_state_{
  OFF,
  SPEED1,  
  SPEED2,
  SPEED3,
  FULLSPEED,
};
const uint8_t fanVirtualPin = V5;
//const uint8_t fanPins[FAN_PINS] = {13, 33, 25};
const uint8_t fanPins[FAN_PINS] = {5, 18, 0};
volatile bool fanState[FAN_PINS] = {1, 1, 1}; 
uint8_t fan_led = 26;
int fan_state = OFF;
bool fan_state_changed = false;


IRrecv irrecv(IR_RECV_PIN);
decode_results results;
BlynkTimer timer1, timer2; 

BLYNK_WRITE_DEFAULT()
{
  uint8_t pin = request.pin;
  if(pin == fanVirtualPin) {
    change_fan_state(param.asInt());
  } else {
    toggleState[pin-1] = param.asInt();
    digitalWrite(relayPins[pin-1], toggleState[pin-1]);
  }
}


BLYNK_CONNECTED()
{
  // Request the latest state from the server
  for (uint8_t i = 0; i < N; i++)
  {
    Blynk.syncVirtual(virtualPins[i]);
  }
  Blynk.syncVirtual(fanVirtualPin);
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
  if(fan_state_changed){
    Blynk.virtualWrite(fanVirtualPin, fan_state);
  }
}

void ir_remote()
{
  if (irrecv.decode(&results))
  {
    // PRINT("value: ");
    // PRINTLN(results.value, HEX);

     Serial.print("value: ");
     Serial.println(results.value, HEX);
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

void serial_setup()
{
  Serial.begin(115200);
  delay(100);
}

void pin_setup()
{
  irrecv.enableIRIn();
  pinMode(WIFI_LED, OUTPUT);
  pinMode(fan_led, OUTPUT);
  
  for (uint8_t i = 0; i < N; i++)
  {
    pinMode(relayPins[i], OUTPUT);
    pinMode(switchPins[i], INPUT_PULLUP);
    digitalWrite(relayPins[i], HIGH);
  }
  for (uint8_t i = 0; i < FAN_PINS; i++) {
    pinMode(fanPins[i], OUTPUT);
    digitalWrite(fanPins[i], HIGH);
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

void change_fan_state(int fan_state) 
{
  Serial.println("Fan State:" + String(fan_state));
  switch (fan_state) {
    /* all relay off 
     *  active low
     */
    case OFF:
      fanState[0] = 1;
      fanState[1] = 1; 
      fanState[2] = 1;
    break;

    case SPEED1:
      fanState[0] = 0;
      fanState[1] = 1;
      fanState[2] = 1;
    break;

    case SPEED2:
      fanState[0] = 1;
      fanState[1] = 0;
      fanState[2] = 1;
    break;

    case SPEED3:
      fanState[0] = 0;
      fanState[1] = 0;
      fanState[2] = 1;
    break;

    case FULLSPEED:
      fanState[0] = 1;
      fanState[1] = 1;
      fanState[2] = 0;
    break;

    default:
    break;
  }

  digitalWrite(fanPins[0], fanState[0]);
  digitalWrite(fanPins[1], fanState[1]);
  digitalWrite(fanPins[2], fanState[2]);
  delay(500);
}

void control_fan()
{
  uint16_t start_val = 95;
  uint16_t offset = 1000;
  
  static bool sensor_val_changed = false;
  static int prev_state = OFF;
  int curr_val = analogRead(A0);
  //Serial.println(curr_val);

  if(curr_val < start_val) {
    fan_state = OFF;
  } else if(curr_val >= start_val && curr_val < (start_val + SPEED1 * offset)) {
    fan_state = SPEED1;
  } else if(curr_val >= (start_val + SPEED1 * offset) && curr_val < (start_val + SPEED2 * offset)) {
    fan_state = SPEED2;
  } else if(curr_val >= (start_val + SPEED2 * offset) && curr_val < (start_val + SPEED3 * offset)) {
    fan_state = SPEED3;
  } else {
    fan_state = FULLSPEED;
  } 

  // Serial.println("Fan State:" + String(fan_state));

  if(prev_state != fan_state) {
    change_fan_state(fan_state);
    fan_state_changed = true;
    prev_state = fan_state; 
  }
  else {
    fan_state_changed = false;
  }
  if(fanState[0] == 1 && fanState[1] == 1 && fanState[2] == 1){
    digitalWrite(fan_led, LOW);
  } else {
    digitalWrite(fan_led, HIGH);
  }
}
