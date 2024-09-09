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
#include <vector>

#define WIDTH 480
#define HEIGHT 1000

using namespace std;
using namespace sf;

class Grid
{
    Vector2i padding;
    Vector2i size;
    Vector2i screen_size;
    float stroke_width;
    Color color;
    vector<RectangleShape> lines;
    
public:
    Grid(Vector2i _size              =  Vector2i(7,15),
	 Vector2i _screen_size       =  Vector2i(WIDTH, HEIGHT),
	 Vector2i _padding           =  Vector2i(10, 10),
	 float _stroke_width         =  1,
	 Color _color                =  Color::Red)
	
	: size(_size),
	  screen_size(_screen_size),
	  padding(_padding),
	  stroke_width(_stroke_width),
	  color(_color)
	  
    {
	float x_inc = (int)(screen_size.x - 2*padding.x) / (float)size.x;
	float y_inc = (int)(screen_size.y - 2*padding.y) / (float)size.y;
	    
	for (int i = 0; i <= _size.x; i++)
	{
	    RectangleShape line(Vector2f(stroke_width, screen_size.y - 2*padding.y));
	    line.move(Vector2f(padding.x + x_inc * i, padding.y));
	    lines.push_back(line);
	}

	for (int i = 0; i <= _size.y; i++)
	{
	    RectangleShape line(Vector2f(screen_size.x - 2*padding.x, stroke_width));
	    line.move(Vector2f(padding.x, padding.y + y_inc * i));
	    lines.push_back(line);
	}
    }

    void render(RenderWindow& win)
    {
	for (RectangleShape& line : lines)
	{
	    line.setFillColor(color);
	    win.draw(line);
	}
    }

    void setColor(Color c)
    {
	color = c;
    }
    
};

int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Test Window");

    Grid g;

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
	g.render(window);
	window.display();
    }

    return 0;
}
