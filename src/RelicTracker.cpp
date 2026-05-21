#include "../include/RelicTracker.h"
#include <fstream>
#include <iostream>

static const std::string RELIC_TRACKER_FILE = "unlocked_relics.txt";

std::vector<std::string> RelicTracker::loadUnlockedRelics() {
    std::vector<std::string> unlockedRelics;
    std::ifstream file(RELIC_TRACKER_FILE);

    std::string relicName;
    while (std::getline(file, relicName)) {
        if (!relicName.empty()) {
            unlockedRelics.push_back(relicName);
        }
    }

    return unlockedRelics;
}

bool RelicTracker::isRelicUnlocked(const std::string& relicName, const std::vector<std::string>& unlockedRelics) {
    for (const std::string& unlockedRelic : unlockedRelics) {
        if (unlockedRelic == relicName) {
            return true;
        }
    }

    return false;
}

void RelicTracker::unlockRelic(const std::string& relicName) {
    std::vector<std::string> unlockedRelics = loadUnlockedRelics();

    if (isRelicUnlocked(relicName, unlockedRelics)) {
        return;
    }

    std::ofstream file(RELIC_TRACKER_FILE, std::ios::app);

    if (!file.is_open()) {
        std::cout << "Could not open relic tracker file.\n";
        return;
    }

    file << relicName << "\n";
    std::cout << "Relic tracked: " << relicName << "\n";
}

void RelicTracker::drawRelicTracker(sf::RenderWindow& window, sf::Font& font, const sf::Sprite& bgSprite) {
    std::vector<std::string> unlockedRelics = loadUnlockedRelics();

    window.draw(bgSprite);

    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text title(font, "UNLOCKED RELICS", 46);
    title.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({(800.f - titleBounds.size.x) / 2.f - titleBounds.position.x, 45.f});
    window.draw(title);

    sf::Text hint(font, "Press B or R to return to the main menu", 18);
    hint.setFillColor(sf::Color(160, 160, 160));
    sf::FloatRect hintBounds = hint.getLocalBounds();
    hint.setPosition({(800.f - hintBounds.size.x) / 2.f - hintBounds.position.x, 105.f});
    window.draw(hint);

    sf::RectangleShape panel({620.f, 390.f});
    panel.setPosition({90.f, 155.f});
    panel.setFillColor(sf::Color(20, 12, 5, 210));
    window.draw(panel);

    if (unlockedRelics.empty()) {
        sf::Text emptyText(font, "No relics discovered yet.", 24);
        emptyText.setFillColor(sf::Color::White);
        sf::FloatRect emptyBounds = emptyText.getLocalBounds();
        emptyText.setPosition({(800.f - emptyBounds.size.x) / 2.f - emptyBounds.position.x, 320.f});
        window.draw(emptyText);
        return;
    }

    float startX = 130.f;
    float startY = 185.f;
    float rowGap = 28.f;

    for (int i = 0; i < static_cast<int>(unlockedRelics.size()) && i < 12; i++) {
        sf::Text relicText(font, std::to_string(i + 1) + ". " + unlockedRelics[i], 20);
        relicText.setFillColor(sf::Color(230, 220, 190));
        relicText.setPosition({startX, startY + i * rowGap});
        window.draw(relicText);
    }

    if (unlockedRelics.size() > 12) {
        sf::Text moreText(font, "...and more relics discovered.", 18);
        moreText.setFillColor(sf::Color(160, 160, 160));
        moreText.setPosition({startX, startY + 12 * rowGap + 10.f});
        window.draw(moreText);
    }
}