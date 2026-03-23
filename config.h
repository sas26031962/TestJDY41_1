/*
 Аппаратная конфигурация контроллера RS485
 */
#ifndef CONFIG_H
#define CONFIG_H
 //========================================================================
 //------------------------- АППАРАТНАЯ КОНФИГУРАЦИЯ ----------------------
 //========================================================================

 #define TITLE "\n{===Title:TestJDY-41 v.1.0.0 ===}"
 
 //--- Ресурсы платы контроллера RS485

#define LED_1 22   //Светодиод - повторитель SET
#define LED_2 24   //Светодиод - повторитель CS

// #define LED_3 26   //Светодиод MODBUS frame
// #define LED_4 28   //Светодиод - повторитель RESIVE_CONTROL
// #define LED_5 30   //Светодиод - повторитель TRANSMIT_CONTROL
// #define LED_6 32   //Светодиод - ошибка контрольной суммы по каналу Pipe 
// #define LED_7 34   //Светодиод приём команды ОК по каналу Pipe 
#define LED_8 36   //Светодиод - индикация наличия символов в приёмнике
 
// #define KEY_1 23   //Кнопка COMMUNICATION_VIEW
// #define KEY_2 25   //Кнопка (зарезервировано для будущего использования)
// #define KEY_3 27   //Кнопка (зарезервировано для будущего использования)
// #define KEY_4 29   //Кнопка (зарезервировано для будущего использования)
// #define KEY_5 31   //Кнопка (зарезервировано для будущего использования)
// #define KEY_6 33   //Кнопка ASYNC MODE
 
 //--- Параметры связи 
 #define COMMUNICATION_SPEED 115200 
 #define JDY41_SPEED 9600 

 #define SERIAL_X Serial3   //Порт для работы с JDY-41 
 #define SERIAL_V Serial    //Порт для работы с интерфейсом Incomint 
  
 // Управление модулем JDY-41
#define CS 44               //Выбор модуля
#define SET 42              //Программирование модуля
 
 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================

enum class cTaskType{
  IO1_LEFT,
  IO2_LEFT,
  HG1_LEFT,
  HG2_LEFT,
  HG3_LEFT,
  HG4_LEFT, 
  
  IO1_RIGHT,
  IO2_RIGHT,
  HG1_RIGHT,
  HG2_RIGHT,
  HG3_RIGHT,
  HG4_RIGHT,

  ANGLE_REQUEST,
  CONTROL_REQUEST,
  CONTROL,
  ANGLE_LEFT,
  ANGLE_RIGHT,

  IO3_LEFT,
  IO4_LEFT,

  IO3_RIGHT,
  IO4_RIGHT,

  IO2_BZSAP,
  IO3_BZSAP,
  ASK,
};
 
 
#endif
