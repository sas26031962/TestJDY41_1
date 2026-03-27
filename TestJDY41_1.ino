/**************************************************************************************************
 * Тестовая программа для работы с модулем JDY-41
 * Подключение модуля cIncoming
 * Подключение модуля cSend
 * Доступные команды:
 * #Cs:0
 * #Cs:1
 * #Set:0
 * #Set:1
 * #Reset
 * #ConfigId
 * #ConfigMode
 * 
 **************************************************************************************************/
//#include "cSend.h"
#include "cIncoming.h"

cSend SendInstance;                 //Составляющая контроля JDY-41
cIncoming Incoming;                 //Коммуникационная составляющая

void setup() 
{
  pinMode(SET, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  
  pinMode(LED_8, OUTPUT);

  Serial.begin(115200);
  SERIAL_X.begin(9600);

  Serial.println(TITLE);

  //Начальная установка выводов управления
  digitalWrite(SET, HIGH);
  digitalWrite(LED_1, LOW);

  digitalWrite(CS, HIGH);
  digitalWrite(LED_2, LOW);

  delay(2000);
  
  digitalWrite(CS, LOW);
  digitalWrite(LED_2, HIGH);

}//End of void setup()

void loop() 
{
  //---
  Incoming.operate();
  //---

  SendInstance.sendTask();
  //---

  if(SendInstance.IsResetOk)
  {
    Serial.println("Send RESET command: AB E3 0D 0A");
    SendInstance.IsResetOk = false;
  }
  
  if(SendInstance.IsConfigIdOk)
  {
    Serial.println("Send CONFIG_ID command: A9 E1 04 00 09 A0 66 77 88 55 01 00 0D 0A");
    SendInstance.IsConfigIdOk = false;
  }
  
  if(SendInstance.IsConfigModeOk)
  {
    Serial.println("Send CONFIG_MODE command: A9 E3 A0 0D 0A");
    SendInstance.IsConfigModeOk = false;
  }

  if(SendInstance.IsGetVersionOk)
  {
    Serial.println("Send GET_VERSION command: AB E4 0D 0A");
    SendInstance.IsGetVersionOk = false;
  }

  if(SendInstance.IsSendFrameOk)
  {
    Serial.println("SEND_FRAME: " + Incoming.getParameter() + " complete");
    SendInstance.IsSendFrameOk = false;
  }

  if(SERIAL_X.available() > 0)
  {
    char X = SERIAL_X.read();
    Incoming.sJdyAnswer += String(X);
  }

  //Индикация наличия символов в буфере
  digitalWrite(LED_8, !(Incoming.sJdyAnswer.length() > 0));

}//End of void loop()
