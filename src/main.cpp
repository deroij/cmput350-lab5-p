#include <cmath>
#include <functional>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

const int FRAME_COUNT = 60;
const int YHEIGHT = 250;

int frame = 0;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
        {
            switch (keyPressed->unicode) {
                case '1':
                    tween = [](float a, float b, float t) { return (1 - t) * a + t * b; };
                    break;
                case '2':
                    tween = [](float a, float b, float t) { return (1 - t * t) * a + (t * t) * b; };
                    break;
                case '3':
                    tween = [](float a, float b, float t) { return (1 - (std::sin((t - 0.5) * M_PI) + 1)/2) * a + ((std::sin((t - 0.5) * M_PI) + 1)/2) * b; };
                    break;
                case '4':
                    tween = [](float a, float b, float t) {
                        float x = 1.0f - t;

                        float n1 = 7.5625f; // Magic numbers I found from online
                        float d1 = 2.75f;
                        float bounce;

                        if (x < 1.0f / d1) {
                            bounce = n1 * x * x;
                        }
                        else if (x < 2.0f / d1) {
                            x -= 1.5f / d1;
                            bounce = n1 * x * x + 0.75f;
                        }
                        else if (x < 2.5f / d1) {
                            x -= 2.25f / d1;
                            bounce = n1 * x * x + 0.9375f;
                        }
                        else {
                            x -= 2.625f / d1;
                            bounce = n1 * x * x + 0.984375f;
                        }

                        float eased = 1.0f - bounce;

                        return (1 - eased) * a + eased * b;
                    };
                    break;
                case '5':
                    tween = [](float a, float b, float t) {
                        float eased;

                        if (t < 0.5f) {
                            eased = 2.0f * t * t;
                        }
                        else {
                            eased = 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
                        }

                        return (1 - eased) * a + eased * b;
                    };
                    break;
                case '6':
                    tween = [](float a, float b, float t) {
                        const float c1 = 1.70158f;
                        const float c3 = c1 + 1.0f;

                        float eased = 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);

                        return (1 - eased) * a + eased * b;
                    };
                    break;
                case '7':
                    tween = [](float a, float b, float t) { 
                        float eased;
                        if (t < 0.3f){
                            eased = t;
                        }
                        else if (t < 0.7f){
                            eased = t/2 + 0.15f;
                        }
                        else if (t < 0.9f) {
                            eased = t*2 - 0.9f;
                        }
                        else {
                            eased = t;
                        }
                        return (1 - eased) * a + eased * b;
                    };
                    break;
                case '8':
                    tween = [](float a, float b, float t) {
                        float eased = t * t * t;
                        return (1 - eased) * a + eased * b;
                    };
                    break;
                case '9':
                    tween = [](float a, float b, float t) {
                            float eased = 1 - (1 - t) * (1 - t);
                            return (1 - eased) * a + eased * b;
                        };
                        break;
                default:
                    break;
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    float t = static_cast<float>(frame) / static_cast<float>(FRAME_COUNT);
    float x = tween(0.0f, static_cast<float>(WINDOW_WIDTH), t);

    // Make circle
    sf::CircleShape circle(25.0f);
    circle.setOrigin({25.0f, 25.0f});
    circle.setPosition({x, static_cast<float>(YHEIGHT)});
    circle.setFillColor(sf::Color::Green);
    window.draw(circle);

    frame = (frame + 1) % FRAME_COUNT;

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    const float graphLeft = 100.0f;
    const float graphRight = 700.0f;
    const float graphTop = 550.0f;
    const float graphBottom = 750.0f;

    // Draw x-axis
    sf::Vertex xAxis[2];

    xAxis[0].position = {graphLeft, graphBottom};
    xAxis[0].color = sf::Color::White;

    xAxis[1].position = {graphRight, graphBottom};
    xAxis[1].color = sf::Color::White;

    window.draw(xAxis, 2, sf::PrimitiveType::Lines);

    // Draw y-axis
    sf::Vertex yAxis[2];

    yAxis[0].position = {graphLeft, graphBottom};
    yAxis[0].color = sf::Color::White;

    yAxis[1].position = {graphLeft, graphTop};
    yAxis[1].color = sf::Color::White;

    window.draw(yAxis, 2, sf::PrimitiveType::Lines);

    // Draw the tween curve
    const int samples = 100;

    for (int i = 0; i < samples; i++) {
        float t1 = static_cast<float>(i) / samples;
        float t2 = static_cast<float>(i + 1) / samples;

        float value1 = tween(0.0f, 1.0f, t1);
        float value2 = tween(0.0f, 1.0f, t2);

        float x1 = graphLeft + t1 * (graphRight - graphLeft);
        float x2 = graphLeft + t2 * (graphRight - graphLeft);

        float y1 = graphBottom - value1 * (graphBottom - graphTop);
        float y2 = graphBottom - value2 * (graphBottom - graphTop);

        sf::Vertex line[2];

        line[0].position = {x1, y1};
        line[0].color = sf::Color::Yellow;

        line[1].position = {x2, y2};
        line[1].color = sf::Color::Yellow;

        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // Draw the current position on the curve
    float currentValue = tween(0.0f, 1.0f, t);

    float currentX =
        graphLeft + t * (graphRight - graphLeft);

    float currentY =
        graphBottom - currentValue * (graphBottom - graphTop);

    sf::CircleShape graphDot(6.0f);
    graphDot.setOrigin({6.0f, 6.0f});
    graphDot.setPosition({currentX, currentY});
    graphDot.setFillColor(sf::Color::Red);

    window.draw(graphDot);

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
