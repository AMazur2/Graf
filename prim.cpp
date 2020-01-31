#include <iostream>
#include <fstream>

using namespace std;

struct Edge
{
    int v1, v2, weight;     //Wierzchołki krawędzie, waga krawędzi
};

class Queue
{
    private:
        Edge *Heap;
        int hpos;
    public:
        Queue( int n );
        ~Queue();
        Edge front();
        void push( Edge e );
        void pop( );
};

struct TNode
{
    TNode *next;
    int v, weight;
};

class MSTree
{
    private:
        TNode **A;          //tablica list sasiedztwa
        int Alen;           //licznik komorek w tablicy
        int weight;         //waga calego drzewa
    public:
        MSTree ( int n );
        ~MSTree();
        void addEdge ( Edge e );
        TNode *getAList ( int n );
        void print( );
};

Queue::Queue( int n )
{
    Heap = new Edge[ n ];    //tablica krawędzi
    hpos = 0;               //pozycja w kopcu
}

Queue::~Queue()
{
    delete [] Heap;
}

Edge Queue::front()
{
    return Heap[0];         //zwraca krawedz z poczatku kopca
}

void Queue::push( Edge e )
{
    int i, j;

    i = hpos++;             //koniec kopca
    j = ( i - 1 ) >> 1;

    while( i && ( Heap[j].weight > e.weight ))
    {
        Heap[i] = Heap[j];
        i = j;
        j = ( i - 1 ) >> 1;
    }

    Heap[i] = e;
}

void Queue::pop()
{
    int i, j;
    Edge e;

    if( hpos )
    {
        e = Heap[--hpos];
        i = 0;
        j = 1;

        while( j < hpos )
        {
            if( ( j + 1 < hpos ) && ( Heap[j+1].weight < Heap[j].weight) )
                j++;
            if( e.weight <= Heap[j].weight )
                break;
            Heap[i] = Heap[j];
            i = j;
            j = ( j << 1 ) + 1;
        }
        Heap[i] = e;
    }
}

MSTree::MSTree( int n )
{
    int i;

    A = new TNode *[n];
    for( i = 0; i < n; ++i )
        A[i] = NULL;

    Alen = n-1;
    weight = 0;
}

MSTree::~MSTree()
{
    int i;
    TNode *p, *r;

    for( i = 0; i <= Alen; ++i )
    {
        p = A[i];
        while(p)
        {
            r = p;
            p = p->next;
            delete r;
        }
    }
    delete []A;
}

void MSTree::addEdge( Edge e )
{
    TNode *p;

    weight += e.weight;             //do wagi drzewa dodajemy wafe krawedzi
    p = new TNode;                  //tworzymy wezel
    p->v = e.v2;                    //wierzcholek koncowy
    p->weight = e.weight;           //waga krawedzi
    p->next = A[ e.v1 ];            //dodajemy do listy
    A[ e.v1 ] = p;

    p = new TNode;                  //tak samo dla krawedzi odwrotnej
    p->v = e.v1;
    p->weight = e.weight;
    p->next = A[ e.v2 ];
    A[ e.v2 ] = p;
}

TNode * MSTree::getAList( int n )
{
    return A[n];
}

void MSTree::print()
{
    int i;
    TNode *p;
    cout << endl;
    for( i = 0; i <= Alen; ++i )
    {
        cout << "Vertex " << i << " - ";
        for( p = A[i]; p; p = p->next )
            cout << p->v << ":" << p->weight << " ";
        cout << endl;
    }
    cout << "Weight = " << weight << endl << endl;
}

void prim(int n, int m, int table[][3])
{
    Edge e;
    TNode *p;
    int i, v;

    Queue Q ( m );                      //kolejka priorytetowa oparta na kopcu
    MSTree T ( n );                     //minimalne drzewo rozpinajace
    MSTree G ( n );                     //graf

    bool *visited = new bool[n];

    for( i = 0; i < n; ++i )
        visited[i] = false;             

    for( i = 0; i < m; ++i )            //wczytujemy krawędzie grafu
    {
        e.v1 = table[i][0];
        e.v2 = table[i][1];
        e.weight = table[i][2];
        cout << "#" << i << " " << e.v1 << " " << e.v2 << " " << e.weight << endl;
        G.addEdge( e );
    }
    cout << "Graph: " << endl;
    G.print();
    cout << endl;
                                        //zaczybamy tworzyć minimalne drzewo rozpinające
    v = 0;                              //wierzchołek startowy
    visited [v] = true;                 //skoro zaczynamy to jest odwiedzony

    for( i = 1; i < n; ++i )
    {
        for( p = G.getAList(v); p; p = p->next) //przegladamy liste sasiadow
        {
            if( !visited[p->v] )        //jesli nie odwiedzony
            {
                e.v1 = v;               //to tworzymy krawedz
                e.v2 = p->v;
                e.weight = p->weight;
                Q.push(e);              //dodajemy ja do kolejki 
            }
        }
        
        do
        {
            e = Q.front();              //pobieramy krawedz z kolejki
            Q.pop();
        } while (visited[e.v2]);        //dopoki prowadzi poza drzewo  

        T.addEdge ( e );                //dodajemy krawedz do drzewa
        visited [ e.v2 ] = true;        //koeljny wierzcholek zostaje odwieddzony
        v = e.v2;
    }
    cout << "Minimal Spanning Tree: " << endl;
    T.print();

    delete [] visited;
}
/*
    Plik z ktorego wypelniany jest nasz grad
    musi zawierac opis krawedzi
    czyli trojki w postaci:
    skad    do kąd  jaka waga
*/
int main()
{
    ifstream file;
    int v = 6;                          //liczba wierzcholkow
    int e = 14;                          //liczba krawedzi
    file.open( "Graf.txt");
    int t[e][3];
    for( int i = 0; i < e; ++i)
    {
        for( int j = 0; j < 3; ++j )
            file >> t[i][j];
    }
    file.close();

    prim( v , e, t );

    return 0;
}