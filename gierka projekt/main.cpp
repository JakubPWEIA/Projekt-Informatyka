#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <windows.h>
#include <fstream>
using namespace std;

void wycentrujTekst(sf::Text& text, const sf::RectangleShape& rectangle) {
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.width / 2, textBounds.height / 2);
	text.setPosition(
		rectangle.getPosition().x + rectangle.getSize().x / 2,
		rectangle.getPosition().y + rectangle.getSize().y / 2
	);
}

class Przycisk {
public:
	sf::RectangleShape prostokat;
	sf::Text tekst;
	bool klikniety = false;

	Przycisk(float x, float y, float szerokosc, float wysokosc, sf::Font& font, const string& napis, sf::Color kolor) {
		prostokat.setSize(sf::Vector2f(szerokosc, wysokosc));
		prostokat.setPosition(x, y);
		prostokat.setFillColor(kolor);
		prostokat.setOutlineThickness(5);
		prostokat.setOutlineColor(sf::Color::White);
		tekst.setFont(font);
		tekst.setString(napis);
		tekst.setCharacterSize(24);
		tekst.setFillColor(sf::Color::White);
		
		wycentrujTekst(tekst, prostokat);
	}
	bool sprawdzKlikniecie(const sf::Vector2i& mousePos, const sf::Event& event) {
		if (prostokat.getGlobalBounds().contains(sf::Vector2f(mousePos)) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
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
class Asteroida:public sf::Sprite{
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
		this->setTexture(tekstura);
		this->setPosition(x, y);
		this->setScale(0.3f, 0.3f);
	}
	


};
class Poziom {
private:
	vector<Wrog> wrogowie;
	int numer;
public:
	bool zakonczony;

	Poziom(int numerpoziomu) {
		numer = numerpoziomu;
		zakonczony = false;
	}

	void utworzWrogow(sf::Texture& tekstura) {
		wrogowie.clear();
		int rows = 3;
		int cols = 8;
		float startx = 100.f;
		float starty = 50.f;
		float spacingx = 70.f;
		float spacingy = 70.f;

		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < numer; j++) {
				float x = startx + i * spacingx;
				float y = starty + j * spacingy;
				wrogowie.emplace_back(tekstura,x, y);
			}
		}
		
	}

	void drawlvl(sf::RenderWindow& window) {

		
		for (auto& wrog : wrogowie) {
			window.draw(wrog);
		}
	}

	bool poziomzakonczony() {
		return wrogowie.empty();
	}

	vector<Wrog>& zwrocwrog(){
		return wrogowie;
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
void kolizjaasteroidy(vector<Pocisk>& pociski, vector<Asteroida>& asteroidy,int& punkciory) {
	for (auto pocisk = pociski.begin(); pocisk != pociski.end();) {
		bool kolizja = false;
		for (auto asteroida = asteroidy.begin(); asteroida != asteroidy.end();) {
			if (pocisk->sprite.getGlobalBounds().intersects(asteroida->sprite.getGlobalBounds())) {
				
				asteroida = asteroidy.erase(asteroida);
				kolizja = true;
				punkciory += 10;

			}
			else
			{
				++asteroida;
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
void EkranPomocy(sf::RenderWindow& window, sf::Font& font) {
	sf::Text instrukcje;
	instrukcje.setFont(font);
	instrukcje.setString("POMOC:\nUnikaj asteroid, aby przetrwac!\n"
		"Strzelaj do kosmitow za pomoca SPACJI.\n"
		"Poruszaj rakieta strzalkami w LEWO i PRAWO.\n"
		"Zniszcz wszystkich kosmitow, aby wygrac!");
	instrukcje.setCharacterSize(24);
	instrukcje.setFillColor(sf::Color::White);

	
	sf::FloatRect textBounds = instrukcje.getLocalBounds();
	instrukcje.setOrigin(textBounds.width / 2, textBounds.height / 2);
	instrukcje.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	
	window.clear(sf::Color::Black);
	window.draw(instrukcje);
	window.display();

	
}
void EkranPoziom(sf::RenderWindow& window, sf::Font& font,int poziomteraz) {
	sf::Text poziom;
	poziom.setFont(font);
	poziom.setString("POZIOM "+to_string(poziomteraz+1));
	poziom.setCharacterSize(24);
	poziom.setFillColor(sf::Color::White);


	sf::FloatRect textBounds = poziom.getLocalBounds();
	poziom.setOrigin(textBounds.width / 2, textBounds.height / 2);
	poziom.setPosition(window.getSize().x / 2, window.getSize().y / 2);


	window.clear(sf::Color::Black);
	window.draw(poziom);
	window.display();
	sf::sleep(sf::seconds(3));
	window.clear(sf::Color::Black);
}


int main()
{
	vector<Poziom> poziomy;
	int poziomteraz = 0;
	int stangry = 3;
	/*stangry = 0 - menu poczatkowe
	  stangry = 1 - gra sie toczy
	  stangry = 2 - escape
	  stangry = 3 - gra sie skonczyla
	  stangry = 4 - nick gracza moze - udalo sie 8)
	  stangry = 5 - jednak highscores 
	*/
	int stangryback = 0;

	int punkciory = 0;
	string punktytekst;
	sf::Sprite eksplozja;
	sf::Text gameover;
	sf::Font arial;
	sf::Text koniecnapis;
	sf::Text tytul;
	sf::Text pauza;
	arial.loadFromFile("zasoby\\arial.ttf");
	gameover.setFont(arial);
	gameover.setString("Game Over");
	gameover.setCharacterSize(80);
	gameover.setFillColor(sf::Color::Red);
	gameover.setPosition(120.f, 300.f);
	sf::Text punkty;
	punkty.setFont(arial);
	punkty.setFillColor(sf::Color::White);
	koniecnapis.setFont(arial);
	koniecnapis.setString("Koniec Gry !");
	koniecnapis.setCharacterSize(80);
	koniecnapis.setFillColor(sf::Color::White);
	koniecnapis.setPosition(120.f, 100.f);
	sf::Text przegrales;
	przegrales.setFont(arial);
	przegrales.setString("Przegrales !");
	przegrales.setCharacterSize(80);
	przegrales.setFillColor(sf::Color::White);
	przegrales.setPosition(120.f, 100.f);

	tytul.setFont(arial);
	tytul.setString("Najezdzcy \n\t  z \n kosmosu");
	tytul.setCharacterSize(50);
	tytul.setFillColor(sf::Color::White);
	tytul.setPosition(240.f, 100.f);

	pauza.setFont(arial);
	pauza.setString("Opuscic \n  Gre?");
	pauza.setCharacterSize(80);
	pauza.setFillColor(sf::Color::White);
	pauza.setPosition(220.f, 100.f);

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
	teksturawrog.loadFromFile("zasoby\\alien.png");

	sf::Texture teksturagracz;
	teksturagracz.loadFromFile("zasoby\\statek.png");

	sf::Texture tlo;
	tlo.loadFromFile("zasoby\\tlo.png");

	sf::Sprite tlogry;
	tlogry.setTexture(tlo);

	sf::Texture asteroidatekstura;
	asteroidatekstura.loadFromFile("zasoby\\asteroida.png");

	sf::Texture pocisktekstura;
	pocisktekstura.loadFromFile("zasoby\\siatka.png");

	sf::Texture eksplozjastatku;
	eksplozjastatku.loadFromFile("zasoby\\eksplozja.png");
	eksplozja.setTexture(eksplozjastatku);



	for (int i = 0; i < 3; i++) {
		float x = rand() % 800;
		float y = rand() % 600;
		float speed = rand() % 1 + 1;
		asteroidy.emplace_back(x, y, speed, asteroidatekstura);
	}

	poziomy.emplace_back(3);
	poziomy[0].utworzWrogow(teksturawrog);
	poziomy.emplace_back(4);
	poziomy[1].utworzWrogow(teksturawrog);
	poziomy.emplace_back(5);
	poziomy[2].utworzWrogow(teksturawrog);

	Gracz G1(teksturagracz, 50.f, 20.f);

	Przycisk graj(260.f, 350.f, 265.f, 100.f, arial, "Rozpocznij \n\tGre",sf::Color::Black);
	Przycisk koniec(260.f, 450.f, 265.f, 100.f, arial, "Wyjdz",sf::Color::Black);
	Przycisk escape(260.f, 450.f, 265.f, 100.f, arial, "Wyjdz",sf::Color::Black);
	Przycisk kontynuuj(260.f, 300.f, 265.f, 100.f, arial, "Kontynuuj",sf::Color::Black);
	Przycisk x(750.f, 9.f, 40.f, 40.f, arial, "X",sf::Color::Red);
	Przycisk highscores(260.f, 300.f, 265.f, 100.f, arial, "High Scores", sf::Color::Black);
	sf::Clock clock;

	bool gracz = true;

	string nick;
	sf::Text tekst;
	tekst.setFont(arial);
	tekst.setCharacterSize(24);
	tekst.setPosition(270.f, 100.f);
	bool wcisnietyklawisz = false;
	sf::Text podajnick;
	podajnick.setFont(arial);
	podajnick.setCharacterSize(30);
	podajnick.setString("Podaj nick:");
	podajnick.setPosition(270.f, 20.f);

	sf::Text enter;
	enter.setFont(arial);
	enter.setCharacterSize(30);
	enter.setString("Wcisnij ENTER aby rozpoczac");
	enter.setPosition(140.f, 200.f);

	//PLIK
	ofstream wyniki;
	wyniki.open("wyniki.txt",ios::app);
	string linijka;
	vector <string> wektorwyniki;
	ifstream wynikiodczyt("wyniki.txt");
	while (getline(wynikiodczyt, linijka)) {
		wektorwyniki.push_back(linijka);
	}
	vector<sf::Text> texts;

	for (int i = 0; i < wektorwyniki.size();i++) {
		sf::Text wynikitekst;
		wynikitekst.setFont(arial);
		wynikitekst.setString(to_string(i + 1) + "." + wektorwyniki[i]);
		wynikitekst.setCharacterSize(20);
		wynikitekst.setFillColor(sf::Color::White);
		wynikitekst.setPosition(10.f, 30.f * i + 10.f);
		texts.push_back(wynikitekst);
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		

		float roznicaczasu = clock.restart().asSeconds();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
			EkranPomocy(window, arial);
			continue;
		}
		window.clear();
		if (stangry == 0) {
			for (auto& gwiazda : gwiazdy) {
				gwiazda.update();
				window.draw(gwiazda.shape);
			}
			graj.rysuj(window);
			window.draw(tytul);
			if (graj.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				EkranPoziom(window, arial, poziomteraz);
				stangry = 1;
				window.clear();
				stangryback = 1;
			}

		}
		punktytekst = to_string(punkciory);
		punkty.setString(punktytekst);
		if (stangry == 1) {
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
			poziomy[poziomteraz].drawlvl(window);
			kolizja(pociski, poziomy[poziomteraz].zwrocwrog(), punkciory);
			kolizjaasteroidy(pociski, asteroidy, punkciory);
			if (asteroidy.empty()) {
				for (int i = 0; i < 3; i++) {
					float x = rand() % 800;
					float y = -50;
					float speed = rand() % 1 + 1;
					asteroidy.emplace_back(x, y, speed, asteroidatekstura);
				}
			}

			// Jeœli wszyscy wrogowie zostali pokonani na aktualnym poziomie
			if (poziomy[poziomteraz].zwrocwrog().empty()) {
				poziomteraz++; // PrzejdŸ do nastêpnego poziomu

				if (poziomteraz < poziomy.size()) {
					EkranPoziom(window, arial, poziomteraz);
					poziomy[poziomteraz].utworzWrogow(teksturawrog); // Twórz wrogów dla nowego poziomu
				}
				else {
					stangry = 3; // Gra zakoñczona, wszystkie poziomy ukoñczone
				}
			}
			if (gracz) {
				G1.graczdraw(window);
			}
			else {
				G1.pozycja();
				eksplozja.setPosition(G1.graczx - 60.f, G1.graczy - 40.f);
				window.draw(eksplozja);
				window.draw(gameover);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					stangry = 6;
					wyniki << nick <<" "<< punkciory <<endl;
					wyniki.close();
					window.clear();
					for (auto& gwiazda : gwiazdy) {
						gwiazda.update();
						window.draw(gwiazda.shape);
					}
					koniec.rysuj(window);
					window.draw(przegrales);
					if (koniec.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
						break;
					}
				}
				


			}

			for (const auto& pocisk : pociski) {
				window.draw(pocisk.sprite);
			}

		}
		if (gracz && stangry == 1) {
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
		if (stangry == 3) {
			window.clear();
			for (auto& gwiazda : gwiazdy) {
				gwiazda.update();
				window.draw(gwiazda.shape);
			}
			highscores.rysuj(window);
			koniec.rysuj(window);
			window.draw(koniecnapis);
			if (koniec.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				break;
			}
			if (highscores.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				stangry = 7;
				window.clear();
				
				
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && stangry == 1) {
			stangry = 2;
		}
		if (stangry == 2 && stangryback == 1) {
			escape.rysuj(window);
			kontynuuj.rysuj(window);
			window.draw(pauza);
			for (auto& gwiazda : gwiazdy) {
				gwiazda.update();
				window.draw(gwiazda.shape);
			}
			if (escape.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				break;
			}
			if (kontynuuj.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				stangry = 1;
				continue;

			}
		}

		if (stangry == 4) {
			window.draw(podajnick);
				if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode >= 32 && event.text.unicode < 128 && wcisnietyklawisz == false) {
						nick += event.text.unicode;
						wcisnietyklawisz = true;
						sf::sleep(sf::milliseconds(150));
						wcisnietyklawisz = false;
					}
					else if (event.text.unicode == 8 && !nick.empty()) {
						nick.pop_back();
						wcisnietyklawisz = true;
						sf::sleep(sf::milliseconds(150));
						wcisnietyklawisz = false;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					stangry = 0;
				}
			tekst.setString(nick);
			window.clear();
			window.draw(podajnick);
			window.draw(tekst);
			window.draw(enter);

		}
		if (stangry == 7) {
			window.clear();
			for (const auto& text : texts) {
				window.draw(text);
			}

			window.display();
		}
		x.rysuj(window);
		if (x.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
			break;
		}
		window.display();
	}
	return 0;
}