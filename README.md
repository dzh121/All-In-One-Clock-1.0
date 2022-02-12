<h1>All In One Clock</h1>
<img src="Clock.jpeg" width=500 highet=500>
<p>In this project we will make an arduino clock alarm with timer and a temperature sensor.
 <br>
  <br>
 <h1>Components</h1>
 <ul>
  <li>Arduino UNO</li>
 <li>Arduino Proto Shield</li>
 <li>Real Time Clock</li>
 <li>16x2 Display</li>
 <li>Tactile Switch</li>
 <li>Buzzer</li>
 <li>Breadboard</li>
 <li>BreadBoard wires</li>
 <li>Proto Sheild Wire</li>
 <li>AM2302 DHT22</li>
 </ul>
 <h1>Wiring</h1>
 
<img src="schematics/schematics.png" width=400 highet=400/>
  
  <b>Button #1 Mode</b> - connected to digtal pin 2

  <b>Button #2 Select</b> - connected to digtal pin 4

  <b>Button #3 Plus</b> - connected to digtal pin 3

  <b>Button #4 Minus</b> - connected to digtal pin 12

  <b>Button #5 Timer on/0ff</b> - connected to digtal pin 11

You can wire all of the component to a breadboard or wire to a arduino proto shield if you would like to use the case.

</p>
<h1>Upload The Code</h1>
<p>Open your arduino ide and upload clock_alarm.ino
 
 make sure to downlod these libraries first</p>
<h1>libraries:</h1>

<b>DHT</b> - https://github.com/adafruit/DHT-sensor-library

<b>LiquidCrystal I2C</b> - https://www.arduinolibraries.info/libraries/liquid-crystal-i2-c

<b>Wire</b> - https://github.com/arduino/ArduinoCore-avr/tree/master/libraries/Wire

<b>virtuabotixRTC</b> - https://github.com/chrisfryer78/ArduinoRTClibrary

<b>RTCLib</b> - https://github.com/adafruit/RTClib
<br>
<h1> usage</h1>
<p>
 <ul>
  <li>Change between screens by pressing mode button</li>
  <li>Set time by holding mode button in screen 1</li>
  <li>Screen 1 - time date and temprature</li>
  <li>Screen 2 - set up the alarm</li>
  <li>Screen 3 - timer</li>
  <li>Screen 4 - alarm On/off</li>
</ul>  
<h1>More Info</h1>
<p>You can find all of the 3d print file in the model folder</p>
<p>contact me or more info at Arduino Project Hub:https://create.arduino.cc/projecthub/dzh121/all-in-one-clock-6ca214</p>
