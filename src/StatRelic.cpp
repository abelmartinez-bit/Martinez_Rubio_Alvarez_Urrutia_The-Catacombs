#include "../include/StatRelic.h"

StatRelic::StatRelic(std::string name, std::string description, RelicRarity rarity, StatType stat, int boostAmount)
    : Relic(name, description, rarity), stat(stat), boostAmount(boostAmount) {}

void StatRelic::applyEffect(Player& player) {
    switch (stat) {
        case StatType::HP:    
            player.setMaxHp(player.getMaxHp() + boostAmount); 
            break;
        case StatType::ATK:   
            player.setAtk(player.getAtk() + boostAmount); 
            break;
        case StatType::DEF:   
            player.setDef(player.getDef() + boostAmount); 
            break;
        case StatType::SPATK: 
            player.setSpAtk(player.getSpAtk() + boostAmount); 
            break;
        case StatType::SPDEF: 
            player.setSpDef(player.getSpDef() + boostAmount); 
            break;
        case StatType::SPD:   
            player.setSpd(player.getSpd() + boostAmount); 
            break;
    }
}   