#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

using namespace sf;

class Particle : public CircleShape{
private:
    bool activeSelf;
public:
    bool getActiveState() {
        return activeSelf;
    }
    void setActive(bool newState) {
        activeSelf = newState;
    }
};

class ParticleSystem{
private:
    std::vector<CircleShape> particles;
    RenderWindow *window;
    std::mt19937 *random_engine;
    std::uniform_int_distribution<int> *color_generator;
    std::uniform_int_distribution<int> *pos_generator;

public:
    ParticleSystem(RenderWindow* window, size_t size = 150):window(window) {
        random_engine = new std::mt19937(time(NULL));
        color_generator = new std::uniform_int_distribution<int>(0, 255);
        pos_generator = new std::uniform_int_distribution<int>(0, 1000);

        particles.resize(size);

        for (auto& particle : particles) {
            particle.setPointCount(15);
            particle.setFillColor(Color((* color_generator)(random_engine), (*color_generator)(random_engine), (*color_generator)(random_engine)));
            particle.setPosition((*pos_generator)(random_engine), (*pos_generator)(random_engine));
            particle.setRadius(10);
        }
    }

    ~ParticleSystem() {
        delete random_engine;
        delete color_generator;
        delete pos_generator;
    }

    void draw() {
        for (const auto& particle : particles) {
            window->draw(particle);
        }
    }
};

int main()
{
    RenderWindow *window = new RenderWindow(VideoMode(1000, 1000), "SFML works!");
    ParticleSystem part(window,150);
    while (window->isOpen())
    {
        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                window->close();
        }
        window->clear();
        part.draw();
        window->display();
    }

    return 0;
}