#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

//// ###### User configuration space for AC library classes ##########

#include <ir_Coolix.h>  //  replace library based on your AC unit model, check https://github.com/crankyoldgit/IRremoteESP8266

// ESP8266 GPIO pin to use for IR blaster.
const uint16_t kIrLed = 4;
bool oldison = false;
// Library initialization, change it according to the imported library file.
IRCoolixAC ac(kIrLed);

/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/00ce1a3f-0c0d-40c6-bf26-762e7a4f9917

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  int fan;
  int mode;
  int temp;
  bool isOn;
  bool onOff;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

void setup() {
    Serial.begin(115200);
//    wifi_connect();
    initProperties();
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();
//    ArduinoOTA.onStart([]() {
//        String type;
//        if (ArduinoOTA.getCommand() == U_FLASH) {
//            type = "sketch";
//        } else { // U_FS
//            type = "filesystem";
//        }
//
//        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
//        Serial.println("Start updating " + type);
//    });
//    ArduinoOTA.onEnd([]() {
//        Serial.println("\nEnd");
//    });
//    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
//    });
//    ArduinoOTA.onError([](ota_error_t error) {
//        Serial.printf("Error[%u]: ", error);
//        if (error == OTA_AUTH_ERROR) {
//            Serial.println("Auth Failed");
//        } else if (error == OTA_BEGIN_ERROR) {
//            Serial.println("Begin Failed");
//        } else if (error == OTA_CONNECT_ERROR) {
//            Serial.println("Connect Failed");
//        } else if (error == OTA_RECEIVE_ERROR) {
//            Serial.println("Receive Failed");
//        } else if (error == OTA_END_ERROR) {
//            Serial.println("End Failed");
//        }
//    });
//    ArduinoOTA.begin();
    ac.begin();
    ac.off();
    ac.send();
}

void checkIsOn() {
    if (oldison != isOn) {
        if (isOn) {
            ac.on();
        } else {
            ac.off();
        }

        ac.send();
        oldison = isOn;
    }
}

void loop() {
//    ArduinoOTA.handle();
    ArduinoCloud.update();
    // Your code here
    temp = ac.getTemp();
    fan = ac.getFan();
    mode = ac.getMode();
    if (fan == 5) {fan = 0;}
    checkIsOn();
}


/*
  Since OnOff is READ_WRITE variable, onOnOffChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onOnOffChange()  {
    // Add your code here to act upon OnOff change
    if (onOff) {
        isOn = !isOn;
    }
}

/*
  Since IsOn is READ_WRITE variable, onIsOnChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onIsOnChange()  {
    // Add your code here to act upon IsOn change
}

/*
  Since Temp is READ_WRITE variable, onTempChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTempChange()  {
    // Add your code here to act upon Temp change
    ac.setTemp(temp);
    ac.send();
}

/*
  Since Mode is READ_WRITE variable, onModeChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onModeChange()  {
    // Add your code here to act upon Mode change
    if (mode == 2 || mode == 0) {
        temp = 24;
    }
    ac.setMode(mode);
    ac.send();
}

/*
  Since Fan is READ_WRITE variable, onFanChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onFanChange()  {
    ac.setFan(fan);
    ac.send();
}
