/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-led-blink
 */

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin GIOP18 as an output.
  pinMode(18, OUTPUT);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(18, HIGH); // turn the LED on
  delay(500);             // wait for 500 milliseconds
  Serial.write("hi");
  digitalWrite(18, LOW);  // turn the LED off
  delay(500);             // wait for 500 milliseconds
  Serial.write("bye");
}
