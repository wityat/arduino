/*
  examples:

  speed = 12345;
  kp = 123.45;

  speed + 10;
  speed - 20;

  speed * print;
  speed * write
  speed * read;
  all * print;
  all * write;
  all * read;

  speed * toEEPROM;
  speed * fromEEPROM;
  all * toEEPROM;
  all * fromEEPROM;
*/

#define count_data 255
char data[count_data] = {0};
int addr = 0;

void bluetoothSetup()
{
  Serial.begin(57600);
}

void bluetoothMonitoring()
{
  if (Serial.available())
  {
    char character;
    character = Serial.read();

    if (character == '\n' || character == ';')
    {
      parse();
      addr = 0;
    }
    else
    {
      data[addr] = character;
      addr ++;
      if (addr > count_data) addr = 0;
    }
  }
}

void parse()
{
  char action = '0';
  String string = "";
  String variable_name;
  float variable_value;

  for (int i = 0; i < addr; i++)
  {
    if (data[i] == ' ') continue;

    if (data[i] == '=' || data[i] == '+' || data[i] == '-' || data[i] == '*')
    {
      action = data[i];
      variable_name = string;
      string = "";
    }
    else
    {
      string += data[i];
    }
  }

  if (action == '=' || action == '+' || action == '-')
  {
    variable_value = string.toFloat();

    if (action == '+') variable_value = variable(variable_name) + variable_value;
    if (action == '-') variable_value = variable(variable_name) - variable_value;

    variable_change(variable_name, variable_value);
  }

  if (action == '*')
  {
    if (string == "toEEPROM") toEEPROM(variable_name);
    if (string == "fromEEPROM") fromEEPROM(variable_name);
    if (string == "read" || string == "print" || string == "write") variable_write(variable_name);

  }

  if (string == "stop") robot_work = false;
  if (string == "start") robot_work = true;
  if (string == "startManualControl") {manualControl = true; Serial.println("Manual Control is enabled.");}
  if (string == "stopManualControl")  {manualControl = false; Serial.println("Manual Control is disabled.");}
  if (string == "stop1") printing = false;
  if (string == "start1") printing = true;
  if (string == "stop2") p = false;
  if (string == "start2") p = true;
  if (string == "startTurbina") {t = true; Serial.println("Turbina is working.");}
  if (string == "stopTurbina") {t = false; Serial.println("Turbina is stopped.");}
  if (string == "setupTurbina") { Serial.println("Turbina is setuping."); turbinaSetup();Serial.println("Turbina is setuped.");}
  if (string == "sep")  {pr=true;}
    if (string == "sepstop")  {pr=false;}

  if (manualControl)
  {
    if (string == "Forward")       motors(255, 255);
    if (string == "Back")          motors(-255, -255);
    if (string == "LeftOneWheel")  motors(0, 255);
    if (string == "RightOneWheel") motors(255, 0);
    if (string == "LeftTwoWheels") motors(-255, 255);
    if (string == "RightTwoWheels")motors(255, -255);
    if (string == "StopMot")       motors(0, 0);
    if (string == "stop") robot_work = false;
    if (string == "start") robot_work = true;
  }
  if (string == "calibration") calibration1();
}

