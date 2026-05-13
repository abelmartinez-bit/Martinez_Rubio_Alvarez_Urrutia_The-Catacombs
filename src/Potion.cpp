#include "../include/Potion.h"
#include "../include/Entity.h" 
// could lead to circular include but we only need the declaration of Entity for the applyEffect function 
// so we can just do a forward declaration in Potion.h and include Entity.h here in the cpp file


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

Potion Potion::createPotion(int index) {
    switch (index) {
        case 0: return Potion("Healing Potion", MoveEffect::HEAL, MoveTarget::SELF, 20);
        case 1: return Potion("Healing+ Potion", MoveEffect::HEAL, MoveTarget::SELF, 45);
        case 2: return Potion("Strength Potion", MoveEffect::ATK_UP, MoveTarget::SELF, 10);
        case 3: return Potion("Strength+ Potion", MoveEffect::ATK_UP, MoveTarget::SELF, 25);
        case 4: return Potion("Weakness Flask", MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 15);
        case 5: return Potion("Weakness+ Flask", MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 30);
        case 6: return Potion("Defense Potion", MoveEffect::DEF_UP, MoveTarget::SELF, 10);
        case 7: return Potion("Defense+ Potion", MoveEffect::DEF_UP, MoveTarget::SELF, 25);
        case 8: return Potion("Vulnerability Flask", MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 15);
        case 9: return Potion("Vulnerability+ Flask", MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 30);
        case 10: return Potion("Dexterity Potion", MoveEffect::SPATK_UP, MoveTarget::SELF, 10);
        case 11: return Potion("Dexterity+ Potion", MoveEffect::SPATK_UP, MoveTarget::SELF, 25);
        case 12: return Potion("Brainfog Flask", MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 15);
        case 13: return Potion("Brainfog+ Flask", MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 30);
        case 14: return Potion("Soul Shield Potion", MoveEffect::SPDEF_UP, MoveTarget::SELF, 10);
        case 15: return Potion("Soul Shield+ Potion", MoveEffect::SPDEF_UP, MoveTarget::SELF, 25);
        case 16: return Potion("Soul Fracture Flask", MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 15);
        case 17: return Potion("Soul Fracture+ Flask", MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 30);
        case 18: return Potion("Swiftness Potion", MoveEffect::SPD_UP, MoveTarget::SELF, 10);
        case 19: return Potion("Swiftness+ Potion", MoveEffect::SPD_UP, MoveTarget::SELF, 25);
        case 20: return Potion("Slowness Flask", MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 15);
        case 21: return Potion("Slowness+ Flask", MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 30);
        default: return Potion("Healing Potion", MoveEffect::HEAL, MoveTarget::SELF, 20);
    }
}


// ===== RANDOM POTION =====

Potion Potion::createRandomPotion() {
    int idx = rand() % getPotionCount();
    return createPotion(idx);
}


// ===== TOTAL DE POCIONES =====

int Potion::getPotionCount() {
    return 22;
}