#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <limits>


const float PADDLE_WIDTH = 10.f;
const float PADDLE_HEIGHT = 100.f;
const float BALL_RADIUS = 10.f;
const float WINDOW_WIDTH = 800.f;
const float WINDOW_HEIGHT = 600.f;

class Paddle {
public:
    sf::RectangleShape shape;
    float speed;

    Paddle(float x) {
        shape.setSize({ PADDLE_WIDTH, PADDLE_HEIGHT });
        shape.setPosition(x, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2);
        shape.setFillColor(sf::Color::White);
        speed = 0.1;
    }

    void move(float dy) {
        shape.move(0, dy);
        if (shape.getPosition().y < 0)
            shape.setPosition(shape.getPosition().x, 0);
        if (shape.getPosition().y + PADDLE_HEIGHT > WINDOW_HEIGHT)
            shape.setPosition(shape.getPosition().x, WINDOW_HEIGHT - PADDLE_HEIGHT);
    }
};

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Ball() {
        shape.setRadius(BALL_RADIUS);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        velocity = { -0.1, -0.1 };
    }

    void reset() {
        shape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        velocity = { -0.1, -0.1 };
    }

    void update() {
        shape.move(velocity);

        // Bounce off top/bottom
        if (shape.getPosition().y <= 0 || shape.getPosition().y + BALL_RADIUS * 2 >= WINDOW_HEIGHT)
            velocity.y = -velocity.y;
    }

    sf::FloatRect getBounds() {
        return shape.getGlobalBounds();
    }
};

void showMenu();

void runGame() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");

    Paddle player(20.f);
    Paddle ai(WINDOW_WIDTH - 30.f);
    Ball ball;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        // Player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player.move(-player.speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player.move(player.speed);

        // AI movement
        if (ball.shape.getPosition().y + BALL_RADIUS > ai.shape.getPosition().y + PADDLE_HEIGHT / 2)
            ai.move(ai.speed * 0.6f);
        else
            ai.move(-ai.speed * 0.6f);

        ball.update();

        // Collision with paddles
        if (ball.getBounds().intersects(player.shape.getGlobalBounds()) ||
            ball.getBounds().intersects(ai.shape.getGlobalBounds())) {
            ball.velocity.x = -ball.velocity.x;
        }

        // Ball out of bounds
        if (ball.shape.getPosition().x <= 0 || ball.shape.getPosition().x >= WINDOW_WIDTH)
            ball.reset();

        // Draw everything
        window.clear(sf::Color::Black);
        window.draw(player.shape);
        window.draw(ai.shape);
        window.draw(ball.shape);
        window.display();
    }

    showMenu(); // recursive call to show menu again
}

void handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        std::cout << "Starting game...\n";
        runGame();
        break;
    case 2:
        std::cout << "Exiting program...\n";
        return;
    default:
        std::cout << "Invalid choice. Try again.\n";
        showMenu();
        break;
    }
}

void showMenu() {
    int choice;
    std::cout << "\n===== PONG GAME MENU =====\n";
    std::cout << "1. Play Game\n";
    std::cout << "2. Exit\n";
    std::cout << "Choose an option: ";
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Try again.\n";
        showMenu();
    }
    else {
        handleMenuChoice(choice);
    }
}

int main() {
    showMenu();
    return 0;
}
