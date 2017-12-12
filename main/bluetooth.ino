void bluetoothSetup()
{
	Serial.begin(9600);
}

void bluetoothMonitoring()
{

	if (Serial.available()) {                                           // если пришли данные по Bluetooth
    ResetDTR[0] = Serial.read();                                      // записываем принятые данные, в первый элемент массива
    if (ResetDTR[0] == 32 && ResetDTR[1] == 48) pinMode(12, OUTPUT);  // Перед началом загрузки sketch, еще до RESET,  поступает (DTR сигнал) (48 и 32) три раза. ASCII Code (48 = Zero / 0) - (32 = Space / пробел). проверяем если пришёл (DTR сигнал), делаем RESET. ( на Pro Mini, PIN-A3 соединён через Резистор 220 Ом с PIN-RESET)
    ResetDTR[1] = ResetDTR[0];
    ezda();// записываем значение из первого элемент массива - во второй элемент массива
  }
	/*
  if(Serial.available())
  {
    char character;
    character = Serial.read();
    if(character == '\n') Serial.print("LINE! ok");
  }
  */
}
