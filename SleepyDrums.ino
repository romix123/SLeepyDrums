/*
Battery powered drum pad with Deep Sleep with Touch Wake Up
=====================================
This code combines 
Ryo Kosaka's drum pad work 
 https://github.com/RyoKosaka/HelloDrum-arduino-Library

with the Deep sleep examples of the ESP32

This sketch uses a LOLIN D32 light powered by a 3.7 Lipo (450 mAh)

 It has 6 drum pads
 25 - kick
 26 - crash
 27 - cymbal
 33 - snare
 34 - floortom  
 36 (vp) - tom 
 
 2 LED's
 19 - heartbeat and BLE status
 23 - pad hit

 and a touch wake up contact
 15 - touch 3 
  

displays how to use deep sleep with
a touch as a wake up source and how to store data in
RTC memory to use it over reboots

This code is under Public Domain License.

Authors:
DeepSleep
  Pranav Cherukupalli <cherukupallip@gmail.com>
Drums
  Ryo Kosaka
Integration
  Ronald Leenes <romix@macuser.nl>
*/

#include "pads.h"
#include <hellodrum.h>

#define DEBUG_DRUM true
#define Debug false


//The next only three lines are about BLE-MIDI.
//Don't forget to install the BLE-MIDI library.
//https://github.com/lathoub/Arduino-BLE-MIDI
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>
BLEMIDI_CREATE_DEFAULT_INSTANCE();

HelloDrum snare(33);
HelloDrum tom(39);
HelloDrum floortom(34);
HelloDrum kick(35);
HelloDrum crash(32);
HelloDrum cymbal(36);

//  kick   tom    cymbal
//  snare  floor   crash



#define Threshold 40 /* Greater the value, more the sensitivity */

RTC_DATA_ATTR int bootCount = 0;
touch_pad_t touchPin;

//define pins
#define HEARTBEAT 19
#define HIT 23

unsigned long inactiveMillis = 0;        // will store how longh the pads have been silent to determine bedtime 
unsigned long previousMillis = 0;        // will store last time heartbeat LED was updated
unsigned long hitMillis = 0;             // will store the time the Hit light went on

// constants won't change:
const long sleepinterval = 1200000 ;      // test set to sleep after 10 mins no pad hit
const long heartbeatinterval = 20000;    // interval at which to blink (milliseconds)
const long heartbeaton = 500;           // and off again
const long hiton = 100;                 // hit light goes off after hiton ms

int HBledState = LOW;             // ledState used to set the LED
int HiTledState = LOW;             // ledState used to set the LED

int hitTimer = 0;
int sleepTimer = 0;

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

/*
Method to print the touchpad by which ESP32
has been awaken from sleep
*/
void print_wakeup_touchpad(){
  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch(touchPin)
  {
    case 0  : Serial.println("Touch detected on GPIO 4"); break;
    case 1  : Serial.println("Touch detected on GPIO 0"); break;
    case 2  : Serial.println("Touch detected on GPIO 2"); break;
    case 3  : Serial.println("Touch detected on GPIO 15"); break;
    case 4  : Serial.println("Touch detected on GPIO 13"); break;
    case 5  : Serial.println("Touch detected on GPIO 12"); break;
    case 6  : Serial.println("Touch detected on GPIO 14"); break;
    case 7  : Serial.println("Touch detected on GPIO 27"); break;
    case 8  : Serial.println("Touch detected on GPIO 33"); break;
    case 9  : Serial.println("Touch detected on GPIO 32"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }

  
}


void callback(){
  //placeholder callback function
}

void signalsleep(){
  for (int i=0; i < 10; i++){
    digitalWrite(HEARTBEAT, HIGH);
    delay(200);
    digitalWrite(HEARTBEAT, LOW);
    delay(200);
    } 
}

void sleep(){
    //Go to sleep now
  Serial.println("Going to sleep now");
  signalsleep();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void heartbeat(){
    unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= heartbeatinterval) {
    HBledState = HIGH; 
    // save the last time you turned the LED on
    previousMillis = currentMillis;
  }
  if (currentMillis - previousMillis >= heartbeaton) {
      HBledState = LOW;
    }
    // set the LED with the HBledState of the variable:
    digitalWrite(HEARTBEAT, HBledState);
}

void hit(){
      unsigned long currentMillis = millis();
      hitMillis = currentMillis;
      digitalWrite(HIT, HIGH);
      inactiveMillis = millis(); //reset sleeptimer
  }

void wakeup(){
    for (int i=0; i < 4; i++){
    digitalWrite(HEARTBEAT, HIGH);
    delay(1000);
    digitalWrite(HEARTBEAT, LOW);
    delay(500);
    } 
}

void boot_BLE(){
    Serial.println("Starting up BLE-MIDI");
    MIDI.begin();
     snare.setCurve(SNARE[5]);
     tom.setCurve(TOM[5]);
     kick.setCurve(KICK[5]);
     crash.setCurve(CRASH[5]);
     floortom.setCurve(FLOOR[5]);
     cymbal.setCurve(CYMBAL[5]);
    delay(10000);
    Serial.println("Start jamming");
    }

void drummer(){
  while (true){
    heartbeat();
    
      unsigned long currentMillis = millis();
      if (currentMillis - inactiveMillis >= sleepinterval) sleep();
      if (currentMillis - hitMillis >= hiton) digitalWrite(HIT, LOW);
      
      //Piezo sensing is done in this line. And it is returned as a velocity of 127 stages.
      //For each piezo, one line is required.
     snare.singlePiezo(SNARE[0], SNARE[1], SNARE[2], SNARE[3]);
     tom.singlePiezo(TOM[0], TOM[1], TOM[2], TOM[3]);
     kick.singlePiezo(KICK[0], KICK[1], KICK[2], KICK[3]);
     crash.singlePiezo(CRASH[0], CRASH[1], CRASH[2], CRASH[3]);
     floortom.singlePiezo(FLOOR[0], FLOOR[1], FLOOR[2], FLOOR[3]);
     cymbal.singlePiezo(CYMBAL[0], CYMBAL[1], CYMBAL[2], CYMBAL[3]);

  //MIDI signals are transmitted with this IF statement.
  //For each piezo, one IF statement is required
  if (snare.hit == true)
  {
    MIDI.sendNoteOn(SNARE[4], snare.velocity, 10); //(note, velocity, channel)
    MIDI.sendNoteOff(SNARE[4], 0, 10);
   if (Debug){ 
    Serial.print("Snare : ");
    Serial.println(snare.velocity); 
   }
    hit();
    snare.hit = false;
  }

  if (tom.hit == true)
  {
    MIDI.sendNoteOn(TOM[4], tom.velocity, 10); //(note, velocity, channel)
    MIDI.sendNoteOff(TOM[4], 0, 10);
   if (Debug){ 
    Serial.print("Tom : ");
    Serial.println(tom.velocity); 
   }
    hit();
   }

  if (kick.hit == true)
  {
    MIDI.sendNoteOn(KICK[4], kick.velocity, 10); //(note, velocity, channel)
    MIDI.sendNoteOff(KICK[4], 0, 10);
  if (Debug){ 
    Serial.print("Kick : ");
    Serial.println(kick.velocity); 
   }    hit();
    kick.hit = false;
   }
   
  if (floortom.hit == true)
  {
    MIDI.sendNoteOn(FLOOR[4], floortom.velocity, 10); //(note, velocity, channel)
    MIDI.sendNoteOff(FLOOR[4], 0, 10);
    if (Debug){ 
    Serial.print("Floor : ");
    Serial.println(floortom.velocity); 
   }
    hit();
   }

  if (crash.hit == true)
  {
    MIDI.sendNoteOn(CRASH[4], crash.velocity, 10); //(note, velocity, channel)
    MIDI.sendNoteOff(CRASH[4], 0, 10);
    if (Debug){ 
    Serial.print("Crash : ");
    Serial.println(crash.velocity); 
   }    hit();
   }
   if (cymbal.hit == true)
  {
    MIDI.sendNoteOn(CYMBAL[4], cymbal.velocity, 10); //(note, velocity, channel)
    MIDI.sendNoteOff(CYMBAL[4], 0, 10);
    if (Debug){ 
    Serial.print("Cymbal : ");
    Serial.println(cymbal.velocity); 
   }    hit();
   }
  }
   
}

void setup(){
  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor

// setup lights
  pinMode(HEARTBEAT, OUTPUT); // heartbeat
  pinMode(HIT, OUTPUT); // hit

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32 and touchpad too
  print_wakeup_reason();
  print_wakeup_touchpad();

  //Setup interrupt on Touch Pad 3 (GPIO15)
  touchAttachInterrupt(T3, callback, Threshold);

  //Configure Touchpad as wakeup source
  esp_sleep_enable_touchpad_wakeup();

  boot_BLE(); // get the BLE interface going
  drummer(); // let's go
  
}

void loop(){
  //This will never be reached
}
