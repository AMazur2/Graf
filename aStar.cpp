#include <cfloat>
#include <cmath>
#include <fstream>
#include "aStar.hpp"

using namespace std;

int Graph::calculateH(Vortex* current)                      //do roznych heurystyk
{
    
    //return abs( current->x - END_X ) + abs( current->y - END_Y);              //manhattan
    return max( abs(current->x - END_X), abs(current->y - END_Y));            //diagonal distance
    //return sqrt( pow(current->x - END_X, 2) + pow(current->y - END_Y, 2));    //euklidesowa
    
}

Graph::Graph(vector<vector<int>> &tab)
{
    this->create(tab);
}

Graph::Graph(string filename)
{
    ifstream file;
    file.open(filename);
    vector<int> row;
    vector<vector<int>> table;
    int k;
    for( int i = 0; i < 7; ++i )
    {
        for( int j = 0; j < 7; ++j )
        {
            file >> k;
            row.push_back(k);
        }
        table.push_back(row);
        row.clear();
    }
    file.close();
    this->create(table);
}

bool Graph::isCorrect(int row, int col)                     //sprawdzenie czy dany wierzchołek istnieje
{
    return ( (row >= 0) && (row < MAX_ROW) && (col >= 0) && (col < MAX_COL));
}

void Graph::create(vector<vector<int>> &tab)
{
    for(int i = 0; i < 7; ++i)
    {
        this->visited.push_back({});                //inicjalizujemy graf jak i visited pustymi
        this->graph.push_back({});                  //wierszami, by było łatwo się do nich odnosić
        for(int j = 0; j < 7; ++j)
        {
            Vortex* v = new Vortex(tab[i][j], i, j);//tworzymy nowy wierzchołek
            v->parent_x = i;                        //początkowo każdy wierzchołek ma swoje współrzędne 
            v->parent_y = j;                        //jako współrzędne rodzis które potem będzeimy zmieniać
            v->f = FLT_MAX;                         //wartości poszczególnych funkcji ustawiamy
            v->g = FLT_MAX;                         //początkowo na maksymalną, dostepną wartość,
            v->h = FLT_MAX;                         //w dalszym etapie będziemy to aktualizować
            this->graph[i].push_back(v);            //do grafu dodajemy nowy wierzchołek
            this->visited[i].push_back(false);      //ustawiamy wartość danego wierzchołka
        }                                           //w visited na false
    }

    this->graph[START_X][START_Y]->f = 0;           //ustawiamy wartości funkcji dla 
    this->graph[START_X][START_Y]->h = 0;           //pierwszego wierzchołka - od niego
    this->graph[START_X][START_Y]->g = 0;           //zaczynamy szukać drogi
    this->graph[START_X][START_Y]->inPath = true;   //skoro zaczynamy to jest na drodze
    this->graph[END_X][END_Y]->f = 0;               //ustawiamy początkowo koszt dojścia do końca na 0

    for(int k = 0; k < 7; ++k)                      //wyświetlenie grafu, pomocnicze aby sprawdzic czy 
    {                                               //graf wczytał się poprawnie
        for(int l = 0; l < 7; ++l)
            cout<<graph[k][l]->weight;
        cout << endl;
    }
}

bool Graph::isEnd(Vortex* v)                                //sprawdzamy czy dany wierzchołek jest naszym celem
{
    if( v->x == END_X && v->y == END_Y )
        return true;
    else 
        return false;
}

Vortex* Graph::neightbour(Vortex* current, Next n)          //zwracamy odpowiedniego sąsiada, jesli istnieje
{
    if( n ==  UP )
    {
        if(isCorrect(current->x -1, current->y))
            return this->graph[current->x -1][current->y];
    }
    else if ( n == LEFT )
    {
        if(isCorrect(current->x, current->y -1))
            return this->graph[current->x][current->y -1];
    }
    else if ( n == DOWN )
    {
        if(isCorrect(current->x +1, current->y))
            return this->graph[current->x +1][current->y];
    }
    else if ( n == RIGHT )
    {
        if(isCorrect(current->x, current->y +1))
            return this->graph[current->x][current->y +1];
    }
    else 
        return nullptr;
}

void Graph::aStar()
{
    openList.insert(make_pair (0, graph[START_X][START_Y]));
    Vortex* v;
    int i, j;
    double g1, h1, f1;

    while( !openList.empty())
    {
        Pair p = *openList.begin();       //pobieramy pierwszy element 
        openList.erase(openList.begin()); //a następnie go usuwamy z listy

        i = p.second->x;
        j = p.second->y;
        visited[i][j] = true;
    
        v = p.second;

        for( int k = 0; k < 4; ++k )
        {
            Vortex* n = neightbour(v, (Next) k);
            
            if( !n )                        //czy istnieje
                continue;

            if( isEnd(n) )                  //czy to koniec
            {
                visited[n->x][n->y] = true;
                n->parent_x = i;
                n->parent_y = j;
                return;
            }
            if(visited[n->x][n->y] == true) //czy już tu byliśmy
                continue;
            else                        
            {
                g1 = v->g + n->weight;      //funkcja g to suma g rodzica i wagi n
                h1 = calculateH(n);         //liczymy wybraną heurystykę dla wierzchołka n
                f1 = g1 + h1;               

                if(n->f == FLT_MAX || n->f > f1) //jeżeli wierzchołek n nie ma jeszcze wartości f
                {                               //lub ma większą od f1 to aktualizujemy
                    openList.insert(make_pair(f1, n));
                    n->f = f1;
                    n->g = g1;
                    n->h = h1;
                    n->parent_x = i;
                    n->parent_y = j;
                }
            }
            
        }
    }
}

void Graph::displayPath()                       //sciezke odtwarzamy zaczynając od końca (od celu)
{                                               //kazdy wierzchołek ktory ma rodzica dostaje wartosc
    int row = graph[END_X][END_Y]->parent_x;    //inPath = true, co odpowiada stworzonej przez
    int col = graph[END_X][END_Y]->parent_y;    //algorytm drodze
    graph[END_X][END_Y]->inPath = true;
    while( !(graph[row][col]->parent_x == row && graph[row][col]->parent_y == col))
    {
        graph[row][col]->inPath = true;
        int temp_x = graph[row][col]->parent_x;
        int temp_y = graph[row][col]->parent_y;
        row = temp_x;
        col = temp_y;
    }
    cout << endl << "Path: " << endl;
    for(int i = 0; i < MAX_ROW; ++i)
    {
        for(int j = 0; j < MAX_COL; ++j)
        {
            if(graph[i][j]->inPath == true)
                cout << graph[i][j]->weight;
            else 
                cout << " ";
        }
        cout << endl;
    }
}

Graph::~Graph()
{
    for(int i = 0; i < MAX_ROW; ++i)
    {
        for(int j = 0; j < MAX_COL; ++j)
            delete graph[i][j];
    }
}

int main()
{
    Graph graph("Graf.txt");
    graph.aStar();
    graph.displayPath();
    return 0;
}