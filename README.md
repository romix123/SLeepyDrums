# SleepyDrums
 ESP32 BLE drum pads, battery powered with touch wake-up

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