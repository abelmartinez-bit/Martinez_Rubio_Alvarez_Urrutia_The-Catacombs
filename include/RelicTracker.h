#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <utility>

class RelicTracker {
public:
    static void unlockRelic(const std::string& relicName, const std::string& relicDescription);
    static std::vector<std::pair<std::string, std::string>> loadUnlockedRelics();
    static bool isRelicUnlocked(const std::string& relicName, const std::vector<std::pair<std::string, std::string>>& unlockedRelics);
    static void drawRelicTracker(sf::RenderWindow& window,sf::Font& font,sf::Font& fontSmall, const sf::Sprite& bgSprite);
};