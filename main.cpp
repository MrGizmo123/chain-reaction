// (defkeybind "C-c C-c" (async-shell-command "make"))
// (defkeybind "C-c C-r" (async-shell-command "./game"))

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Grid
{
    Vector2i size;
    float stroke_width;
    Color color;

public:
    Grid(Vector2i _size=Vector2i(10,30), float _stroke_width=1, Color _color=Color::Red)
	: size(_size),
	  stroke_width(_stroke_width),
	  color(_color)
	{}
    
};

int main()
{
    RenderWindow window(VideoMode(480, 1000), "Test Window");

    CircleShape circle(75.0f);

    RectangleShape line(Vector2f(10, 100));

    line.rotate(-45);
    line.move(Vector2f(100,100));

    circle.setFillColor(Color::Blue);

    while (window.isOpen())
    {
	Event ev;

	while (window.pollEvent(ev))
	{
	    if (ev.type == Event::Closed)
	    {
		window.close();
	    }
	}

	window.clear();
	window.draw(circle);
	window.draw(line);
	window.display();
    }

    return 0;
}
