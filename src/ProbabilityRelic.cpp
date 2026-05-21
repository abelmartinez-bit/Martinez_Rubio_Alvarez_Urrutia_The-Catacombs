#include "../include/ProbabilityRelic.h"
#include <iostream>

ProbabilityRelic::ProbabilityRelic(std::string name, std::string description, RelicRarity rarity, ProbabilityType probabilityType, float modifier)
                : Relic(name, description, rarity), probabilityType(probabilityType), modifier(modifier) {}

ProbabilityType ProbabilityRelic::getProbabilityType() const { return probabilityType; }
float ProbabilityRelic::getModifier() const { return modifier; }

void ProbabilityRelic::applyEffect(Player& player) {
    switch (probabilityType) {
        case ProbabilityType::MOVE_ACCURACY:
        {
            std::vector<Move> moves = player.getMoves();
            for (int i = 0; i < 4; i++) {
                moves[i].addAccuracy((int)modifier);
                player.setMove(i, moves[i]);
            }
            std::cout << "All move accuracy increased by " << modifier << "\n";
            break;
        }
        case ProbabilityType::CORRUPT_DOOR:
            player.addCorruptDoorModifier(modifier);
            std::cout << "Corrupt door chance changed by " << modifier << "%\n";
            break;
        case ProbabilityType::MINIBOSS_DROP:
            player.addMinibossDropModifier(modifier);
            std::cout << "Miniboss relic drop quality changed by " << modifier << "%\n";
            break;
        case ProbabilityType::ENEMY_DROP:
            player.addEnemyDropModifier(modifier);
            std::cout << "Enemy relic drop quality changed by " << modifier << "%\n";
            break;
    }
}