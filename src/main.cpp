/**
 * NeonDrift - Futuristic 2D Arcade Racing Game
 * Main entry point
 */

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main()
{
    // Window settings
    const unsigned int WINDOW_WIDTH = 1280;
    const unsigned int WINDOW_HEIGHT = 720;
    const std::string WINDOW_TITLE = "Neon Drift";
    
    // Create the game window
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        WINDOW_TITLE,
        sf::Style::Close | sf::Style::Titlebar
    );
    window.setFramerateLimit(60);
    
    // Main game loop
    while (window.isOpen())
    {
        // Event handling
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            
            // Escape to close
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }
        
        // Clear with dark background (deep purple)
        window.clear(sf::Color(15, 5, 25));
        
        // Display
        window.display();
    }
    
    return 0;
}
