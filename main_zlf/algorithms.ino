double e_last = 0;
unsigned long long int time_last = 0;

#include <QTRSensors.h>
#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading


// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
//QTRSensorsAnalog qtra((unsigned char[]) {A0,A1,A2,A3,A4,A5,A6,A7}, 
//  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
  QTRSensorsAnalog qtra((unsigned char[]) {A7,A6,A5,A4,A3,A2,A1,A0}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
void algorithm1()
{

  int left, right;

  for (int i = 0; i < 4; i++) left += sensors_int[i];
  for (int i = 4; i < 8; i++) right += sensors_int[i];

  //with inversiya
  int left_speed = map(left, 0, 1024 * 4, 0, 255) * 5 / 2;
  int right_speed = map(right, 0, 1024 * 4, 0, 255) * 5 / 2;

  motorsOnlySpeed(left_speed, right_speed);
}

/*working variables*/
unsigned long lastTime;                                                     //последнее время
double errSum, lastInput, lastErr, error;                                              //сумма ошибок, последняя ошибка, ошибка
double sensors_weight[8] = { -4, -3, -2, -1, 1, 2, 3, 4};                   //вес сенсоров
float TimeMy = 0.01;
void algorithm2()
{
  double sum_weight_sensors, Output, count_sensors_on_black;                  //входные значения, выходные значения, заданное значение
  for (int i = 0; i < 8; i++)
  {
    sum_weight_sensors += sensors_weight[i] * sensors[i];                    // сумма весов сенсоров
    count_sensors_on_black += sensors[i];                                    //количество сенсоров на черном
  }
  if (count_sensors_on_black == 0) error = lastErr;
  else error = sum_weight_sensors / (4 * count_sensors_on_black);            //формула, которая работает
  /*Как давно мы рассчитывали*/
  unsigned long now = millis();                                              //время сейчас
  double timeChange = (double)(now - lastTime);                              //настоящее - прошлое, промежуток времени
  if (timeChange > TimeMy)
  {
  /*Вычисляем все переменные рабочей ошибки*/
  errSum += (error * TimeMy);                                            // сумма ошбок = ошибка * (настоящее - прошлое)
  double dErr = (error - lastErr) / TimeMy;                              //ошибка - прошлая ошибка /  (настоящее - прошлое)
  /*Вычисляем выходной сигнал PID*/
  Output = variable ("kp") * error + variable ("ki") * errSum + variable ("kd") * dErr;

  /**/
  float speed_left = variable("speed") - Output;
  float speed_right = variable("speed") + Output ;

  //if (speed_left < 0) speed_left = 0;
  //if (speed_right < 0) speed_right = 0;                                        
  /*
    Serial.print(speed_left);
    Serial.print(" ");
    Serial.print(speed_right);
    Serial.print(" ");
    Serial.println(error);
  */
  motors(speed_left, speed_right);

  /*Запоминаем некоторые переменные для следующего раза*/
  lastErr = error;
  lastTime = now;
  }
}
int lastError = 0;
void algorithm3()
{
  unsigned int position = qtra.readLine(sensors_int);

  int error = position - 3500;
  int motorSpeed = variable ("KP")  * error + variable ("KD")  * (error - lastError);
  lastError = error;

  int rightMotorSpeed = variable ("speed") + motorSpeed;
  int leftMotorSpeed = variable ("speed") - motorSpeed;
  motors (leftMotorSpeed, rightMotorSpeed); 
}


int count_turns = 0; bool turn_now = 0;
long unsigned int l_time = 0;
void check_perek ()
{
  int count_black = 0;
  for (int i = 0; i < 8; i++)
    count_black += sensors[i];
  if (turn_now == 0 && count_black >= 6)
  {
    count_turns++;
    turn_now = 1;
    //right();
    //beep(1);
  }
  else if (count_black < 3)
  {
    turn_now = 0;
  }
  
  Serial.println(count_turns);
  
  /*
  switch (count_turns)
  {
    case 1:
      right();
      //  inver = true;
      count_turns++;
      break;
    case 2:

      break;
    case 3:

      break;
    case 4:
      inver = false;
      break;
    case 5:
      motorsOFF();
      break;
  }
*/
}
bool check_90()
{
  int count_black_left = 0, count_black_right = 0;
  for (int i = 0; i < 5; i++)
    count_black_left += sensors[i];
  if (count_black_left == 5)
  {
    //beep(1);
    //left(); 
    right();
    
    return 1;
  }

  for (int i = 3; i < 8; i++)
    count_black_right += sensors[i];
  if (count_black_right == 5)
  {
    //beep(2);
   // right();
     left();
    return 1;
  }
}


