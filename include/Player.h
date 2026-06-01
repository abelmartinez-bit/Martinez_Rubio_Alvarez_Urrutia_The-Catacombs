#pragma once
#include "Entity.h" //porque heredamos de
#include "Relic.h" //para el vector
#include "Potion.h" //para el vector
#include <vector>
#include <string>

enum class PlayerType {
    ARCHER, 
    MAGE,
    WARRIOR,
    ROGUE,
};

//igual que enemy pero con más adiciones
class Player : public Entity { 
//ahora si private porque son propias del jugador
private: 
    int level;
    int exp;
    int expToNextLevel;
    PlayerType type;
    //vector de pointers ya que no podemos guardar objetos Relic directamente en el vector 
    //ya que no pueden exsistir porque la clase Relic es abstracta
    // y asi apuntamos a objetos de clases hijas de Relic que implementen applyEffect
    std::vector<Relic*> relics;
    int potionSlots;
    std::vector<Potion> potions;
    std::vector<Move> movePool; // banco de movimientos de donde se seleccionan  
    
    // nuevos atributos para efectos de reliquias
    int expBoostPercent;
    int restoreOnLevelUpPercent;

    // modificadores de probabilidad para Floor de ProbabilityRelics
    float corruptDoorModifier;   // suma a la chance de puerta corrupta
    float minibossDropModifier;  // sube la rareza del drop al ganarle a un miniboss
    float enemyDropModifier;     // sube la rareza del drop al ganarle a un enemigo normal
    std::vector<std::string> activeSynergies;

    //nueva variable para level up sin cin
    int pendingStatBoosts; //para saber cuantas elecciones de stat faltan

    //nuevas variables para MoveRelic select sin cin
    bool hasPendingMove; // saber si hay un move esperando ser reemplazado o boosteado
    Move pendingMove; //guarda el move que se va a agregar o boostear

public:
    Player(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd, PlayerType playerType);

   // Getters
    int getLevel() const { return level; }
    int getExp() const { return exp; }
    int getExpToNextLevel() const { return expToNextLevel; }
    int getPotionSlots() const { return potionSlots; }
    std::vector<Relic*> getRelics() const { return relics; }
    std::vector<Potion> getPotions() const { return potions; }
    int getPendingStatBoosts() const { return pendingStatBoosts; }
    bool getHasPendingMove() const { return hasPendingMove; }
    Move getPendingMove() const { return pendingMove; }
    PlayerType getPlayerType() const { return type; }

    //acciones por ahora, adicionales a las heredadas
    void gainExp(int amount);
    void levelUp();
    void boostStat(int statChoice);
    // funcion para agregar reliquia, devuelve string para mostrar en el log del combate
    std::string addRelic(Relic* relic);

    // funciones para sinergias
    bool hasRelic(const std::string& relicName) const;
    bool hasSynergy(const std::string& synergyName) const;
    std::string checkRelicSynergies();
    //funciones para pociones 
    void addPotion(Potion potion);
    void usePotion(int index, Entity& target);
    void expandPotionSlots(int amount); // con relics
    // Nuevas funciones para los efectos de las reliquias
    void addExpBoost(int percent);
    void addRestoreOnLevelUp(int percent);
    // Probabilidad relics
    void addCorruptDoorModifier(float amount);
    void addMinibossDropModifier(float amount);
    void addEnemyDropModifier(float amount);

    //funciones para level up sin cin
    bool needsStatBoost() const { return pendingStatBoosts > 0; } //para saber si el jugador necesita elegir un stat boost
    void applyStatBoost(int choice); //para aplicar el boost elegido y reducir la variable pendingStatBoosts
    
    //funciones para MoveRelic sin cin
    void setPendingMove(const Move& m) { 
        pendingMove = m; //guarda el move de la relic 
        hasPendingMove = true; } //activa la variable que hay un move esperando
    bool hasPendingMoveRelic() const { return hasPendingMove; } //para saber si hay un move de relic esperando a ser aplicado
    void applyPendingMove(int slotIndex); //para reemplazar el move elegido y desactivar pending move

    // sus getters para que Floor los lea
    float getCorruptDoorModifier()  const { return corruptDoorModifier; }
    float getMinibossDropModifier() const { return minibossDropModifier; }
    float getEnemyDropModifier()    const { return enemyDropModifier; }

    //override para el virtual para mostrar stats del jugador, incluyendo nivel, exp, relics y potions
    void displayStats() const override;
    void initializeMovePool();
    void selectMovesFromPool();
    
    // Destructor para limpiar
    ~Player();

    // Player no se puede copiar porque maneja memoria con raw pointers (relics)
    // (Por el momento nunca se copia pero si en algun momento se quisiera copiar, esto previene bugs al intentar copiar sin querer y que ambos objetos apunten a las mismas reliquias)
    // Si se copiara, ambas copias intentarian hacer delete a los mismos pointers = crash
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
};