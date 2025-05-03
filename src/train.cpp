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
    countOp = 0;
    Car* current = first;
    bool initialLight = current->light;
    int length = 0;
    bool allSame = true;
    do {
        if (current->light != initialLight) {
            allSame = false;
        }
        current = current->next;
        countOp+=2;
    } while (current != first);
    if (allSame) {
        first->light = !initialLight;
        countOp++;
    }
    length = 1;
    current = first->next; 
    countOp++;              
    while (current != first) {
        length++;
        current = current->next;
        countOp+=2;        
    }
    if (allSame) {
        first->light = initialLight;
        countOp++;     
    }
    return length;
}

int Train::getOpCount() { return countOp; }
void Train::resetOpCount() { countOp = 0; }
