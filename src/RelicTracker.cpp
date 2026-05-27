#include "../include/RelicTracker.h"
#include <fstream>
#include <iostream>

static const std::string RELIC_TRACKER_FILE = "unlocked_relics.txt";

// Initialize static member
float RelicTracker::scrollOffset = 0.f;

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

void RelicTracker::handleScroll(int delta) {
    scrollOffset -= delta * 10.f;  // Multiply by 10 for more noticeable scrolling
}

void RelicTracker::resetScroll() {
    scrollOffset = 0.f;
}

void RelicTracker::drawRelicTracker(sf::RenderWindow& window, sf::Font& font, sf::Font& fontSmall, const sf::Sprite& bgSprite) {
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

    sf::Text hint(fontSmall, "Press B to return to the main menu | Scroll to see more relics", 15);
    hint.setFillColor(sf::Color(160, 160, 160));

    sf::FloatRect hintBounds = hint.getLocalBounds();
    hint.setPosition({(800.f - hintBounds.size.x) / 2.f - hintBounds.position.x, 95.f});

    window.draw(hint);

    const float panelX = 70.f;      // Left edge
    const float panelY = 130.f;     // Top edge
    const float panelW = 660.f;     // Width
    const float panelH = 440.f;     // Height

    sf::RectangleShape panel({panelW, panelH});
    panel.setPosition({panelX, panelY});
    panel.setFillColor(sf::Color(20, 12, 5, 210));
    window.draw(panel);

    if (unlockedRelics.empty()) {
        sf::Text emptyText(fontSmall, "No relics discovered yet.", 22);
        emptyText.setFillColor(sf::Color::White);

        sf::FloatRect emptyBounds = emptyText.getLocalBounds();
        emptyText.setPosition({
            (800.f - emptyBounds.size.x) / 2.f - emptyBounds.position.x,
            320.f
        });

        window.draw(emptyText);
        return;
    }

    float startX = 100.f;
    float startY = panelY + 15.f;  // Start below the top of the panel with some padding
    float rowGap = 45.f;

    // Calculate maximum scroll offset to prevent scrolling too far
    float totalHeight = unlockedRelics.size() * rowGap;
    float maxScroll = std::max(0.f, totalHeight - panelH + 20.f);
    if (scrollOffset > maxScroll) {
        scrollOffset = maxScroll;
    }
    if (scrollOffset < 0.f) {
        scrollOffset = 0.f;
    }

    // Save the current view so we can restore it later
    sf::View viewport = window.getView();

    sf::Vector2f viewSize = viewport.getSize();  // (800, 600)

    sf::View scissorView(sf::FloatRect({panelX, panelY}, {panelW, panelH}));
    scissorView.setViewport(sf::FloatRect(
        {panelX / viewSize.x,  panelY / viewSize.y},
        {panelW / viewSize.x,  panelH / viewSize.y}
    ));
    window.setView(scissorView);

    for (int i = 0; i < static_cast<int>(unlockedRelics.size()); i++) {
        float yPos = panelY + 15.f + i * rowGap - scrollOffset;

        if (yPos + rowGap < panelY || yPos > panelY + panelH) continue;

        sf::Text relicNameText(fontSmall, std::to_string(i + 1) + ". " + unlockedRelics[i].first, 14);
        relicNameText.setFillColor(sf::Color(230, 220, 190));
        relicNameText.setPosition({startX, yPos});
        window.draw(relicNameText);

        sf::Text relicDescriptionText(fontSmall, unlockedRelics[i].second, 14);
        relicDescriptionText.setFillColor(sf::Color(170, 170, 170));
        relicDescriptionText.setPosition({startX + 20.f, yPos + 18.f});
        window.draw(relicDescriptionText);
    }

    window.setView(viewport);

    // Draw scrollbar
    if (totalHeight > panelH) {
        const float scrollbarX = panelX + panelW + 10.f;
        const float scrollbarW = 8.f;
        const float scrollbarTrackH = panelH;

        // Scrollbar track
        sf::RectangleShape track({scrollbarW, scrollbarTrackH});
        track.setPosition({scrollbarX, panelY});
        track.setFillColor(sf::Color(60, 40, 20, 150));
        window.draw(track);

        // Scrollbar thumb
        float thumbHeight = (panelH / totalHeight) * panelH;
        float thumbY = panelY + (scrollOffset / maxScroll) * (panelH - thumbHeight);

        sf::RectangleShape thumb({scrollbarW, thumbHeight});
        thumb.setPosition({scrollbarX, thumbY});
        thumb.setFillColor(sf::Color(150, 100, 40, 200));
        window.draw(thumb);
    }
}