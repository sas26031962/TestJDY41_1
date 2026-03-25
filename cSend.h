/*
 Класс для передачи кадров
 */
#ifndef C_SEND_H
#define C_SEND_H

 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
 #include "config.h"

 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
 #define SEND_BUFFER_LENGTH 32
 
 //========================================================================
 //--------------------------- ОБЪЯВЛЕНИЕ КЛАССОВ -------------------------
 //========================================================================

class cSend
{

 //------------------------------------------------------------------------
 // Атрибуты
 //------------------------------------------------------------------------
 
public:
 bool IsSet = false;                 // Флаг программирования JDY-41
 bool IsCs = false;                  // Флаг выбора модуля

 byte Buffer[SEND_BUFFER_LENGTH];   // Буфер для передачи

 int FrameLength = 0;               //Длина пакета данных
 bool IsFrame = false;              //Признак события: ПАКЕТ ГОТОВ ДЛЯ ПЕРЕДАЧИ

 bool IsReset = false;              //Признак события: Передача команды Reset
 bool IsResetOk = false;            //Признак события: Передача команды Reset завершена

 bool IsConfigId = false;           //Признак события: Передача команды ConfigId
 bool IsConfigIdOk = false;         //Признак события: Передача команды ConfigId завершена
 
 bool IsConfigMode = false;         //Признак события: Передача команды ConfigMode
 bool IsConfigModeOk = false;       //Признак события: Передача команды ConfigMode завершена
 
 bool IsGetVersion = false;         //Признак события: Передача команды GetVersion
 bool IsGetVersionOk = false;       //Признак события: Передача команды GetVersion завершена
 
 bool IsSendFrame = false;          //Признак события: Передача команды SendFrame
 bool IsSendFrameOk = false;        //Признак события: Передача команды SendFrame завершена
 
 int CheckSumCalculated = 0;
 char h = '0';
 char l = '0';

//------------------------------------------------------------------------
 // Конструкторы
 //------------------------------------------------------------------------
 cSend();

 //------------------------------------------------------------------------
 // Методы
 //------------------------------------------------------------------------
 void sendTask(); 

 void prepareReset();
 void prepareConfigId();
 void prepareConfigRole();
 void prepareGetVersion();
 void prepareSendFrame(String s);
 
};//End of class cSend

 //========================================================================
 //--------------------------- РЕАЛИЗАЦИЯ МЕТОДОВ -------------------------
 //========================================================================

//
// Конструктор
//
cSend::cSend()
{
  //...  
}

//
// Реализация задачи передачи
//
void cSend::sendTask()
{
    if(!this->IsFrame) return;

    // Передача данных в канал
    SERIAL_X.write(this->Buffer, this->FrameLength);
    SERIAL_X.flush();

    IsFrame = false;
    if(IsReset)
    {
      IsReset = false;
      IsResetOk = true;
    }

    if(IsConfigId)
    {
      IsConfigId = false;
      IsConfigIdOk = true;
    }

    if(IsConfigMode)
    {
      IsConfigMode = false;
      IsConfigModeOk = true;
    }

    if(IsGetVersion)
    {
      IsGetVersion = false;
      IsGetVersionOk = true;
    }

    if(IsSendFrame)
    {
      IsSendFrame = false;
      IsSendFrameOk = true;
      //Serial.println("SendFrame complete");
    }
}

void cSend::prepareReset()
{
  Buffer[0] = 0xAB;
  Buffer[1] = 0xE3;
  Buffer[2] = 0x0D;
  Buffer[3] = 0x0A;

  FrameLength = 4;
  IsReset = true;
  IsFrame = true;
  
}//End of void cSend::prepareReset()

void cSend::prepareConfigId()
{
  Buffer[0] = 0xA9;
  Buffer[1] = 0xE1;
  Buffer[2] = 0x04;
  Buffer[3] = 0x00;
  Buffer[4] = 0x09;
  Buffer[5] = 0xA0;
  Buffer[6] = 0x66;
  Buffer[7] = 0x77;
  Buffer[8] = 0x88;
  Buffer[9] = 0x55;
  Buffer[10] = 0x01;
  Buffer[11] = 0x00;
  Buffer[12] = 0x0D;
  Buffer[13] = 0x0A;
  
  FrameLength = 14; 
  IsConfigId = true;
  IsFrame = true;
  
}//End of void cSend::prepareConfigId()


void cSend::prepareConfigRole()
{
  Buffer[0] = 0xA9;
  Buffer[1] = 0xE3;
  Buffer[2] = 0xA0;
  Buffer[3] = 0x0D;
  Buffer[4] = 0x0A;

  FrameLength = 5;
  IsConfigMode = true;
  IsFrame = true;
    
}//End of void cSend::prepareConfigRole()

void cSend::prepareGetVersion()
{
  Buffer[0] = 0xAB;
  Buffer[1] = 0xE4;
  Buffer[2] = 0x0D;
  Buffer[3] = 0x0A;

  FrameLength = 4;
  IsGetVersion = true;
  IsFrame = true;
    
}//End of void cSend::prepareGetVersion()

void cSend::prepareSendFrame(String s)
{
  FrameLength = s.length();
  if(FrameLength > SEND_BUFFER_LENGTH)
  {
    Serial.println("Frame too long, nothing to do!!!");
  }
  else
  {
    for(int i = 0; i < FrameLength; i++)
    {
      Buffer[i] = s[i];
    }

    IsSendFrame = true;
    IsFrame = true;
    Serial.println("PrepareSendFrame:" + s);
  }  
}

//=========================================================================

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
#endif
