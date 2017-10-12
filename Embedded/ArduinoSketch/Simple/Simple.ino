
/**
 * Key connected to PH4 (Arduino Mega Pin 7) with external pull up
 * LED connected to PJ1 (Arduino Mega Pin 14
 */
const int KEY_PORT = 7;
const int LED_PORT = 14;


void setup() {
  pinMode(14, OUTPUT);
}


void loop() {
  int s = digitalRead(KEY_PORT);
  digitalWrite(LED_PORT, s);
}

