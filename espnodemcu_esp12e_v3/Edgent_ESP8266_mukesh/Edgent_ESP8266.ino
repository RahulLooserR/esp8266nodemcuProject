
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL5diP6tjd"
#define BLYNK_DEVICE_NAME "Mukesh"
#define BLYNK_AUTH_TOKEN "XrRna-kFktjn6A50N_WgMx3hAUVqwAsQ" // not required

// char *WIFI_SSID = "WIFI-L00seR_Home";          
// char *WIFI_PASS = "home@421";

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

void setup()
{
    serialSetup();
    
    if (wifi_setup(40)){
        blynk_setup();
    }
    pinSetup();
    // irrecv.enableIRIn();
    
    
    PRINTLN("Setup Successful");
}

void loop() {
  if(blynk_setup_state) {
    if (WiFi.status() == WL_CONNECTED)
	  {
		  Blynk.run();
	  }
    if(Blynk.connected()){
      timer1.run();
    } else {
      timer2.run();
    }
  }else {
    if (WiFi.status() == WL_CONNECTED) {
      blynk_setup();
    }
    timer2.run();
    PRINTLN("timer 2 running");
  }
  ir_remote();
  btn_handler();
}