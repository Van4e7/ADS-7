// Copyright 2021 NNTU-CS
#include "train.h"

Train::Train() : countOp(0), first(nullptr) {}
Train::~Train() {
    if (first) {
        Car* current = first;
        Car* next;
        do {
            next = current->next;
            delete current;
            current = next;
        } while (current != first);
    }
}

void Train::addCar(bool light) {
    Car* newCar = new Car{ light, nullptr, nullptr };

    if (!first) {
        first = newCar;
        first->next = first;
        first->prev = first;
    } else {
        Car* last = first->prev;
        last->next = newCar;
        newCar->prev = last;
        newCar->next = first;
        first->prev = newCar;
    }
}

int Train::getLength() {
    countOp = 0;
    Car* temp;
    while (true) {
      temp = first;
      int lenght = 1;
      if (!temp->light) {
        temp->light = true;
    }
    temp = temp->next;
    countOp += 2;
    while (!temp->light) {
      temp = temp->next;
      countOp += 2;
      lenght++;
    }
    temp->light = false;
    if (!first->light) {
      return lenght;
    }
  }
}

int Train::getOpCount() { return countOp; }
void Train::resetOpCount() { countOp = 0; }
