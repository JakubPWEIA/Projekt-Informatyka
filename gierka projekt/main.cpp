#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <windows.h>

using namespace std;

class Przycisk {
public:
	sf::RectangleShape prostokat;
	sf::Text tekst;
	bool klikniety = false;

	Przycisk(float x, float y, float szerokosc, float wysokosc, sf::Font& font, const string& napis) {
		prostokat.setSize(sf::Vector2f(szerokosc, wysokosc));
		prostokat.setPosition(x, y);
		prostokat.setFillColor(sf::Color::Black);

		tekst.setFont(font);
		tekst.setString(napis);
		tekst.setCharacterSize(24);
		tekst.setFillColor(sf::Color::White);
		tekst.setPosition(x + (szerokosc - tekst.getGlobalBounds().width) / 2, y + (wysokosc - tekst.getGlobalBounds().height) / 2);
	}
	bool sprawdzKlikniecie(const sf::Vector2i& mousePos) {
		if (prostokat.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
			return true;
		}
		return false;
	}

	
	void rysuj(sf::RenderWindow& window) {
		window.draw(prostokat);
		window.draw(tekst);
	}
};
class Gwiazda {
public:
	sf::CircleShape shape;
	float predkosc;

	Gwiazda(float x, float y, float predkosc) {
		shape.setRadius(2.f);
		shape.setFillColor(sf::Color::White);
		shape.setPosition(x, y);
		this->predkosc = predkosc;
	}
	
	void update() {
		shape.move(0.f, predkosc);
		if (shape.getPosition().y > 600) {
			shape.setPosition(rand() % 800, 0);
		}
	}
};
class Asteroida{
public:
	sf::Sprite sprite;
	float predkosc;

	Asteroida(float x, float y, float predkosc,sf::Texture& tekstura) {
		sprite.setTexture(tekstura);
		sprite.setPosition(x, y);
		this->predkosc = predkosc;
		sprite.setScale(0.2f, 0.2f);
	}

	void update() {
		sprite.move(0.f, predkosc);
		if (sprite.getPosition().y > 600) {
			sprite.setPosition(rand() % 800, 0);
		}
	}
};

class Gracz: public sf::Sprite {

public:
	sf::Sprite sprite;
	float graczx;
	float graczy;
	Gracz(sf::Texture& tekstura, float x, float y) {
		sprite.setTexture(tekstura);
		sprite.setScale(0.2f, 0.2f);
		float srodekX = 800.f / 2;
		float srodekY = 600.f / 2;
		sprite.setPosition(srodekX-50.f , srodekY+160.f );
		graczx = sprite.getPosition().x;
		graczy = sprite.getPosition().y;

	}
	void graczdraw(sf::RenderWindow& window) {
		window.draw(sprite);
	}
	void ruchprawo() {
		sprite.move(5.f,0.f);
	}
	void ruchlewo() {
		sprite.move(-5.f, 0.f);
	}
	void pozycja() {
		graczx = sprite.getPosition().x;
		graczy = sprite.getPosition().y;
	}
};

class Wrog : public sf::Sprite {
public:

	Wrog(sf::Texture& tekstura, float x, float y) {
		setTexture(tekstura);
		setPosition(x, y);
		setScale(0.3f, 0.3f);
	}
	


};
class Poziom {
public:
	vector<Wrog> wrogowie;

	Poziom(sf::Texture& tekstura,int rows) {
		int cols = 8;
		float startx = 100.f;
		float starty = 50.f;
		float spacingx = 70.f;
		float spacingy = 70.f;
		


		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {
				float x = startx + i * spacingx;
				float y = starty + j * spacingy;
				wrogowie.emplace_back(tekstura, x, y);
			}
		}
	}
	void drawlvl(sf::RenderWindow& window){
		for (auto& wrog : wrogowie) {
			window.draw(wrog);
		}
	}
};
class Pocisk :public sf::Sprite {
public:
	sf::Sprite sprite;
	float speed;


	Pocisk(float x, float y, sf::Texture& tekstura, float speed) {
		sprite.setTexture(tekstura);
		sprite.setPosition(x, y);
		this->speed = speed;
	}

	void updatepocisk() {
		sprite.move(0.f, -speed);
	}


};

void kolizja(vector<Pocisk>& pociski, vector<Wrog>& wrogowie,int& punkciory) {
	for (auto pocisk = pociski.begin(); pocisk != pociski.end();) {
		bool kolizja = false;
		for (auto wrog = wrogowie.begin(); wrog != wrogowie.end();) {
			if (pocisk->sprite.getGlobalBounds().intersects(wrog->getGlobalBounds())) {
				wrog = wrogowie.erase(wrog);
				kolizja = true;
				punkciory++;
				
			}
			else
			{
				++wrog;
			}
		}

		if (kolizja) {
			pocisk = pociski.erase(pocisk);
		}
		else {
			++pocisk;
		}
	}
	
}

int main()
{
	 int punkciory = 0;
	string punktytekst;
	sf::Sprite eksplozja;
	sf::Text gameover;
	sf::Font arial;
	arial.loadFromFile("C:\\Users\\jakub\\source\\repos\\gierka projekt\\arial.ttf");
	gameover.setFont(arial);
	gameover.setString("Game Over");
	gameover.setCharacterSize(80);
	gameover.setFillColor(sf::Color::Red);
	gameover.setPosition(120.f, 300.f);
	sf::Text punkty;
	punkty.setFont(arial);
	punkty.setFillColor(sf::Color::White);

	vector<Gwiazda> gwiazdy;
	vector<Asteroida> asteroidy;

	vector<Pocisk> pociski;
	
	bool shoot = true;
	float cooldown = 0.5f;
	sf::Clock shootTimer;

	for (int i = 0; i < 60; i++) {
		float x = rand() % 800;
		float y = rand() % 600;
		float speed = rand() % 2 + 1;
		gwiazdy.emplace_back(x, y, speed);
	}
	

	sf::RenderWindow window(sf::VideoMode(800, 600), "NajeŸdczcy z kosmosu !");
	window.setFramerateLimit(60);
	
	sf::Texture teksturawrog;
	teksturawrog.loadFromFile("C:\\Users\\jakub\\source\\repos\\gierka projekt\\alien.png");
	
	sf::Texture teksturagracz;
	teksturagracz.loadFromFile("C:\\Users\\jakub\\source\\repos\\gierka projekt\\statek.png");
	
	sf::Texture tlo;
	tlo.loadFromFile("C:\\Users\\jakub\\source\\repos\\gierka projekt\\tlo.png");
	
	sf::Sprite tlogry;
	tlogry.setTexture(tlo);
	
	sf::Texture asteroidatekstura;
	asteroidatekstura.loadFromFile("C:\\Users\\jakub\\source\\repos\\gierka projekt\\asteroida.png");
	
	sf::Texture pocisktekstura;
	pocisktekstura.loadFromFile("C:\\Users\\jakub\\source\\repos\\gierka projekt\\siatka.png");

	sf::Texture eksplozjastatku;
	eksplozjastatku.loadFromFile("C:\\Users\\jakub\\source\\repos\\gierka projekt\\eksplozja.png");
	eksplozja.setTexture(eksplozjastatku);
	
	for (int i = 0; i < 5; i++) {
		float x = rand() % 800;
		float y = rand() % 600;
		float speed = rand() % 1 + 1;
		asteroidy.emplace_back(x, y, speed,asteroidatekstura);
	}
	Poziom jeden(teksturawrog,1);
	Poziom dwa(teksturawrog, 2);
	Poziom trzy(teksturawrog, 3);

	Gracz G1(teksturagracz,50.f,20.f);

	Przycisk p1(200.f, -300.f, 200.f, 100.f, arial, "dupa");

	sf::Clock clock;
	
	bool gracz = true;


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		float roznicaczasu = clock.restart().asSeconds();

		if (gracz) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				G1.ruchprawo();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				G1.ruchlewo();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shoot) {
				G1.pozycja();
				pociski.emplace_back(G1.graczx + 20.f, G1.graczy - 30.f, pocisktekstura, 5.f);

				shoot = false;
				shootTimer.restart();

			}
			if (!shoot && shootTimer.getElapsedTime().asSeconds() >= cooldown) {
				shoot = true;
			}
			for (auto& pocisk : pociski) {
				pocisk.updatepocisk();
			}
		}
		for (auto& asteroida : asteroidy) {
			if (asteroida.sprite.getGlobalBounds().intersects(G1.sprite.getGlobalBounds())) {
				gracz = false;
				
			}
		}

		
		
		kolizja(pociski, trzy.wrogowie,punkciory);
		punktytekst = to_string(punkciory);
		punkty.setString(punktytekst);

		window.clear();
		window.draw(tlogry);
		window.draw(punkty);
		
		for (auto& gwiazda : gwiazdy) {
			gwiazda.update();
			window.draw(gwiazda.shape);
		}

		for (auto& asteroida : asteroidy) {
			asteroida.update();
			window.draw(asteroida.sprite);
		}
		
		trzy.drawlvl(window);
		if (gracz) {
			G1.graczdraw(window);
		}
		else {
			G1.pozycja();
			eksplozja.setPosition(G1.graczx-60.f,G1.graczy-40.f);
			window.draw(eksplozja);
			window.draw(gameover);
			
			
			
		}
		for (const auto& pocisk : pociski) {
			window.draw(pocisk.sprite);
		}

		p1.rysuj(window);
		
		
		
		window.display();
	}

		

	return 0;
}