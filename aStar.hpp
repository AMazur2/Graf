#include <vector>
#include <iostream>
#include <cstdlib>
#include <set>
#include <utility>

#define END_X 6
#define END_Y 0

#define START_X 0
#define START_Y 6

#define MAX_ROW 7
#define MAX_COL 7

/*
    Są to kierunki w które możemy sprawdzać z danego wierzchołka możliwe drogi.
*/
enum Next 
{
    UP = 0, LEFT, DOWN, RIGHT,
};

/*
    Klasa V - klasa obrazująca wierzchołem w naszym grafie.
    Każdy wierzchołek posiada współrzedne swoje oraz swojego rodzica, wagę
    oraz wartości funkcji:
    h - funkcja heurystyki,
    g - funkcja która poakzuje koszt dojścia wraz z wejściem do danego wierzchołka
    f - jest so sume g i h.
    Dodatkowo umieszczamy wartość boolowską która pokażen nam czy dany wierzchołek
    jest w znalezionej ścieżce czy nie.
*/
class Vortex
{
    int parent_x, parent_y, weight, x, y;
    double f, g, h;
    bool inPath;
    friend class Graph;

    public: 
        Vortex(int w, int x1, int y1) : x(x1), y(y1), weight(w), g(w), f(0), h(0), inPath(false) {};
};

/*
    Potrzebujemy stworzyć taką parę aby łatwjem było nam stworzyć zbiór
    openList. Ta para składa się z wierzchołka i funkcji f - całkowity koszt
*/
typedef std::pair<double, Vortex*> Pair;

/*
    Klasa ta musi zawierać cały nasz graf, listę tzw otwartych pól i zamkniętych.
    Doadtkowo musimy mieć funkcję do liczenia heurystki.
    Dwie główne funkcje to:
    findPath - czyli nasz algorytm A*,
    displaypath - do wyświetlenia ścieżki.
    Dodatkowo mamy dwa konstruktory - do odczytu z pliku jak i do przetważania
    zwykłej tablicy na graf.
    Funkcje pomocnicze:
    isCorrect - sprawdza czy ewentualny wierzchołek istnieje,
    isEnd - sprawdza czy wierzchołek jest końcem naszej drogi.
*/
class Graph
{
    private:
        std::vector<std::vector<Vortex*>> graph;
        std::set<Pair> openList;
        std::vector<std::vector<bool>> visited;

        int calculateH(Vortex* current);
        Vortex* neightbour(Vortex* current, Next n);
        void create(std::vector<std::vector<int>> &tab);
        bool isCorrect(int row, int col);
        bool isEnd(Vortex* v);

    public:
        Graph(std::string filename);
        Graph(std::vector<std::vector<int>> &tab);
        void aStar();
        void displayPath();
        ~Graph();

};