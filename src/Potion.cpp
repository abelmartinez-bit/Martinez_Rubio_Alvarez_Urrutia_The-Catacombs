#include "../include/Potion.h"

Potion::Potion(std::string name, MoveEffect effect, MoveTarget target, int potency)
    : name(name), effect(effect), target(target), potency(potency) {}

void Potion::applyEffect(Entity& targetEntity) {
    switch (effect) {
        case MoveEffect::HEAL:
            targetEntity.heal(targetEntity.getMaxHp() * potency / 100);
            break;
        case MoveEffect::ATK_UP:
            targetEntity.setAtk(targetEntity.getAtk() + (targetEntity.getAtk() * potency / 100));
            break;
        case MoveEffect::DEF_UP:
            targetEntity.setDef(targetEntity.getDef() + (targetEntity.getDef() * potency / 100));
            break;
        case MoveEffect::SPATK_UP:
            targetEntity.setSpAtk(targetEntity.getSpAtk() + (targetEntity.getSpAtk() * potency / 100));
            break;
        case MoveEffect::SPDEF_UP:
            targetEntity.setSpDef(targetEntity.getSpDef() + (targetEntity.getSpDef() * potency / 100));
            break;
        case MoveEffect::SPD_UP:
            targetEntity.setSpd(targetEntity.getSpd() + (targetEntity.getSpd() * potency / 100));
            break;
        default:
            break;
    }
}