// Copyright 2022 NNTU-CS AND V47
#ifndef INCLUDE_TRAIN_H_
#define INCLUDE_TRAIN_H_

#include <iostream>
#include <random>

class Train {
 private:
  struct Car {
    bool light; // состояние лампочки
    Car *next;
    Car *prev;
  };
  int countOp; // счетчик шагов (число переходов из вагона в вагон)
  Car *first; // точка входа в поезд (первый вагон)
 public:
  Train();
  ~Train();
  void addCar(bool light); // добавить вагон с начальным состоянием лампочки
  int getLength();          // вычислить длину поезда
  int getOpCount();         // вернуть число переходов (из вагона в вагон)
  void resetOpCount();
};
#endif  // INCLUDE_TRAIN_H_
