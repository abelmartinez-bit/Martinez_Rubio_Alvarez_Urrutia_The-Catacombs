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
        case 0: return Potion("Health Potion",    MoveEffect::HEAL,      MoveTarget::SELF, 25);
        case 1: return Potion("Greater Healing",  MoveEffect::HEAL,      MoveTarget::SELF, 50);
        case 2: return Potion("Strength Potion",  MoveEffect::ATK_UP,    MoveTarget::SELF, 30);
        case 3: return Potion("Iron Skin Potion", MoveEffect::DEF_UP,    MoveTarget::SELF, 30);
        case 4: return Potion("Mage's Brew",      MoveEffect::SPATK_UP,  MoveTarget::SELF, 30);
        case 5: return Potion("Mystic Ward",      MoveEffect::SPDEF_UP,  MoveTarget::SELF, 30);
        case 6: return Potion("Swiftness Potion", MoveEffect::SPD_UP,    MoveTarget::SELF, 25);
        case 7: return Potion("Elixir of Power",  MoveEffect::ATK_UP,    MoveTarget::SELF, 50);
        default: return Potion("Health Potion",   MoveEffect::HEAL,      MoveTarget::SELF, 25);
    }
}

// Genera una pocion random del catalogo
Potion Potion::createRandomPotion() {
    int idx = rand() % getPotionCount();
    return createPotion(idx);
}

// Cuenta total de pociones en el catalogo
int Potion::getPotionCount() {
    return 8;
}
