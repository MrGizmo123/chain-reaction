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
#include <cmath>

#define WIDTH 480
#define HEIGHT 1000

using namespace std;
using namespace sf;

class Grid
{
    Vector2i padding;
    Vector2i size;
    Vector2f cell_size;
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

	cell_size.x = x_inc;
	cell_size.y = y_inc;
	    
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

    Vector2i getGridCoord(Vector2i pos)
    {
	int x = floor((pos.x - padding.x) / cell_size.x);
	int y = floor((pos.y - padding.y) / cell_size.y);

	return Vector2i(x, y);
    }

    void setColor(Color c)
    {
	color = c;
    }
    
};

bool color = true;
Grid g;

void mouse_clicked(Vector2i pos)
{
    cout << pos.x  << ", " << pos.y << endl;
    if (color)
    {
	g.setColor(Color::Red);
    }
    else
    {
	g.setColor(Color::Green);
    }
    color = !color;
}

void check_events(RenderWindow& window, bool& mouse_down)
{
    Event ev;
    
    while (window.pollEvent(ev))
    {
	if (ev.type == Event::Closed)
	{
	    window.close();
	}

	if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left && !mouse_down)
	{
	    // mouse clicked

	    mouse_down = true;

	    mouse_clicked(Mouse::getPosition(window));
	}

	if (ev.type == Event::MouseButtonReleased && ev.mouseButton.button == Mouse::Left && mouse_down)
	{
	    mouse_down = false;
	}
    }
}

int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Chain Reaction");
    
    bool mouse_down = false;

    while (window.isOpen())
    {
	check_events(window, mouse_down);

	window.clear();
	g.render(window);
	window.display();
    }

    return 0;
}
