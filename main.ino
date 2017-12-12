byte  sensors[8]; /* (!) requires revision (!) */
String string_in; //string_in.clear();



/* ### METHODS ###*/

void setup()
{
  motorsSetup();
  motorsON();
  motors(0, 0);
  motorsForward();

  sensorsSetup();
  EEPROM_setup();
  bluetoothSetup();


  //test
  Serial.println(variable("kp"));
  variable_change("kp", 17);
  Serial.println(variable("kp"));
  toEEPROM("kp");
  
}

void loop()
{

  
  readLine();
  algorithm2();
  
 // unsigned long long int last_time = millis();  
  bluetoothMonitoring();
 // int res = millis()-last_time;
  // Serial.println(res);
  // Serial.println(string_in.length());

  // delay(1000);
}



