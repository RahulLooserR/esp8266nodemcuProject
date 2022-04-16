
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLN15-s6EJ"
#define BLYNK_DEVICE_NAME "MayankHA"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7

#include "BlynkEdgent.h"
#include "soc/soc.h"                // to avoid brownout
#include "soc/rtc_cntl_reg.h"       // to avoid brownout
#include <pthread.h>

pthread_t manual_control_thread;
TaskHandle_t Task1;
bool isConnected = false;

void manul_control_function(void *param)
{  
//  Serial.println("inside thread");
//  Serial.print("thread() running on core ");
//  Serial.println(xPortGetCoreID());
  while (true){
    if(isConnected){
      timer1.run();
    } else {
      timer2.run();
    }
    btn_handler();
    ir_remote();
    delay(1);  
  }
}

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); //disable brownout detector
  delay(5000);
  setCpuFrequencyMhz(120);
  delay(500);
  
  Serial.begin(115200);
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println(BOARD_BUTTON_PIN);
  delay(2000);
  pinSetup();
  delay(100);
  BlynkEdgent.begin();
  blynk_setup();
//  Serial.println(BlynkState::get());
//  Serial.println("setup completed");
//  Serial.print("Current state: ");
//  Serial.println(BlynkState::get());
//  pthread_create(&manual_control_thread, NULL, manul_control_function, (void *)&toggleState);
  xTaskCreatePinnedToCore(
                    manul_control_function,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);     
}

void loop() {
  if(Blynk.connected()){
      isConnected = true;
      sync();
  } else {
    isConnected = false;
  }
  BlynkEdgent.run();
}
