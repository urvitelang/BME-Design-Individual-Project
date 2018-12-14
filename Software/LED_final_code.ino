#define FLASH_RATE_HZ 2
#define PWM_MAX 255
#define PMW_50 127
#define PMW_10 25
const byte ledPin = 9;
const byte interruptpin = 3;
volatile byte Button_push = LOW;
int Buttonstate;
int Operating_mode;
unsigned long lasttime = 0;
unsigned int timeDebounce = 0;
// operating mode: 5 - off, 4 - bright, 3 - mid-bright, 2 - dim, 1 - flash 
//(since decremented When working on LED operating mode appears to be 1:off,2:bright,3:mid-bright,4:dim,5:flash
void setup() {
  pinMode(ledPin, OUTPUT );                                                  //defining Output pin
  pinMode(interruptpin, INPUT);                                              // defining input pin
  Operating_mode = 6;                                                        //setting operating mode to 6 for decrementing to switch between modes
  attachInterrupt(digitalPinToInterrupt(interruptpin), buttonpress, RISING); //attaching interrupt
  Serial.begin(9600);
}

void loop()
{
  check_for_button_press();
  if (Operating_mode <= 0)
  {
    Operating_mode = 5;                                                       
  }
  set_pwm_based_on_operating_mode();
}
void buttonpress()
{
  Button_push = HIGH;
}

void check_for_button_press()
{
  if (Button_push == HIGH)
  { if (millis() - lasttime > timeDebounce)
    { Operating_mode--;
      lasttime = millis();
      Button_push = LOW;
    }
  }
}
void set_pwm_based_on_operating_mode()
{
  switch (Operating_mode)
  {
    case 5:
      digitalWrite(ledPin, LOW);//off
      break;
    case 4:
      analogWrite(ledPin, PWM_MAX);//maxPMW-max brightness
      break;
    case 3:
      analogWrite(ledPin, PMW_50 ); //50% duty cycle-medium bright
      break;
    case 2:
      analogWrite(ledPin, PMW_10); //10% duty cycle-Dim
      break;
    case 1:
      flash_the_light();
      break;

  }
  delay(500);
}
void flash_the_light()
{ float periodON = (1 / FLASH_RATE_HZ) * 1000;
  digitalWrite(ledPin, HIGH);
  delay(periodON);
  digitalWrite(ledPin, LOW);
  delay(periodON);

}
