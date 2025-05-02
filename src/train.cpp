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
    }
    else {
        Car* last = first->prev;
        last->next = newCar;
        newCar->prev = last;
        newCar->next = first;
        first->prev = newCar;
    }
}

int Train::getLength() {
  if (!first) return 0;
  countOp = 0;
  Car* current = first;
  bool initialLight = current->light;
  int length = 0;
  bool foundDifferent = false;
  do {
        current = current->next;
        countOp++;
        if (current->light != initialLight) {
            foundDifferent = true;
            break;
        }
    } while (current != first);
    if (!foundDifferent) {
        first->light = !initialLight;
        countOp++;
        current = first; 
    }
    Car* start = current;
    length = 1; 
    current = current->next;
    countOp++; 
    while (current != start) {
        length++;
        current = current->next;
        countOp++; 
    }
    if (!foundDifferent) {
        first->light = initialLight; 
        countOp++;
    }
    return length;
}

int Train::getOpCount() { return countOp; }
void Train::resetOpCount() { countOp = 0; }
