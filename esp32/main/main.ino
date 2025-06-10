#define RED 22
#define GREEN 17
#define BLUE 5

int red_led_value = 0;
int green_led_value = 0;
int blue_led_value = 0;
char led_increasing = 1;

hw_timer_t *led_fade_timer = NULL;

void IRAM_ATTR onFadeTimer() {
  if(led_increasing == 1) {
    blue_led_value += 4;
  } else {
    blue_led_value -= 4;
  }
  if(blue_led_value == 252) led_increasing = 0;
  if(blue_led_value == 0) led_increasing = 1;
  analogWrite(BLUE, blue_led_value);
}

void setup() {
  pinMode(BLUE, OUTPUT);
  digitalWrite(BLUE, LOW);

  led_fade_timer = timerBegin(1000000);
  timerAttachInterrupt(led_fade_timer, &onFadeTimer);
  timerAlarm(led_fade_timer, 10000, true, 0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
