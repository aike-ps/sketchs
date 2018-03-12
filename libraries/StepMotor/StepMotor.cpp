/*
StepMotor.h - библиотека управления шаговыми двигателями

Подробно описана в Уроке 29.
http://mypractic.ru/uroki-programmirovaniya-arduino-navigaciya-po-urokam

Библиотека разработана Калининым Эдуардом
mypractic.ru 
*/

#include "Arduino.h"
#include "StepMotor.h"

//---------------------------- конструктор -----------------------------------
StepMotor::StepMotor (byte pinA, byte pinB, byte pinC, byte pinD) {

  // перегрузка номеров выводов
  _pinA = pinA;
  _pinB = pinB;
  _pinC = pinC;
  _pinD = pinD;
  
  // установка выводов
  pinMode(_pinA, OUTPUT); digitalWrite(_pinA, LOW);
  pinMode(_pinB, OUTPUT); digitalWrite(_pinB, LOW);
  pinMode(_pinC, OUTPUT); digitalWrite(_pinC, LOW);
  pinMode(_pinD, OUTPUT); digitalWrite(_pinD, LOW);
  
  // начальное состояние параметров
  _steps= 0;
  _fixStop= false;
  _mode= 0;
  _stepPhase= 0;
  _divider= 100;
  _dividerCount= 0;     
}


//------------------------------- управление коммутацией фаз
// метод должен вызываться регулярно с максимальной частотой коммутации фаз
void  StepMotor::control() {

  // делитель частоты коммутации
  if ( _steps == 0 ) { 
    // двигатель остановлен
    _dividerCount= 65534;  // сброс делителя частоты
    _prevSteps = 0;
    return;
    }
  else  {
    // двигатель не остановлен
    _dividerCount++;  
    if ( _dividerCount < _divider ) return;  
      _dividerCount= 0;        
  }

  if ( _prevSteps != 0 ) {
    if ( _steps > 0 ) _steps--; // вращение против часовой стрелки
    else              _steps++; // вращение по часовой стрелке           
  }
  
  _prevSteps = _steps;

  if ( _steps == 0 ) {
    if ((_steps == 0) && (_fixStop == false)) {
      // выключение фаз при остановке
      digitalWrite(_pinA, LOW);
      digitalWrite(_pinB, LOW);
      digitalWrite(_pinC, LOW);
      digitalWrite(_pinD, LOW);
    }
    return;
  }
          
  if ( _steps > 0 ) _stepPhase++; // вращение против часовой стрелки        
  else              _stepPhase--; // вращение по часовой стрелке

  // ограничение _stepPhase
    if ( _mode == 1 )   _stepPhase &= 7;
    else                _stepPhase &= 3;    
    
  // коммутация фаз 
  // шаговый режим
  if ( _mode == 0 )  {

    switch (_stepPhase)  {
      case 0:
        digitalWrite(_pinA, HIGH);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, LOW);        
      break;
      case 1:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, HIGH);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, LOW);        
      break;
      case 2:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, HIGH);        
        digitalWrite(_pinD, LOW);        
      break;
      case 3:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, HIGH);        
      break;            
    }    
  }
  
  // полу шаговый режим
  else if ( _mode == 1 )  {

    switch (_stepPhase)  {
      case 0:
        digitalWrite(_pinA, HIGH);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, LOW);        
      break;
      case 1:
        digitalWrite(_pinA, HIGH);        
        digitalWrite(_pinB, HIGH);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, LOW);        
      break;
      case 2:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, HIGH);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, LOW);        
      break;
      case 3:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, HIGH);        
        digitalWrite(_pinC, HIGH);        
        digitalWrite(_pinD, LOW);        
      break;
      case 4:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, HIGH);        
        digitalWrite(_pinD, LOW);        
      break;
      case 5:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, HIGH);        
        digitalWrite(_pinD, HIGH);        
      break;
      case 6:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, HIGH);        
      break;
      case 7:
        digitalWrite(_pinA, HIGH);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, HIGH);        
      break;
    }    
  }
  
  // между шаговый режим
  else  {

    switch (_stepPhase)  {
      case 0:
        digitalWrite(_pinA, HIGH);        
        digitalWrite(_pinB, HIGH);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, LOW);        
      break;
      case 1:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, HIGH);        
        digitalWrite(_pinC, HIGH);        
        digitalWrite(_pinD, LOW);        
      break;
      case 2:
        digitalWrite(_pinA, LOW);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, HIGH);        
        digitalWrite(_pinD, HIGH);        
      break;
      case 3:
        digitalWrite(_pinA, HIGH);        
        digitalWrite(_pinB, LOW);        
        digitalWrite(_pinC, LOW);        
        digitalWrite(_pinD, HIGH);        
      break;
    }    
  }               
}


//------------------------------- запуск вращения
// инициирует поворот двигателя на заданное число шагов
void  StepMotor::step(int steps) {
  noInterrupts();
  _steps= steps;
  interrupts();

  if ((_steps == 0) && (_fixStop == false)) {
    // выключение фаз при остановке
    digitalWrite(_pinA, LOW);
    digitalWrite(_pinB, LOW);
    digitalWrite(_pinC, LOW);
    digitalWrite(_pinD, LOW);
  }
  
}
//------------------------------ режим коммутации фаз и остановки
void  StepMotor::setMode(byte stepMode, boolean fixStop)  {  
  _mode= stepMode;
  _fixStop= fixStop;
}

//------------------------------ установка делителя частоты для коммутации фаз
void  StepMotor::setDivider(int divider)  {
  _divider= divider;
  //_dividerCount= 0;  
}

//----------------------------- чтение оставшихся шагов
int StepMotor::readSteps()  {
  int stp;
  noInterrupts();
  stp= _steps;
  interrupts();
  return(stp);
}
