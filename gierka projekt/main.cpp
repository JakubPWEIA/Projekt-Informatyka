#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

void wycentrujTekst(sf::Text& text, const sf::RectangleShape& rectangle) { //Funkcja ustawiajaca tekst na srodku przycisku
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
	bool sprawdzKlikniecie(const sf::Vector2i& mousePos, const sf::Event& event) {//jak myszka jest w granicach przycisku  i wcisniety lewy przycisk to true
		if (prostokat.getGlobalBounds().contains(sf::Vector2f(mousePos)) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			return true;
		}
		return false;
	}

	
	void rysuj(sf::RenderWindow& window) {//Rysowanie przycisku
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
	
	void update() {//Aktualizuje pozycje gwiazdy co wywolanie, jak gwiazda wyleci za ekran to przenosi sie na gore ekranu
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

	void update() {//Aktualizuje pozycje asteroidy co wywolanie, jak asteroida wyleci za ekran to przenosi sie na gore ekranu
		sprite.move(0.f, predkosc);
		if (sprite.getPosition().y > 600) {
			sprite.setPosition(rand() % 800, 0);
		}
	}
};
class Gwiazdaconvex:public sf::ConvexShape{
public:
	sf::ConvexShape gwiazdaconvex;
	float predkosc;

	Gwiazdaconvex(float x, float y, float predkosc) {
		gwiazdaconvex.setPosition(x, y);
		this->predkosc = predkosc;
		gwiazdaconvex.setPointCount(10);
		gwiazdaconvex.setPoint(0, sf::Vector2f(0, -20));
		gwiazdaconvex.setPoint(1, sf::Vector2f(8, -8));
		gwiazdaconvex.setPoint(2, sf::Vector2f(20, -10));
		gwiazdaconvex.setPoint(3, sf::Vector2f(12, 8));
		gwiazdaconvex.setPoint(4, sf::Vector2f(15, 20));
		gwiazdaconvex.setPoint(5, sf::Vector2f(0, 12));
		gwiazdaconvex.setPoint(6, sf::Vector2f(-15, 20));
		gwiazdaconvex.setPoint(7, sf::Vector2f(-12, 8));
		gwiazdaconvex.setPoint(8, sf::Vector2f(-20, -10));
		gwiazdaconvex.setPoint(9, sf::Vector2f(-8, -8));

		gwiazdaconvex.setFillColor(sf::Color::Yellow);
	}

	void update() {//Aktualizuje pozycje gwiazdy (nieregularnej) co wywolanie, jak gwiazda wyleci za ekran to przenosi sie na gore ekranu
		gwiazdaconvex.move(0.f, predkosc);
		if (gwiazdaconvex.getPosition().y > 600) {
			gwiazdaconvex.setPosition(rand() % 800, 0);
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
	bool kolizja() {
		return (this->getPosition().x <= 0 || this->getPosition().x >= 600);
	}
	void graczdraw(sf::RenderWindow& window) {//Rysowanie gracza
		window.draw(sprite);
	}
	void ruchprawo() {//Funkcja do ruchu w prawo 
		if (sprite.getPosition().x < 700+sprite.getGlobalBounds().width) {
			sprite.move(5.f, 0.f);
		}
	}
	void ruchlewo() {//Funkcja do ruchu w lewo
		if (sprite.getPosition().x > 0+sprite.getGlobalBounds().width) {
			sprite.move(-5.f, 0.f);
		}
	}
	void pozycja() {//Funkcja potrzebna zeby dobrze ustawic teksture eksplozji w miejscu w ktorym przegral gracz
		graczx = sprite.getPosition().x;
		graczy = sprite.getPosition().y;
	}
	
};

class Wrog : public sf::Sprite {
public:
	sf::Sprite sprite;
	float predkosc = 2.0f;
	static bool kierunek;
	Wrog(sf::Texture& tekstura, float x, float y) {
		this->setTexture(tekstura);
		this->setPosition(x, y);
		this->setScale(0.3f, 0.3f);
	}
	
	bool ruch() {
		if (kierunek) {
			this->move(predkosc, 0);
		}
		else {
			this->move(-predkosc, 0);
		}

		if (this->getPosition().x <= 0 || this->getPosition().x <= 800) {
			return true;
		}
		return false;
	}
	bool kolizja() {
		return (this->getPosition().x <= 0 || this->getPosition().x >= 750);
	}

};

bool Wrog::kierunek = true;

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

	void utworzWrogow(sf::Texture& tekstura) {//Metoda poziomu na rysowanie wrogow, ilosc wrogow zalezy od poziomu
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

	void drawlvl(sf::RenderWindow& window) {//Metoda rysujaca poziom

		
		for (auto& wrog : wrogowie) {
			window.draw(wrog);
		}
	}

	bool poziomzakonczony() { //Metoda sprawdzajaca czy poziom sie skonczyl potrzebna do automatycznego przelaczania poziomow
		return wrogowie.empty();
	}

	vector<Wrog>& zwrocwrog(){//Metoda zwracajaca wektor wrogow potrzebna do kolizji i do automatycznego przelaczania poziomow
		return wrogowie;
	}

	void update() {
		bool zmiana = false;
		for (auto& wrog : wrogowie) {
			if (wrog.kolizja()) {
				zmiana = true;
				break;
			}
		}
		if (zmiana) {
			Wrog::kierunek = !Wrog::kierunek;
		}
		for (auto& wrog : wrogowie) {
			if (Wrog::kierunek) {
				wrog.move(wrog.predkosc, 0);
			}
			else {
				wrog.move(-wrog.predkosc, 0);
			}
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

	void updatepocisk() {//Metoda aktualizujaca ruch pocisku
		sprite.move(0.f, -speed);
	}


};

void kolizja(vector<Pocisk>& pociski, vector<Wrog>& wrogowie,int& punkciory) {//Funkcja sprawdzajaca kolizje miedzy pociskami a wrogami
	for (auto pocisk = pociski.begin(); pocisk != pociski.end();) {
		bool kolizja = false;
		for (auto wrog = wrogowie.begin(); wrog != wrogowie.end();) {
			if (pocisk->sprite.getGlobalBounds().intersects(wrog->getGlobalBounds())) {//Gdy pocisk trafi wroga usun wroga z wektoru i dodaj punkt
				wrog = wrogowie.erase(wrog);
				kolizja = true;
				punkciory++;
				
			}
			else//Jezeli nie trafi wroga to przesun do nastepnego wroga
			{
				++wrog;
			}
		}

		if (kolizja) {//jezeli kolizja jest true to usun pocisk z wektora 
			pocisk = pociski.erase(pocisk);
		}
		else {//inaczej przejdz do nastepnego pocisku
			++pocisk;
		}
	}
	
}
void kolizjaasteroidy(vector<Pocisk>& pociski, vector<Asteroida>& asteroidy,int& punkciory) {//Funkcja sprawdzajaca kolizje pociskow i asteroid
	for (auto pocisk = pociski.begin(); pocisk != pociski.end();) {							//dziala analogicznie do tej wyzej
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

void EkranPomocy(sf::RenderWindow& window, sf::Font& font) {//Funkcja tworzaca ekran pomocy ktory pojawia sie po przycisnieciu F1
	sf::Text instrukcje;
	instrukcje.setFont(font);
	instrukcje.setString("POMOC:\nUnikaj asteroid, aby przetrwac!\n"
		"Strzelaj do kosmitow za pomoca SPACJI.\n"
		"Poruszaj rakieta strzalkami w LEWO i PRAWO.\n"
		"Zniszcz wszystkich kosmitow, aby wygrac!");
	instrukcje.setCharacterSize(24);
	instrukcje.setFillColor(sf::Color::White);

	
	window.clear(sf::Color::Black);
	window.draw(instrukcje);
	window.display();

	
}
void EkranPoziom(sf::RenderWindow& window, sf::Font& font,int poziomteraz) {//Funkcja tworzaca wstawki przed poziomem z numerem poziomu
	sf::Text poziom;
	poziom.setFont(font);
	poziom.setString("POZIOM "+to_string(poziomteraz+1));//Tu wiadomo zamiana numeru poziomu do stringa
	poziom.setCharacterSize(24);
	poziom.setFillColor(sf::Color::White);


	sf::FloatRect textBounds = poziom.getLocalBounds();//Tutaj robimy tak zeby napis wyswietlal sie na srodku za pomoca FloatRect(prostokat o zmiennoprzecinkowych wspolrzednych i wymiarach)
	poziom.setOrigin(textBounds.width / 2, textBounds.height / 2);
	poziom.setPosition(window.getSize().x / 2, window.getSize().y / 2);


	window.clear(sf::Color::Black);
	window.draw(poziom);
	window.display();
	sf::sleep(sf::seconds(3));//Tutaj dajemy sleep na 3 sekundy zeby wstrzymac rysowanie poziomu 
	window.clear(sf::Color::Black);
}



int main()
{
	

	int poziomteraz = 0;//Zmienna decydujaca ktory poziom jest teraz (pozycja w wektorze od 0 do 2)
	int stangry = 0;//Zmienna manipulujaca stanem gry
	/*stangry = 0 - menu poczatkowe
	  stangry = 1 - gra sie toczy
	  stangry = 2 - escape
	  stangry = 3 - gra sie skonczyla
	  stangry = 4 - nick gracza moze - udalo sie 8)
	  stangry = 5 - jednak highscores 
	  stangry = 6 - przegrana po asteroidzie
	*/
	int stangryback = 0;//Zmienna zwiazana ze stanem gry ktora zapisuje poprzedni stan gry przed zmiana
	int punkciory = 0;//zmienna od punktow gracza
	string punktytekst;//zmienna przechowujaca punkty gracza jako string
	//sprite'y
	sf::Sprite eksplozja;//Sprite eksplozji
	sf::Sprite tlogry;//sprite tla

	//Potrzebne napisy
	sf::Text gameover;
	sf::Font arial;
	sf::Text koniecnapis;
	sf::Text tytul;
	sf::Text pauza;
	sf::Text punkty;
	sf::Text przegrales;

	//wczytanie czcionki
	arial.loadFromFile("zasoby\\arial.ttf");

	gameover.setFont(arial);
	gameover.setString("Game Over");
	gameover.setCharacterSize(80);
	gameover.setFillColor(sf::Color::Red);
	gameover.setPosition(120.f, 300.f);

	punkty.setFont(arial);
	punkty.setFillColor(sf::Color::White);

	koniecnapis.setFont(arial);
	koniecnapis.setString("Koniec Gry !");
	koniecnapis.setCharacterSize(80);
	koniecnapis.setFillColor(sf::Color::White);
	koniecnapis.setPosition(120.f, 100.f);
	
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

	//Potrzebne wektory
	vector<Gwiazda> gwiazdy;
	vector<Asteroida> asteroidy;
	vector<Poziom> poziomy;
	vector<Pocisk> pociski;
	vector <Gwiazdaconvex> gwiazdyconvex;
	
	//zmienne i zegar potrzebne do stworzenia mechaniki cooldownu miedzy strzalami
	bool shoot = true;
	float cooldown = 0.5f;
	sf::Clock shootTimer;

	//Tworzenie gwiazd w ksztalcie bialych okregow
	for (int i = 0; i < 60; i++) {
		float x = rand() % 800;
		float y = rand() % 600;
		float speed = rand() % 2 + 1;
		gwiazdy.emplace_back(x, y, speed);
	}


	sf::RenderWindow window(sf::VideoMode(800, 600), "NajeŸdczcy z kosmosu !");//Glowne okno gry z tytulem
	window.setFramerateLimit(60);//Limit fps dla gry

	//Tekstury
	sf::Texture teksturawrog;
	teksturawrog.loadFromFile("zasoby\\alien.png");

	sf::Texture teksturagracz;
	teksturagracz.loadFromFile("zasoby\\statek.png");

	sf::Texture tlo;
	tlo.loadFromFile("zasoby\\tlo.png");

	
	tlogry.setTexture(tlo);

	sf::Texture asteroidatekstura;
	asteroidatekstura.loadFromFile("zasoby\\asteroida.png");

	sf::Texture pocisktekstura;
	pocisktekstura.loadFromFile("zasoby\\siatka.png");

	sf::Texture eksplozjastatku;
	eksplozjastatku.loadFromFile("zasoby\\eksplozja.png");
	eksplozja.setTexture(eksplozjastatku);

	//Tworzenie asteroid
	for (int i = 0; i < 3; i++) {
		float x = rand() % 800;
		float y = rand() % 600;
		float speed = rand() % 1 + 1;
		asteroidy.emplace_back(x, y, speed, asteroidatekstura);
	}
	//Tworzenie nieregularnych gwiazd
	for (int i = 0; i < 3; i++) {
		float x = rand() % 800;
		float y = rand() % 600;
		float speed = rand() % 1 + 1;
		gwiazdyconvex.emplace_back(x, y, speed);
	}

	
	//Generowanie poziomow
	poziomy.emplace_back(3);
	poziomy[0].utworzWrogow(teksturawrog);
	poziomy.emplace_back(4);
	poziomy[1].utworzWrogow(teksturawrog);
	poziomy.emplace_back(5);
	poziomy[2].utworzWrogow(teksturawrog);

	//Tworzenie gracza
	Gracz G1(teksturagracz, 50.f, 20.f);
	
	//zmienna czy gracz istnieje
	bool gracz = true;

	//Tworzenie przyciskow uzytych w grze
	Przycisk graj(260.f, 350.f, 265.f, 100.f, arial, "Rozpocznij \n\tGre",sf::Color::Black);
	Przycisk koniec(260.f, 450.f, 265.f, 100.f, arial, "Wyjdz",sf::Color::Black);
	Przycisk escape(260.f, 450.f, 265.f, 100.f, arial, "Wyjdz",sf::Color::Black);
	Przycisk kontynuuj(260.f, 300.f, 265.f, 100.f, arial, "Kontynuuj",sf::Color::Black);
	Przycisk x(750.f, 9.f, 40.f, 40.f, arial, "X",sf::Color::Red);
	Przycisk highscores(260.f, 300.f, 265.f, 100.f, arial, "High Scores", sf::Color::Black);

	//Zegar ktory bedzie sprawdzal czas miedzy klatkami
	sf::Clock clock;


	string nick;//Zmienna przechowujaca nick gracza wpisany

	sf::Text tekst;//Obiekt ktory sprawia ze widac tekst na ekranie podczas wpisywania
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
	ofstream wyniki;//Otwarcie pliku
	wyniki.open("wyniki.txt",ios::app);//append po to zeby nowe rzeczy zapisywaly sie na koncu pliki a stare sie nie usuwaly
	string linijka;
	vector <pair<string, int>> wektorwyniki;//wektor zapisujacy linijki pliku, para string(nick) i int(punkty)

	ifstream wynikiodczyt("wyniki.txt");//Odczyt pliku

	while (getline(wynikiodczyt, linijka)) {//Tutaj co linijke rozdzielamy ja na string nickname i int punktygracza
		stringstream wartosci(linijka);
		string nickname;
		int punktygracza;
		wartosci >> nickname >> punktygracza;
		wektorwyniki.push_back({ nickname,punktygracza });//Wrzucamy na koniec wektora
	}

	sort(wektorwyniki.begin(), wektorwyniki.end(), [](const pair<string, int>& a, const pair<string, int>& b) {//Sortujemy wektor malejaco biorac pod uwage punkty
		return a.second > b.second; // .second oznacza druga czesc pary w tym przypadku int
		});
	vector<sf::Text> texts;//wektor przechowujacy teksty do wypisania w tablicy wynikow

	for (int i = 0; i < wektorwyniki.size();i++) {//Wrzucamy dane z pliku do sfmlowego tekstu
		sf::Text wynikitekst;
		wynikitekst.setFont(arial);
		wynikitekst.setString(to_string(i + 1) + "." + wektorwyniki[i].first + " " + to_string(wektorwyniki[i].second));
		wynikitekst.setCharacterSize(20);
		wynikitekst.setFillColor(sf::Color::White);
		wynikitekst.setPosition(10.f, 30.f * i + 10.f);
		texts.push_back(wynikitekst);
	}


	while (window.isOpen()) {//glowna petla gry
		
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		

		float roznicaczasu = clock.restart().asSeconds();//zmienna sprawdzajaca roznice czasu miedzy klatkami

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {//wyswietlanie ekranu pomocy
			EkranPomocy(window, arial);
			continue;
		}

		window.clear();

		if (stangry == 0) {//Menu glowne gry
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
		punktytekst = to_string(punkciory);//Konwersja punktow z int na string
		punkty.setString(punktytekst);//przypisanie stringa z punktami do stringa wyswietlanego na ekranie

		if (stangry == 1) {//Stan w ktorym toczy sie rozgrywka
			window.draw(tlogry);
			window.draw(punkty);

			for (auto& gwiazda : gwiazdy) {//Rysowanie gwiazd
				gwiazda.update();
				window.draw(gwiazda.shape);
			}

			for (auto& asteroida : asteroidy) {//Rysowanie asteroid
				asteroida.update();
				window.draw(asteroida.sprite);
			}
			for (auto& gwiazdaconvex : gwiazdyconvex) {//Rysowanie nieregularnych gwiazd
				gwiazdaconvex.update();
				window.draw(gwiazdaconvex.gwiazdaconvex);
			}
			
			kolizja(pociski, poziomy[poziomteraz].zwrocwrog(), punkciory);//Kolizja pociskow z wrogami na danym poziomie

			kolizjaasteroidy(pociski, asteroidy, punkciory);//Kolizja asteroid z pociskami

			if (asteroidy.empty()) {//Tworzenie nowych asteroid jezeli wektor jest pusty
				for (int i = 0; i < 3; i++) {
					float x = rand() % 800;
					float y = -50;
					float speed = rand() % 1 + 1;
					asteroidy.emplace_back(x, y, speed, asteroidatekstura);
				}
			}
			
			poziomy[poziomteraz].drawlvl(window);//Rysowanie poziomu
			poziomy[poziomteraz].update();
			
			if (poziomy[poziomteraz].zwrocwrog().empty()) {// Jeœli wszyscy wrogowie zostali pokonani na aktualnym poziomie
				poziomteraz++; // PrzejdŸ do nastêpnego poziomu

				if (poziomteraz < poziomy.size()) {
					EkranPoziom(window, arial, poziomteraz);
					poziomy[poziomteraz].utworzWrogow(teksturawrog); // Twórz wrogów dla nowego poziomu
				}
				else {
					stangry = 3; // Gra zakoñczona, wszystkie poziomy ukoñczone
				}
			}
			if (gracz) {//Jak gracz istnieje to go rysuj
				G1.graczdraw(window);
			}
			else {//Jezeli nie to wstawiamy teksture eksplozji w miejsce tekstury graca
				G1.pozycja();
				eksplozja.setPosition(G1.graczx - 60.f, G1.graczy - 40.f);
				window.draw(eksplozja);
				window.draw(gameover);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					wyniki << nick <<" "<< punkciory <<endl;//zapis wyniku gracza do pliku
					wyniki.close();
					stangry = 6;
					window.clear();
				
				}
				


			}

			for (const auto& pocisk : pociski) {//Rysowanie pociskow ktore dodaja sie po wciskaniu spacji
				window.draw(pocisk.sprite);
			}

		}
		if (gracz && stangry == 1) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				G1.ruchprawo();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ) {
				G1.ruchlewo();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shoot) {//Tutaj warunek ktory dodaje pocisk do wektora i przypisuje mu pozycje na podstawie pozycji gracza
				G1.pozycja();
				pociski.emplace_back(G1.graczx + 20.f, G1.graczy - 30.f, pocisktekstura, 5.f);

				shoot = false;
				shootTimer.restart();//restart cooldownu strzalu

			}
			if (!shoot && shootTimer.getElapsedTime().asSeconds() >= cooldown) {//tutaj sprawdzenie cooldownu 
				shoot = true;
			}
			for (auto& pocisk : pociski) {//aktualizacja pociskow w ktore sa w wektorze co powoduje ich ruch w strone przeciwnikow
				pocisk.updatepocisk();
			}
		}
			for (auto& asteroida : asteroidy) {//kolizja gracza z asteroidami
				if (asteroida.sprite.getGlobalBounds().intersects(G1.sprite.getGlobalBounds())) {
					gracz = false;
				
				}
			}

			for (auto it = gwiazdyconvex.begin(); it != gwiazdyconvex.end();) {//Kolizja gracza z nieregularnymi gwiazdkami
				if (it->gwiazdaconvex.getGlobalBounds().intersects(G1.sprite.getGlobalBounds())) {
					punkciory += 20;
					it = gwiazdyconvex.erase(it); 
				}
				else {
					++it; 
				}
			}
		if (stangry == 3) {//Menu po zakonczeniu gry
			window.clear();
			for (auto& gwiazda : gwiazdy) {
				gwiazda.update();
				window.draw(gwiazda.shape);
			}
			highscores.rysuj(window);
			koniec.rysuj(window);
			window.draw(koniecnapis);
			if (koniec.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				wyniki << nick << " " << punkciory << endl;
				wyniki.close();
				break;
			}
			if (highscores.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				wyniki << nick << " " << punkciory << endl;
				wyniki.close();
				stangry = 5;
				
				
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && stangry == 1) {//stan gry dwa czyli pauza pod escapem
			stangry = 2;
		}
		if (stangry == 2 && stangryback == 1) {//Pauza pod escape
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
			if (kontynuuj.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {//Wznowienie gry
				stangry = 1;
				continue;

			}
		}

		if (stangry == 4) {//Okno podawania nicku gracza
			window.draw(podajnick);

				if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode > 32 && event.text.unicode < 128 && wcisnietyklawisz == false) {//Wcisniety klawisz to funkcja ktora jest potrzebna do powodowania spolownienia wpisywania
						nick += event.text.unicode;
						wcisnietyklawisz = true;
						sf::sleep(sf::milliseconds(150));//Sleep spowalniajacy wpisywanie
						wcisnietyklawisz = false;
					}
					else if (event.text.unicode == 8 && !nick.empty()) {//Obsluga backspace
						nick.pop_back();
						wcisnietyklawisz = true;
						sf::sleep(sf::milliseconds(150));
						wcisnietyklawisz = false;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !nick.empty()) {//Po wcisnieciu enter przelaczenie na glowne menu
					stangry = 0;
				}
			tekst.setString(nick);//tutaj ustawiamy tekst na ekranie na to co wpisuje uzytkownik
			window.clear();
			window.draw(podajnick);
			window.draw(tekst);
			window.draw(enter);

		}
		if (stangry == 5) {//Highscores !!!
			window.clear();
			for (auto& gwiazda : gwiazdy) {
				gwiazda.update();
				window.draw(gwiazda.shape);
			}
			for (const auto& text : texts) {//wypisywanie wektoru tekstow
				window.draw(text);
			}
			x.rysuj(window);//przycisk zamykajacy program w dowolnym momencie
			if (x.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				break;
			}
			window.display();
			continue;
		}
		if (stangry == 6) {//Przegrana gry po zderzeniu z asteroida
			for (auto& gwiazda : gwiazdy) {
				gwiazda.update();
				window.draw(gwiazda.shape);
			}
			highscores.rysuj(window);
			koniec.rysuj(window);
			window.draw(przegrales);
			window.display();
			if (koniec.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				break;
			}
			if (highscores.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
				
				stangry = 5;
			}
			continue;
		}
		x.rysuj(window);//Tu tez przycisk ktory pozwala na zatrzymanie programu w kazdym momencie
		if (x.sprawdzKlikniecie(sf::Mouse::getPosition(window), event)) {
			break;
		}
		window.display();
	}
	return 0;
}