#pragma once
#include "Entity.h" //porque heredamos de
#include "Relic.h" //para el vector
#include "Potion.h" //para el vector
#include <vector>

//igual que enemy pero con más adiciones
class Player : public Entity { 
//ahora si private porque son propias del jugador
private: 
    int level;
    int exp;
    int expToNextLevel;
    //vector de pointers ya que no podemos guardar objetos Relic directamente en el vector 
    //ya que no pueden exsistir porque la clase Relic es abstracta
    // y asi apuntamos a objetos de clases hijas de Relic que implementen applyEffect
    std::vector<Relic*> relics;
    int potionSlots;
    std::vector<Potion> potions;

public:
    Player(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd);

    // Getters
    int getLevel() const;
    int getExp() const;
    int getExpToNextLevel() const;
    int getPotionSlots() const;
    std::vector<Relic*> getRelics() const;
    std::vector<Potion> getPotions() const;

    //acciones por ahora, adicionales a las heredadas
    void gainExp(int amount);
    void levelUp();
    void boostStat(int statChoice);
    void addRelic(Relic* relic);
    void addPotion(Potion potion);
    void usePotion(int index, Entity& target);
    void expandPotionSlots(int amount); // con relics

    //override para el virtual para mostrar stats del jugador, incluyendo nivel, exp, relics y potions
    void displayStats() const override;
};