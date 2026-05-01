#include "../include/Move.h"

Move::Move(std::string name, MoveType type, int power, int accuracy, MoveEffect effect, MoveTarget target, int effectPotency)
    : name(name), type(type), power(power), accuracy(accuracy), effect(effect), target(target), effectPotency(effectPotency) {}



void Move::boostPower(int amount) {
    power += amount;
}
void Move::addAccuracy(int amount) {
    accuracy += amount;
    if (accuracy > 100) accuracy = 100; //100% cap
}