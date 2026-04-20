#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    std::cout << "test1" << std::endl;
    
    sf::RenderWindow window(sf::VideoMode({800, 600}), "The Catacombs");
    
    std::cout << "testbien" << std::endl;
    std::cout << "Window is open: " << window.isOpen() << std::endl;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();

    }


    return 0;
}