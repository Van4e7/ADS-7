// Copyright 2021 NNTU-CS
#include "train.h"

Train::Train() : countOp(0), first(nullptr) {}
Train::~Train() {
  if (first) {
    Car* current = first->next;
    while (current != first) {
      Car* next = current->next;
      delete current;
      current = next;
    }
    delete first;
    first = nullptr;
  }
}

void Train::addCar(bool light) {
  Car* newCar = new Car;
  newCar->light = light;

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
  if (!first) return 0;

  int length = 0;
  Car* current = first;
  do {
    length++;
    current = current->next;
    countOp++; 
  } while (current != first);

  return length;
}

int Train::getOpCount() { return countOp; }
void Train::resetOpCount() { countOp = 0; }
void Train::setAllLightsOff() {
  if (!first) return;

  Car* current = first;
  do {
    current->light = false;
    current = current->next;
  } while (current != first);
}

void Train::setAllLightsOn() {
  if (!first) return;

  Car* current = first;
  do {
    current->light = true;
    current = current->next;
  } while (current != first);
}

void Train::setRandomLights() {
  if (!first) return;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 1);

  Car* current = first;
  do {
    current->light = (distrib(gen) == 1);
    current = current->next;
  } while (current != first);
}

void Train::moveNext() {
    if (first) {
        first = first->next;
        countOp++;
    }
}

void Train::movePrev() {
    if (first) {
        first = first->prev;
        countOp++;
    }
}

bool Train::getLight() {
    if (first) {
        return first->light;
    }
    return false; 
}

void Train::toggleLight() {
    if (first) {
        first->light = !first->light;
    }
}
