/*
StepMotor.h - библиотека управления шаговыми двигателями

Подробно описана в Уроке 29.
http://mypractic.ru/uroki-programmirovaniya-arduino-navigaciya-po-urokam

Библиотека разработана Калининым Эдуардом
mypractic.ru 
*/

// проверка, что библиотека еще не подключена
#ifndef StepMotor_h // если библиотека StepMotor не подключена
#define StepMotor_h // тогда подключаем ее

#include "Arduino.h"

class StepMotor {

  public:
    StepMotor(byte pinA, byte pinB, byte pinC, byte pinD); // конструктор
    void  control();  // управление, метод должен вызываться регулярно с максимальной частотой коммутации фаз
    void  step(int steps);  // инициирует поворот двигателя на заданное число шагов
    void  setMode(byte stepMode, boolean fixStop);  // задает режимы коммутации фаз и остановки
    void  setDivider(int divider);  // установка делителя частоты для коммутации фаз
    int readSteps();  // чтение оставшихся шагов

    private:
      int _steps;        // оставшееся число шагов 
      boolean _fixStop;  // признак фиксации положения при остановке
      byte  _mode;       // режим: 0 - шаговый, 1 - полушаговый, 2 - междушаговый
      char  _stepPhase;  // номер шага между фазами
      unsigned int  _divider;  // делитель частоты для коммутации фаз
      unsigned int  _dividerCount;  // счетчик делителя частоты для коммутации фаз
      byte _pinA, _pinB, _pinC, _pinD;
      int _prevSteps;      	  
} ;

#endif
