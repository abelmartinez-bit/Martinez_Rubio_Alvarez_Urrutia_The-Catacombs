#include "../include/RelicTracker.h"
#include <fstream>
#include <iostream>

static const std::string RELIC_TRACKER_FILE = "unlocked_relics.txt";

std::vector<std::pair<std::string, std::string>> RelicTracker::loadUnlockedRelics() {
    std::vector<std::pair<std::string, std::string>> unlockedRelics;
    std::ifstream file(RELIC_TRACKER_FILE);

    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            size_t separatorPosition = line.find('|');

            if (separatorPosition != std::string::npos) {
                std::string relicName = line.substr(0, separatorPosition);
                std::string relicDescription = line.substr(separatorPosition + 1);

                unlockedRelics.push_back({relicName, relicDescription});
            } else {
                unlockedRelics.push_back({line, "No description available."});
            }
        }
    }

    return unlockedRelics;
}

bool RelicTracker::isRelicUnlocked(const std::string& relicName, const std::vector<std::pair<std::string, std::string>>& unlockedRelics) {
    for (const auto& unlockedRelic : unlockedRelics) {
        if (unlockedRelic.first == relicName) {
            return true;
        }
    }

    return false;
}

void RelicTracker::unlockRelic(const std::string& relicName, const std::string& relicDescription) {
    std::vector<std::pair<std::string, std::string>> unlockedRelics = loadUnlockedRelics();

    if (isRelicUnlocked(relicName, unlockedRelics)) {
        return;
    }

    std::ofstream file(RELIC_TRACKER_FILE, std::ios::app);

    if (!file.is_open()) {
        std::cout << "Could not open relic tracker file.\n";
        return;
    }

    file << relicName << "|" << relicDescription << "\n";
    std::cout << "Relic tracked: " << relicName << " - " << relicDescription << "\n";
}

void RelicTracker::drawRelicTracker(sf::RenderWindow& window, sf::Font& font, const sf::Sprite& bgSprite) {
    std::vector<std::pair<std::string, std::string>> unlockedRelics = loadUnlockedRelics();

    window.draw(bgSprite);

    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text title(font, "UNLOCKED RELICS", 40);
    title.setFillColor(sf::Color(210, 160, 60));

    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({
        (800.f - titleBounds.size.x) / 2.f - titleBounds.position.x,
        40.f
    });

    window.draw(title);

    sf::Text hint(font, "Press B or R to return to the main menu", 15);
    hint.setFillColor(sf::Color(160, 160, 160));

    sf::FloatRect hintBounds = hint.getLocalBounds();
    hint.setPosition({
        (800.f - hintBounds.size.x) / 2.f - hintBounds.position.x,
        95.f
    });

    window.draw(hint);

    sf::RectangleShape panel({660.f, 420.f});
    panel.setPosition({70.f, 135.f});
    panel.setFillColor(sf::Color(20, 12, 5, 210));
    window.draw(panel);

    if (unlockedRelics.empty()) {
        sf::Text emptyText(font, "No relics discovered yet.", 22);
        emptyText.setFillColor(sf::Color::White);

        sf::FloatRect emptyBounds = emptyText.getLocalBounds();
        emptyText.setPosition({
            (800.f - emptyBounds.size.x) / 2.f - emptyBounds.position.x,
            320.f
        });

        window.draw(emptyText);
        return;
    }

    float startX = 105.f;
    float startY = 155.f;
    float rowGap = 42.f;

    for (int i = 0; i < static_cast<int>(unlockedRelics.size()); i++) {
        std::string relicName = unlockedRelics[i].first;
        std::string relicDescription = unlockedRelics[i].second;

        sf::Text relicNameText(font, std::to_string(i + 1) + ". " + relicName, 15);
        relicNameText.setFillColor(sf::Color(230, 220, 190));
        relicNameText.setPosition({startX, startY + i * rowGap});
        window.draw(relicNameText);

        sf::Text relicDescriptionText(font, relicDescription, 12);
        relicDescriptionText.setFillColor(sf::Color(170, 170, 170));
        relicDescriptionText.setPosition({startX + 20.f, startY + 18.f + i * rowGap});
        window.draw(relicDescriptionText);
    }
}