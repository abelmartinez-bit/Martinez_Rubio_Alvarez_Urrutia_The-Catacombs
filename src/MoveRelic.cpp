#include "../include/MoveRelic.h"
#include <iostream>

MoveRelic::MoveRelic(std::string name, std::string description, RelicRarity rarity, Move move, int powerBoost)
    : Relic(name, description, rarity), move(move), powerBoost(powerBoost) {}

void MoveRelic::applyEffect(Player& player) {
    std::vector<Move> moves = player.getMoves();

    // Chequeo p el boost si ya tiene el move
    for (int i = 0; i < 4; i++) {
        if (moves[i].getName() == move.getName()) {
            moves[i].boostPower(powerBoost);
            player.setMove(i, moves[i]);
            std::cout << move.getName() << " power boosted by " << powerBoost << "!\n";
            return;
        }
    }

    // Caso normal, elige un move a reemplazar
    std::cout << "Choose a move to replace with " << move.getName() << ":\n";
    for (int i = 0; i < 4; i++) {
        std::cout << i + 1 << ". " << moves[i].getName() << "\n";
    } 

    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= 4) {
        player.setMove(choice - 1, move);
        std::cout << "Move replaced with " << move.getName() << "!\n";
    } else {
        std::cout << "Invalid choice, replacing first slot.\n";
        player.setMove(0, move);
    }
}