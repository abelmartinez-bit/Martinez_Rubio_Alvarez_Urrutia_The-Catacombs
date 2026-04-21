#pragma once
#include "Relic.h"
#include "Player.h"
#include "Move.h"

//relic que da un nuevo movimiento
class MoveRelic : public Relic {
private:
    Move move;
    int powerBoost; //si el jugador ya tiene el move, boostea su power en vez de dar un nuevo move

public:
    MoveRelic(std::string name, std::string description, RelicRarity rarity, Move move, int powerBoost);

    void applyEffect(Player& player) override;
};