#include "../include/MoveRelic.h"
#include <iostream>

MoveRelic::MoveRelic(std::string name, std::string description, RelicRarity rarity, Move move, int powerBoost)
    : Relic(name, description, rarity), move(move), powerBoost(powerBoost) {}

void MoveRelic::applyEffect(Player& player) {
    std::vector<Move> moves = player.getMoves();

    // Chequeo para el boost si ya tiene el move
    for (int i = 0; i < 4; i++) {
        if (moves[i].getName() == move.getName()) {
            moves[i].boostPower(powerBoost);
            player.setMove(i, moves[i]);
            std::cout << move.getName() << " power boosted by " << powerBoost << "!\n";
            return;
        }
    }

    //caso normal nuevo, ahora solo guardamos el move pendiente para mostrar la pantalla y elegir el slot
    player.setPendingMove(move);
    std::cout << "Elige que move reemplazar con " << move.getName() << "\n";
}