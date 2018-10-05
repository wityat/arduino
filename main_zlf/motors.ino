const byte AIN1 = 4; /* (!) requires revision (!) */
const byte AIN2 = 5; /* (!) requires revision (!) */
const byte PWMA = 3; /* (!) requires revision (!) */

const byte BIN1 = 8; /* (!) requires revision (!) */
const byte BIN2 = 7; /* (!) requires revision (!) */
const byte PWMB = 9; /* (!) requires revision (!) */

const byte STBY = 6; /* (!) requires revision (!) */



/* ### METHODS ###*/

void turbinaSetup()
{
  pinMode(tur,OUTPUT);
  Serial.println("Turbina is setuping...");
  for (int i = 0; i < 1000; i++)
  {
    digitalWrite(tur, HIGH);
    delayMicroseconds(2000);
    digitalWrite(tur, LOW);
    delay(1);
  }
  for (int i = 0; i < 1000; i++)
  {
    digitalWrite(tur, HIGH);
    delayMicroseconds(800);
    digitalWrite(tur, LOW);
    delay(1);
  }
  Serial.println("Turbina is setuped.");
}

void motorsSetup()
{
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

   pinMode(STBY, OUTPUT);

  motorsON();
}

void  motorsON()
{
  digitalWrite(STBY, 1);
}

void  motorsOFF()
{
  digitalWrite(STBY, 0);
}

void motorLeftForward()
{
  digitalWrite(AIN1, 1);
  digitalWrite(AIN2, 0);

  analogWrite(PWMA, 0);
}

void motorLeftBackward()
{
  digitalWrite(AIN1, 0);
  digitalWrite(AIN2, 1);

  //analogWrite(PWMA, 0);
}

void motorRightForward()
{
  digitalWrite(BIN1, 1);
  digitalWrite(BIN2, 0);

  analogWrite(PWMB, 0);
}

void motorRightBackward()
{
  digitalWrite(BIN1, 0);
  digitalWrite(BIN2, 1);

  //analogWrite(PWMB, 0);
}

void motorsForward()
{
  motorLeftForward();
  motorRightForward();
}

void motorsBackward()
{
  motorLeftBackward();
  motorRightBackward();
}

//function for control motors and direction
void motors(int left_speed, int  right_speed)
{
  if (left_speed > 0) motorLeftForward();
  if (left_speed < 0) {
    motorLeftBackward(); left_speed = -(variable("backspeed"));
  }

  if (right_speed > 0) motorRightForward();
  if (right_speed < 0) {
    motorRightBackward(); right_speed = - (variable("backspeed"));
  }


  if (left_speed > variable("maxspeed")) left_speed = variable("maxspeed");
  if (right_speed > variable("maxspeed")) right_speed = variable("maxspeed");

  motorsOnlySpeed(abs(left_speed), abs(right_speed));
}

//function for  control only speed of  motors, not  direction
void motorsOnlySpeed(int left_speed, int  right_speed)
{
  analogWrite(PWMA, left_speed);
  analogWrite(PWMB, right_speed);
  if (printing)
  {
    Serial.print(left_speed);
    Serial.print(" ");
    Serial.println(right_speed);
  }
}

///....
void right()
{
  motors(150, -150);
  delay(150);
  while (1)
  {
    //motors(150, -150);
    readLine();
    if (sensors[4] == 1 || sensors [8] == 1)
    {
      break;
    }
  }
  motors(150, 150);
}
void left()
{
  motors(-150, 150);
  delay(150);
  while (1)
  {
    //motors(-150, 150);
    readLine();
    if (sensors[4] == 1 || sensors[1] == 1)
    {
      break;
    }
  }
  motors(150, 150);
}

