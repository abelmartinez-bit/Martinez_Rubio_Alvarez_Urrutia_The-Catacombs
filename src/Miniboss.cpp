#include "../include/Miniboss.h"

MiniBoss::MiniBoss(std::string name, EnemyType type, int hp, int atk, int def,
                   int spAtk, int spDef, int spd)
    : Enemy(name, type, hp, atk, def, spAtk, spDef, spd) {
    // Apply 2x exp multiplier (since already int, we static_cast to avoid data issues)
    expReward = static_cast<int>(expReward * 2);
    setMiniBossMoves();
}

//remember change effect values higher
void MiniBoss::setMiniBossMoves() {
    switch (type) {
        case EnemyType::SENTINEL:
            setMove(0, Move("Devastating Crush", MoveType::PHYSICAL, 100, 90));
            setMove(1, Move("Iron Fortress", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF));
            setMove(2, Move("Titanic Slam", MoveType::PHYSICAL, 110, 85));
            setMove(3, Move("War Cry", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF, 60));
            break;
        case EnemyType::SHADE:
            setMove(0, Move("Shadow Annihilation", MoveType::PHYSICAL, 105, 85));
            setMove(1, Move("Blur", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF, 65));
            setMove(2, Move("Lethal Strike", MoveType::PHYSICAL, 95, 95));
            setMove(3, Move("Paralyze", MoveType::EFFECT, 0, 95, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 45));
            break;
        case EnemyType::WRAITH:
            setMove(0, Move("Soul Annihilation", MoveType::SPECIAL, 100, 90));
            setMove(1, Move("Doom Curse", MoveType::EFFECT, 0, 90, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 50));
            setMove(2, Move("Ethereal Storm", MoveType::SPECIAL, 110, 85));
            setMove(3, Move("Terror Wail", MoveType::EFFECT, 0, 95, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 45));
            break;
        case EnemyType::GOLEM:
            setMove(0, Move("Cataclysmic Quake", MoveType::PHYSICAL, 110, 80));
            setMove(1, Move("Diamond Skin", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF));
            setMove(2, Move("Mountain Slam", MoveType::PHYSICAL, 100, 85));
            setMove(3, Move("Stone Fortress", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF));
            break;
        case EnemyType::SHAMAN:
            setMove(0, Move("Apocalyptic Hex", MoveType::EFFECT, 0, 95, MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 55));
            setMove(1, Move("Cursed Bolt", MoveType::SPECIAL, 90, 90));
            setMove(2, Move("Mass Enfeeble", MoveType::EFFECT, 0, 90, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 45));
            setMove(3, Move("Dark Empowerment", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_UP, MoveTarget::SELF, 50));
            break;
        case EnemyType::SPECTER:
            setMove(0, Move("Void Annihilation", MoveType::SPECIAL, 105, 90));
            setMove(1, Move("Phase Shift", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF, 60));
            setMove(2, Move("Soul Devastation", MoveType::SPECIAL, 110, 85));
            setMove(3, Move("Spectral Curse", MoveType::EFFECT, 0, 95, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 45));
            break;
    }
}