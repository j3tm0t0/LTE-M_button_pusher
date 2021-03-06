#include <M5StickC.h>

#define BRIGHTNESS 0x0f // 0x07 (Darkest) - 0x0f (Brightest), darker = longer battery life

#define BTN_A_PIN 37
#define BTN_B_PIN 39
#define LED_PIN   10
#define GROVE_PIN 32  // for Unit RELAY https://docs.m5stack.com/#/en/unit/relay
#define GPIO_PIN 26   // for HAT I/F

#define BTN_ON  LOW
#define BTN_OFF HIGH

#define LED_ON  LOW
#define LED_OFF HIGH

#define SINGLE  (1)
#define DOUBLE  (2)
#define LONG    (3)

#define DOUBLE_THRESHOLD  (1000)
#define LONG_THRESHOLD  (2000)

int interval=0;
int intervals[]={1,5,60,60*24}; // 1 min / 5 min / 1 hour / 1 day

char *click_types[]={"","SINGLE CLICK!  ","DOUBLE CLICK!!","LONG CLICK!!!       "};

void setup() {
  M5.begin();

  while(!setCpuFrequencyMhz(10)){
    ;
  }
  M5.Axp.ScreenBreath(BRIGHTNESS);
  
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  pinMode(GROVE_PIN, OUTPUT);
  pinMode(GPIO_PIN, OUTPUT);
  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_B_PIN, INPUT_PULLUP);
  pinMode(LED_PIN,   OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);
}

void push(int duration)
{
  digitalWrite(LED_PIN, LED_ON);
  digitalWrite(GPIO_PIN, HIGH);
  digitalWrite(GROVE_PIN, HIGH);
  delay(duration);
  digitalWrite(GPIO_PIN, LOW);
  digitalWrite(GROVE_PIN, LOW);
  digitalWrite(LED_PIN, LED_OFF);
}

int check_click()
{
  unsigned long t;
  int click_type;
  if(digitalRead(BTN_A_PIN) == BTN_ON)
  {
    click_type=SINGLE;
    t=millis();    
    while(digitalRead(BTN_A_PIN) == BTN_ON)
    {
      if(millis() > t + LONG_THRESHOLD)
      {
        click_type=LONG;
        break;
      }
      delay(10);
    }
    if(click_type!=LONG)
    {
      t=millis();
      while(millis() < t + DOUBLE_THRESHOLD)
      {
        if(digitalRead(BTN_A_PIN) == BTN_ON)
        {
          click_type=DOUBLE;
          break;
        }
        delay(10);
      }
    }
    return click_type;
  }
  return 0;
}

int change_interval()
{
  if(digitalRead(BTN_B_PIN) == BTN_OFF)
  {
    return interval;
  }
  return (interval + 1) % (sizeof intervals / sizeof intervals[0]);
}


void loop(void) {
  unsigned long start=millis();
  int click_type;
  int new_interval;
  M5.Lcd.setCursor(16,16);
  M5.Lcd.print("INTERVAL: ");
  M5.Lcd.print(intervals[interval]);
  M5.Lcd.println(" min                 ");

  while(millis() < start+intervals[interval] * 60 * 1000)
  {
    M5.Lcd.setCursor(16,32);
    M5.Lcd.print("REST: ");
    M5.Lcd.print((start + intervals[interval] * 60 * 1000 - millis())/1000);
    M5.Lcd.println(" sec               ");
    click_type = check_click();
    M5.Lcd.setCursor(16,48);
    if(click_type)
    {
      M5.Lcd.println(click_types[click_type]);    
    }
    else
    {
      M5.Lcd.print("UPTIME: ");
      M5.Lcd.print(millis()/1000);
      M5.Lcd.println(" sec              ");
    }
    switch(click_type){
      case 0:
        break;
      case 1:
        push(1000);
        break;
      case 2:
        push(500);
        delay(500);
        push(500);
        break;
      case 3:
        push(2000);
        break;
    }
    if(click_type)
    {
      delay(1000);
      return;
    }
    new_interval = change_interval();
    if(interval != new_interval)
    {
      interval = new_interval;
      delay(1000);
      return; 
    }
    delay(100);
  }
  push(1000);
}
