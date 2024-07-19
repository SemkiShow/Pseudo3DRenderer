#include <SFML/Graphics.hpp>
using namespace sf;

RectangleShape rectangle(Vector2f(0, 0));

int main()
{
    auto window = RenderWindow{ { 576u, 324u }, "Pseudo3DShooter" };
    window.setFramerateLimit(144);
    rectangle.setFillColor(Color(127, 127, 255));

    while (window.isOpen())
    {
        for (auto event = Event{}; window.pollEvent(event);)
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        rectangle.setPosition(10, 324 - 50);
        window.draw(rectangle);
        rectangle.setSize(Vector2f(1, 100));
        rectangle.setPosition(20, 324 - 100);
        window.draw(rectangle);

        // window.clear();
        window.display();
    }
}
