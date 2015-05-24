#include "Sprite.h"
#include "Planet.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <set>

#include <gl/glu.h>

int main()
{
	::srand((unsigned)::time(nullptr));

	sf::ContextSettings settings(32); // Depth bits.
	sf::RenderWindow window(sf::VideoMode(1024, 768, 32), "Wombat", sf::Style::Default, settings);
	window.setFramerateLimit(30);

	Planet planet;
	sf::Clock clock;

	glEnable(GL_DEPTH_TEST);

	int nFrames = 0;
	double tPeriod = 0;

	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Escape)
			       window.close();
				else 
					planet.OnKeyPressed(event.key.code);
        }

	    double tDelta = clock.restart().asSeconds();
		tPeriod += tDelta;
		++nFrames;
		if (tPeriod >= 1)
		{
			std::cout << int(nFrames / tPeriod) << " fps" << std::endl;
			nFrames = 0;
			tPeriod = 0;
		}

		//sprite.Update(tDelta.asSeconds());
		planet.Update(tDelta);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto sz = window.getSize();
		glViewport(0, 0, sz.x, sz.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, sz.x / (double)sz.y, 0.1, 1000);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		planet.Draw(window);

		window.display();
    }

    return EXIT_SUCCESS;
}
