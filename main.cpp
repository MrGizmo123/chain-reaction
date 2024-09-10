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
#include <string>
#include <vector>
#include <cmath>

#define WIDTH 480
#define HEIGHT 1000

#define GRID_WIDTH 7
#define GRID_HEIGHT 15

#define FONT "Iosevka-Medium.ttf"

using namespace std;
using namespace sf;

struct Cell
{
    int num_particles;
    Color color;
};

class ParticleGrid
{
    int width;
    int height;
    Cell* particles;

public:
    
    Cell& get(int x, int y)
	{	
	    return particles[y * width + x];
	}

    int getWidth() { return width; }
    int getHeight() { return height; }

    ~ParticleGrid()
	{
	    delete particles;
	}
    
    ParticleGrid(int _w = GRID_WIDTH, int _h = GRID_HEIGHT) : width(_w), height(_h)
	{
	    particles = new Cell[width * height];
	
	    for (int i=0;i<width;i++)
	    {
		for (int j=0;j<height;j++)
		{
		    this->get(i, j).num_particles = 0;
		    this->get(i,j).color = Color::Black;
		}
	    }
	}

    void add(int x, int y, Color color)
	{
	    bool is_on_top_edge = (y == 0);
	    bool is_on_bottom_edge = (y == height-1);
	    bool is_on_left_edge = (x == 0);
	    bool is_on_right_edge = (x == width-1);

	    int max_particles = 3;
	    max_particles -= ((is_on_left_edge || is_on_right_edge) ? 1 : 0);
	    max_particles -= ((is_on_top_edge || is_on_bottom_edge) ? 1 : 0);

	    if ((this->get(x,y).num_particles + 1) > max_particles) // explode
	    {
		this->get(x, y).num_particles = 0;
	    
		if (!is_on_top_edge)
		{
		    this->add(x, y-1, color);
		}
		if (!is_on_bottom_edge)
		{
		    this->add(x, y+1, color);
		}
		if (!is_on_left_edge)
		{
		    this->add(x-1, y, color);
		}
		if (!is_on_right_edge)
		{
		    this->add(x+1, y, color);
		}
	    }
	    else {
		this->get(x,y).num_particles++;
		this->get(x,y).color = color;
	    }
	}
    
};

class VisualGrid
{
    Vector2i padding;
    Vector2i size;
    Vector2f cell_size;
    Vector2i screen_size;
    float stroke_width;
    Color color;
    vector<RectangleShape> lines;
    
public:
    VisualGrid(Vector2i _size              =  Vector2i(GRID_WIDTH, GRID_HEIGHT),
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

    void render(RenderWindow& win, ParticleGrid& particles, Font& f)
    {
	for (RectangleShape& line : lines)
	{
	    line.setFillColor(color);
	    win.draw(line);
	}

	Text text;
	text.setFont(f);
	text.setCharacterSize(24);
	
	for (int i=0;i<particles.getWidth();i++)
	{
	    for (int j=0;j<particles.getHeight();j++)
	    {
		Cell& cell = particles.get(i, j);

		if (cell.num_particles == 0)
		{
		    continue;
		}
		
		text.setFillColor(cell.color);
		text.setString(to_string(cell.num_particles));

		text.setPosition(Vector2f(cell_size.x * (i + 0.5f), cell_size.y * (j + 0.5f)));
		win.draw(text);
	    }
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

Color current_col = Color::Red;
VisualGrid g;
ParticleGrid g1;

void mouse_clicked(Vector2i pos)
{
    Vector2i grid_coord = g.getGridCoord(pos);

    //cout << grid_coord.x << ", " << grid_coord.y << endl;

    if (g1.get(grid_coord.x, grid_coord.y).color != current_col && g1.get(grid_coord.x, grid_coord.y).num_particles > 0)
    {
	return;
    }
    g1.add(grid_coord.x, grid_coord.y, current_col);
    
    
    if (current_col == Color::Green)
    {
	g.setColor(Color::Red);
	current_col = Color::Red;
    }
    else
    {
	g.setColor(Color::Green);
	current_col = Color::Green;
    }
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

    Font font;
    if (!font.loadFromFile(FONT))
    {
        cerr << "Error loading font" << endl;
    }
    
    while (window.isOpen())
    {
	check_events(window, mouse_down);

	window.clear();
	g.render(window, g1, font);
	window.display();
    }

    return 0;
}
