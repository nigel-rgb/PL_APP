//Nigel RTOS test 2 ------------------------------- blinkywith user input speed --------------------------------------


#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// #include 


#define LED_BUILTIN 2

static int blink_speed = 100;


void blink_led(void *parametes) {

  // int *set_blink_s;
  // set_blink_s = (int *) pointer_blink_speed;

  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay((uint) blink_speed / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay((uint) blink_speed / portTICK_PERIOD_MS);
  }
}

void read_serial_speed(void *parameter) {

  while (1){
    if (Serial.available() > 0){
        blink_speed = Serial.parseInt() ; 
        Serial.print("new blink speed =");
        Serial.println(blink_speed);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

}


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(1000);
  vTaskDelay(1500 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
    blink_led,
    "blink led",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);

  xTaskCreatePinnedToCore(
    read_serial_speed,
    "read serial speed",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);

    vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:


}