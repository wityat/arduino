const byte count_sensors = 8; /* (!) requires revision (!) */

//const byte sensors_pins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};  /* (!) requires revision (!) */
const  byte sensors_pins[8] = {A7, A6, A5, A4, A3, A2, A1, A0};




/* ### METHODS ###*/

void sensorsSetup()
{
  for (int i; i < count_sensors; i++)
    pinMode(sensors[i], INPUT);
}

void  readLine()
{
  for (int i = 0; i < count_sensors;  i++)
  {
    sensors_int[i] = analogRead(sensors_pins[i]);
    sensors[i] = (sensors_int[i] >= sensors_black[i]);
  }
}

void calibration1()
{
    pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  
  for (int i = 0; i < 200; i++)  // make the calibration take about 10 seconds
  {
    if(i<100)motors(-100,100);
    else motors(100,-100);
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  motors(0,0);
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
}

void printing_sensors_int()
{
  for (int i = 0; i < count_sensors;  i++)
  {
    Serial.print(sensors_int[i]);
    Serial.print(" ");
  }
  Serial.print(" ");
   int error = qtra.readLine(sensors_int) - 3500;
   Serial.print(error);
   Serial.println();
}
void printing_sensors()
{
  for (int i = 0; i < count_sensors;  i++)
  {
    Serial.print(sensors[i]);
    Serial.print(" ");
  }
  Serial.println();
}
void calibrate()
{
  byte min_value[8];
  byte max_value[8];

  readLine();
  for (int i = 0; i < count_sensors; i++)
  {
    min_value[i] = sensors_int[i];
    max_value[i] = sensors_int[i];
  }

  long unsigned int last_time = millis();

  while (millis() - last_time < 5000)
  {
    readLine();
    for (int i = 0; i < count_sensors; i++)
    {
      if (min_value[i] > sensors_int[i]) min_value[i] = sensors_int[i];
      if (max_value[i] < sensors_int[i]) max_value[i] = sensors_int[i];
    }
  }

  for (int i = 0; i < count_sensors; i++)
    sensors_black[i] = (max_value[i] + min_value[i]) / 2;
  for (int i = 0; i < count_sensors; i++)
  {
    int adrr = 512;
    adrr  += 2;
    EEPROM_writeInt(adrr, sensors_black[i]);
  }
}
void old_sensors_values()
{
  for (int i = 0; i < count_sensors; i++)
  {
    int adrr = 512;
    adrr  += 2;
    sensors_black[i] = EEPROM_readInt(adrr);
  }
}

void calibrate_after()
{
  int min_value = sensors_int[0];
  int max_value = sensors_int[0];

  for (int i = 0; i < count_sensors; i++)
  {
    if (sensors_int[i] < min_value) min_value = sensors_int[i];
    if (sensors_int[i] > max_value) max_value = sensors_int[i];
  }

  for (int i = 0; i < count_sensors; i++)
    sensors_black[i] = (max_value + min_value) / 2;
}

void inversion()
{
  int count_black_zones = 0;

  bool color_now = 0; // 0 - white // 1 - black

  for (int i = 0; i < count_sensors; i++)
  {
    if (sensors[i] != color_now)
    {
      color_now = !color_now;
      if (color_now == 1) count_black_zones += 1;
    }
  }
  // unsigned long int timel = 0;
  //time1 = millis();
  if (count_black_zones > 1) for (int i = 0; i < count_sensors; i++) sensors[i] = !sensors[i];
}



int countSensors()
{
  return count_sensors;
}

void chek_sensors()
{
  for (int i = 0; i < 8; i++)
  {
    Serial.print(sensors_int[i]);
    Serial.print("_");
    Serial.print(sensors_black[i]);
    Serial.print("      ");
  }
  Serial.println(" ");

}

