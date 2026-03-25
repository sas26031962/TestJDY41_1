/*******************************************************************
 * Класс управления коммуникацией по последовательному порту
 * Строка с командой начинается  символом "решётки", а оканчивается 
 * символом ASCII(10). В начале строки могут шумовые символы.
 * Тело строки с командой содержит код команды и параметр, которые
 * разделяются символом "двоеточие".
  *******************************************************************/
#ifndef C_INCOMING
#define C_INCOMING

using namespace std;

//-----------------------------------------------------------------------
// КОНСТАНТЫ
//-----------------------------------------------------------------------

#define LF 10
#define OCTOTHORP 0x23

//-----------------------------------------------------------------------
// ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ
//-----------------------------------------------------------------------
//#include "config.h"
#include "cSend.h"

//-----------------------------------------------------------------------
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
//-----------------------------------------------------------------------
extern cSend SendInstance;

//-----------------------------------------------------------------------
// КЛАССЫ И СТРУКТУРЫ
//-----------------------------------------------------------------------
enum IncomingPhase{
    NONE = 0,
    SEARCH_HEAD = 1,
    SEARCH_TAIL = 2,
    SEARCH_COMPLETE = 3
};

class cIncoming
{
protected:
String Command;
String Parameter;
bool IsValid;
bool IsData;


public:

String Answer;
String Buffer;

char incomingByte;
char Phase;
int InvalidBytes;

String sJdyAnswer = "";

cIncoming();
void operate();
String getParameter();

private:
void execute();  

};//End of class cIncoming

cIncoming::cIncoming()
{
  this->Buffer = "";
  this->incomingByte = 0;
  this->InvalidBytes = 0;
  this->Command = "";
  this->Parameter = "";
  this->Phase = SEARCH_HEAD;
  this->IsValid = false;
  this->IsData = false;
}

String cIncoming::getParameter()
{
  return this->Parameter;  
}

// Реализация приёма команды
void cIncoming::operate()
{
  if(SERIAL_V.available() > 0) 
  {
    //Чтение байта из последовательного порта
    this->incomingByte = SERIAL_V.read();
    //Разбор принятого байта в зависимости от фазы приёма
    switch(this->Phase)
    {
      case SEARCH_HEAD:
      if(this->incomingByte == OCTOTHORP)
      {
        this->Phase = SEARCH_TAIL;  
      }
      else
      {
        this->InvalidBytes++;
      }
      break;

      case SEARCH_TAIL:
      if(this->incomingByte == LF)
      {
        this->Phase = SEARCH_COMPLETE;  
      }
      else
      {
        this->Buffer += String(this->incomingByte);
      }
      break;

      case SEARCH_COMPLETE:

      break;

      case NONE:

      break;

      default:
      break;
    }//End of switch(this->Phase)
    
  }//End of while(SERIAL_V.available() > 0)

  //Выполнение принятой команды
  this->execute();
   
}//End of void cIncoming::operate()

//====================================================
// Реализация функциональности
//====================================================
void cIncoming::execute()
{
  if(this->Phase == SEARCH_COMPLETE)
  {
    //================================
    // Выделение команды и параметра
    //================================
    int pos = this->Buffer.indexOf(':');
    if( pos >= 0 )
    {
      this->Command = this->Buffer.substring(0,pos);
      this->Parameter = this->Buffer.substring(pos+1);
    }
    else
    {
      this->Command = this->Buffer;
      this->Parameter = "";
    }
    //Сброс признака правильной команды
    this->IsValid = false;

    this->Answer = "{\n";//Начало ответа

    //Индикация результатов разбора
    SERIAL_V.print("Command:");
    SERIAL_V.print(this->Command);
    SERIAL_V.print(" Parameter:");
    SERIAL_V.println(this->Parameter);

    //------------------------------------------------------
    // Дешифровка команды
    //------------------------------------------------------
    if(Command == "Answer")
    {
      Answer += "Answer JDY-41:";
      Answer += this->sJdyAnswer;
      this->sJdyAnswer = "";
      this->IsValid = true;
    }//End of if(Command == "Reset")
    //------------------------------------------------------
    
    if(Command == "Set")
    {
      int Val;
      if(this->Parameter.length() > 0)
      {
        Val = Parameter.toInt();
      }
      else
      {
         Val = 0;
      }
      digitalWrite(SET, (Val > 0));
      digitalWrite(LED_1, !(Val > 0));
      
      Answer += "Pin'Set' JDY-41:";
      Answer += String(Val);
      this->IsValid = true;
    }//End of if(Command == "Set")
    //------------------------------------------------------
    
    if(Command == "Cs")
    {
      int Val;
      if(this->Parameter.length() > 0)
      {
        Val = Parameter.toInt();
      }
      else
      {
         Val = 0;
      }
      digitalWrite(CS, (Val > 0));
      digitalWrite(LED_2, !(Val > 0));
      
      Answer += "Pin'Cs' JDY-41:";
      Answer += String(Val);
      this->IsValid = true;
    }//End of if(Command == "Set")
    //------------------------------------------------------
    
    if(Command == "Reset")
    {
      //Подготовка команды RESET
      SendInstance.prepareReset();
      
      Answer += "Reset JDY-41";
      this->IsValid = true;
    }//End of if(Command == "Reset")
    //------------------------------------------------------
    
    if(Command == "ConfigId")
    {
      //Подготовка команды ConfigId
      SendInstance.prepareConfigId();
      
      Answer += "ConfigId JDY-41";
      this->IsValid = true;
    }//End of if(Command == "Reset")
    //------------------------------------------------------
    
    if(Command == "ConfigRole")
    {
      //Подготовка команды ConfigMode
      SendInstance.prepareConfigRole();
      
      Answer += "ConfigMode JDY-41";
      this->IsValid = true;
    }//End of if(Command == "Reset")
    //------------------------------------------------------
    
    if(Command == "Version")
    {
      //Подготовка команды Version
      SendInstance.prepareGetVersion();
      
      Answer += "ConfigMode JDY-41";
      this->IsValid = true;
    }//End of if(Command == "Reset")
    //------------------------------------------------------
    
    if(Command == "Send")
    {
      //Подготовка команды Send
      SendInstance.prepareSendFrame(this->Parameter);
      
      Answer += "SEND_FRAME:";
      Answer += this->Parameter;
      
      this->IsValid = true;
    }//End of if(Command == "Reset")
    //------------------------------------------------------
    
    //==============================================
    // Индикация ошибки 
    //==============================================
    if(this->IsValid)
    {
      if(this->IsData)
      {
        this->IsData = false;
      }
      else
      {
        //Окончание посылки и ее передача
        this->Answer = this->Answer + "\n}";
        SERIAL_V.println(this->Answer);
      }
    }//End of if(this->IsValid)
    else
    {
      this->Answer = this->Answer + "Invalid command or parameter value!";
      this->Answer = this->Answer + "\n}";
      SERIAL_V.println(this->Answer);
    }
    
    //Очистка буфера
    this->Answer = "";
    //Возврат к поиску начального маркера
    this->Buffer = "";
    this->Phase = SEARCH_HEAD;
  
  }//End of if(this->Phase == SEARCH_COMPLETE)
  
}//End of void cIncoming::execute()

#endif
