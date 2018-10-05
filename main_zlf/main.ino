bool  sensors[8]; /* (!) requires revision (!) */
unsigned int  sensors_int[8]; /* (!) requires revision (!) */
int sensors_black[8] = {0};

bool inver = false;

#define analogRead analogReadFast
#define digitalRead digitalReadFast
#define digitalWrite digitalWriteFast

int tur = 10;    //Подключаем левый мотор к пину 7


int js_position = 1500;  //Начальная позиция, всегда 1.5 мс для регуляторов бесколлекторных двигателей
int max_position = 2300; //Максимальное значение ШИМ 2.3 мс
int min_position = 800;  //Минимальное значени ШИМ 0.8 мс


bool t = 0;
bool printing = 0;
bool p = 0;
bool pr=0;
bool robot_work = true;
bool manualControl = false;

/* ### METHODS ###*/


void setup()
{
  //  led_setup();
  // beepSetup();

  motorsSetup();
  motorsForward();
 

  //sensorsSetup();
  //calibrate();
  // old_sensors_values();
  //calibration1();
  EEPROM_setup();
  bluetoothSetup();

}

void loop()
{
  readLine();
  algorithm3();

  //inversion();
  // led_sensors_indication();
  // beep_check();
  bluetoothMonitoring();
  //check_perek();
  //check_90();
  // beep(1);
  if (p) printing_sensors_int();
   if (pr) printing_sensors();
  if (t)
  {
    
    digitalWrite(tur, HIGH);
    delayMicroseconds(variable("turbina"));
    digitalWrite(tur, LOW);
  }
  else
  {

    digitalWrite(tur, HIGH);
    delayMicroseconds(0);
    digitalWrite(tur, LOW);
  }
  if (!robot_work)
  {
    motors(0, 0);
    Serial.println("robot stoped");
    while (!robot_work)
    {
      if (p) printing_sensors_int();
       if (pr) printing_sensors();
      readLine();
      //led_sensors_indication();
      bluetoothMonitoring();
      if (t)
      {
        digitalWrite(tur, HIGH);
        delayMicroseconds(variable("turbina"));
        digitalWrite(tur, LOW);
      }
    }
    Serial.println("robot started");
  }
  if (manualControl)
  {

    while (manualControl)
    {
      bluetoothMonitoring();
    }
  }
}
