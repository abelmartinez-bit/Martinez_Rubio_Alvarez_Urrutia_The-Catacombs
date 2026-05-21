#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class RelicTracker {
public:
    static void unlockRelic(const std::string& relicName);
    static std::vector<std::string> loadUnlockedRelics();
    static bool isRelicUnlocked(const std::string& relicName, const std::vector<std::string>& unlockedRelics);

    static void drawRelicTracker(sf::RenderWindow& window, sf::Font& font, const sf::Sprite& bgSprite);
};
