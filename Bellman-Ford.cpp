#include <iostream>
#include <fstream>
#include <cstdlib>

#include <ctime>
#include <string>

using namespace std;

struct adj_list  ///***|-> struktura reprezentujaca pojedyncza liste sasiedztwa dla jednego wierzcholka
{
    int v,w;    /// zmienna v przechowuje nam sasiadow, a w wage
    adj_list* next;
};

void print_data(int vertecies, int n_of_edges, fstream& stream) ///funkcja wypisujaca wygenerowane grafy do pliku
{
    int number_of_poz_wyk = n_of_edges - vertecies + 1;  ///liczba wykonan pozostalych po stworzeniu sciezki
                                                        ///laczacej wszystkie wierzcholki grafu
    int tmp = 0;
    int numer_of_vertex = vertecies;             // numer wierzcholka
    int counter = 0;                             // inicjalizacja licznika

    stream.open("dane.txt", ios_base::app);           ///otwieramy strumien danych zapisu do pliku

    stream << vertecies<<" "<<n_of_edges<<" "<<rand()%vertecies<<endl;

                                                        ///wypisujemy ilosc wierzcholkow, krawedzi i wierzcholek startowy

                                                        ///wierzcholek startowy oraz wagi krawedzi sa pseudolosowe
    for(int i = 0; i < vertecies - 1 && i < n_of_edges -1 ; i++)

                                                        ///wypisujemy glowny cykl laczacy wszystkie wierzcholki po kolei
    {
        stream << i<< " "<<i+1<<" "<<rand()%100+1<<endl;
    }


    while(counter < number_of_poz_wyk) ///wypisujemy pozostale krawdzie w zaleznosci od wypelnienia
    {
        tmp = 0;

        while(counter < number_of_poz_wyk && tmp < numer_of_vertex - 2)
        {
            stream << numer_of_vertex - 1<<" "<< tmp <<" "<<rand()%10+1<<endl;

            counter++;
            tmp++;
        }
        numer_of_vertex--;
    }
    stream.close();           ///zamykamy strumien do pliku

}

void generate_data(fstream& stream)
{
    //int n_of_vertex= 10;                            // ilosc wierzcholkow w grafie
    //int n_of_vertex = 50;
    int n_of_vertex = 100;
    //int n_of_vertex = 500;
    //int n_of_vertex = 1000;
    int max_edges[5];                            ///****|-> tablica maksymalnych liczb krawedzi
    int edges[5];                            ///****|-> tablica liczb krawedzi dla kazdego z przypadkow zadanych w projekcie
    //int percent_of_filling= 25;                            ///****|-> liczba% wypelnien zadanych przypadkow
    //int percent_of_filling= 50; 
    //int percent_of_filling= 75;
    int percent_of_filling= 100;

    stream.open("dane.txt", ios_base::out);       ///****|-> otwieramy strumien do zapisu danych do pliku dane.txt
    stream.close();                               ///****|-> zamykamy go, daje to pewnosc, ze plik uprzednio zostal
                                                    ///****|-> wyczyszczony z poprzednich danych

    for (int i = 0; i < 5; i++) ///******|-> generujemy kolejne przypadki dla ktorych pozniej rozpoczynamy
                                ///******|-> wypisywanie grafow do pliku poprzez funkcje wypisz_dane
    {
        max_edges [i] = n_of_vertex * (n_of_vertex - 1) /2;
        edges[i] = percent_of_filling * max_edges[i] /100;
        for(int c = 0; c < 100; c++) 
        {
        print_data (n_of_vertex, edges[i], stream);
        }
    }

}

void display_path(int* previous, int* cost_of_access, int vertex)
{
    int* display = new int [vertex];  ///alokujemy pamiec dla tablicy dynamicznej do wyswietlania sciezki

    int n = 0;
    int m;

    for(int c = 0; c < vertex; c++)    ///wyswietlamy dojscie do kazdego z wierzcholkow oraz koszt dojscia

                                        ///razem z wierzcholkiem startowym, dla niego koszt dojsca = 0
    {
        cout << c <<": ";
        for(m = c; m != -1 && n < vertex; m = previous[m])
        {
            display [n++] = m;
        }

        while(n)cout << display[--n] << " ";

        cout << "$" << cost_of_access[c] << endl;
    }
    delete [] display;             ///usuwamy tablice
}

void BF_list(int vertex, int edge, int start, adj_list** tab_list)
{
    adj_list* neighbors;

    int max_int =2147483647;            ///maksymalna wartosc int w 32bit -> 64bit 9223372036854775807

    int* cost_of_access;                 ///inicjujemy wskaznik na tablice dynamiczna kosztow dojscia
    cost_of_access = new int [vertex];

    int* previous;                    ///inicjujemy wskaznik na tablice dynamiczna poprzednikow
    previous = new int [vertex];

    for(int c = 0; c < vertex; c++)
    {
        previous[c] = -1;             ///przypisujemy wartosci tablicy poprzednikow -1 nie ma wierzcholka ujmenego
        cost_of_access[c] = max_int;     ///koszty dojscia ustawiamy na nieskonczonosc, tutaj na maksymalna wartosc inta
    }

    cost_of_access[start] = 0;           ///zerujemy koszt dojscia do naszego wierzcholka startowego

    for(int c = 1; c < vertex; c++)
    {
        for(int b = 0; b < vertex; b++)    ///przeszukujemy liste sasiedztwa w poszukiwaniu krawedzi dla
                                            ///danego wierzcholka relaksujac kolejne krawedzie
        {
            for(neighbors = tab_list[b]; neighbors; neighbors = neighbors -> next)
            {
                if(cost_of_access[b] != max_int && cost_of_access[neighbors -> v] > cost_of_access[b] + neighbors -> w)
                {
                    cost_of_access[neighbors -> v] = cost_of_access [b] + neighbors -> w;
                    previous[neighbors -> v] = b;
                }
            }
        }
    }
    /*cout <<"Lista sasiedztwa:\n";
    display_path(previous, cost_of_access, vertex);*/
                            ///************* ^-> funkcja do wyswietlania drog oraz kosztow dojscia
}

void BF_matrix(int** graph_m, int vertex, int edge, int start)
{

    int max_int =2147483647;            ///maksymalna wartosc int w 32bit -> 64bit 9223372036854775807

    int* cost_of_access;                 ///inicjujemy wskaznik na tablice dynamiczna kosztow dojscia
    cost_of_access = new int [vertex];

    int* previous;                    ///inicjujemy wskaznik na tablice dynamiczna poprzednikow
    previous = new int [vertex];

    for(int c = 0; c < vertex; c++)
    {
        previous[c] = -1;             ///przypisujemy wartosci tablicy poprzednikow
        cost_of_access[c] = max_int;     ///koszty dojscia ustawiamy na nieskonczonosc, tutaj na maksymalna wartosc inta
    }

    cost_of_access[start] = 0;           ///zerujemy koszt dojscia do naszego wierzcholka startowego

    for(int c = 1; c < vertex; c++)
    {
        for(int b = 0; b < vertex; b++)
        {
            for(int v = 0; v < vertex; v++)    ///przeszukujemy macierz sasiedztwa w poszukiwaniu krawedzi dla
                                                ///danego wierzcholka relaksujac kolejne krawedzie
            {
                if(cost_of_access[b] != max_int && graph_m[v][b] != 0 && cost_of_access[v] > cost_of_access[b] + graph_m[v][b])
                {
                    cost_of_access[v] = cost_of_access [b] +  graph_m[v][b];
                    previous[v] = b;
                }
            }
        }
    }
    /*cout <<"Macierz sasiedztwa:\n";
    display_path(previous, cost_of_access, vertex);*/
                            ///************* ^-> funkcja do wyswietlania drog oraz kosztow dojscia
}

void display_list(adj_list** tab_list, int vertex)
{
    /// ################## |-> funkcja do wyswietlania tablicy list
    adj_list* list_w;
    cout<< "lista:\n";
    for(int i = 0; i < vertex; i++)
      {
        cout << "tab[" << i << "] =";
        list_w = tab_list[i];
        while(list_w)
        {
          cout << list_w->v <<" ";
          list_w = list_w->next;
        }
        cout << endl;
    }
    delete []list_w; ///***|-> usuwamy liste sluzaca do wyswietlania tablicy list sasiedztwa
}

void graph_adjlist(int& vertex, int& edge, int& start, int** graph, adj_list** tab_list, adj_list* list)
{
    int v1, v2, weight;

    for(int c = 0; c < vertex; c++)   ///wypelniamy tablice list NULL'ami
    {
        tab_list[c] = NULL;

    }

    for(int c = 0; c < edge; c++)   ///tworzymy listy dla grafu skierowanego
    {
        v1 = graph[c][0];    ///wczytujemy dane z reprezentacji grafu z pliku
        v2 = graph[c][1];
        weight = graph[c][2];

        list = new adj_list;

        list->v = v2;
        list->w = weight;
        list->next = tab_list[v1];
        tab_list[v1] = list;

    }

    for(int c = 0; c < edge; c++)
    {
        v2 = graph[c][0];    ///wczytujemy dane z reprezentacji grafu z pliku
        v1 = graph[c][1];
        weight = graph[c][2];

        list = new adj_list;

        list->v = v2;
        list->w = weight;
        list->next = tab_list[v1];
        tab_list[v1] = list;
    }                           ///*** |-> funkcja odpowiada za tworzenie list dla grafu nieskierowanego

    /*display_list(tab_list, vertex);   */   ///**|-> funkcja odpowiedzialna za wyswietlanie listy sasiedztwa
}

void display_matrix(int** graph_m, int vertex)
{
    ///********** |-> funkcja wyswietlajaca macierz sasiedztwa

    for(int c = 0; c < vertex; c++)
    {
        for(int v = 0; v < vertex; v++)
        {
            cout << graph_m[c][v] <<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void** graph_adjmatrix(int& vertex, int& edge, int& start, int** graph, int** graph_m)
{
    int v1, v2, weight;
    int p = 0;

    for (int v = 0; v < vertex; ++v)   ///zerujemy krawedzie; 0 = brak krawedzi
    {
        for(int b = p; b < vertex; ++b)
        {
            graph_m[v][b] = 0;
            graph_m[b][v] = 0;
        }
        p++;
    }

    for (int v = 0; v < edge; ++v)
    {
        v1 = graph[v][0];        ///wczytujemy dane z reprezentacji grafu z pliku
        v2 = graph[v][1];
        weight = graph[v][2];

        graph_m[v1][v2] = weight;  ///wpisujemy dane do macierzy sasiedztwa dla grafu nieskierowaniego
        graph_m[v2][v1] = weight;  ///dla skierowanego wylaczyc te linijke
    }
    //display_matrix(graph_m, vertex);
}

void** load_graph(int** graph, int& vertex, int& edge, int& start, ifstream& input)  // funkcja wczytujaca graf z pliku
{
    //cout<< vertex <<" "<<edge<<" "<<start<<endl; ///wyswietlamy ilosc wierzcholkow, krawedzi i wierzcholek startowy
    for(int x = 0; x < edge;x++)    ///wczytujemy graf z pliku do tablicy dynamicznej 2-wymiarowej
    {
        input >> graph[x][0];
        input >> graph[x][1];
        input >> graph[x][2];

        /*cout<<graph[x][0]<<" ";
        cout<<graph[x][1]<<" ";
        cout<<graph[x][2]<<endl;*/ /// |-> wyswietlanie grafu, w celu wyswietlenia odkomentowac tu i na poczatku funkcji
    }
}

void find_way_list(int** graph, int& vertex, int& edge, int& start, fstream& output) // funkcja do znalezienia najkrotszej drogi dla listy sasiedztwa
{
    double time_during;                    //***|-> czas trwania
    clock_t time1, time2;           ///***|-> zmienne odpowiadajace za pomiar czasu

    adj_list** tab_list;
    adj_list* list, *del;      ///***|-> struktury dynamiczne odpowiadajace za reprezentacje grafu
                                    ///***|-> w postaci listy sasiedztwa

    tab_list = new adj_list * [vertex]; ///***|-> alokujemy pamiec dla tablicy list sasiedztwa

    graph_adjlist(vertex, edge, start, graph, tab_list, list);    ///funkcja odpowiedzialna za tworzenie
                                                                ///tablicy dynamicznej list sasiedztwa

    time1 = clock();                ///zaczynamy pomiar czasu trwania algorytmu

    BF_list(vertex, edge, start, tab_list);   ///funkcja odpowiedzialna za szukanie najkrotszej sciezki algorytmem
                                                ///Bellmana-Forda dla listy sasiedztwa

    time2 = clock() - time1;        ///konczymy pomiar czasu trwania algorytmu


      for(int i = 0; i < vertex; i++) ///usuwamy listy z tablicy list
      {
        list = tab_list[i];
        while(list)
        {
          del = list;
          list = list->next;
          delete del;
        }
      }
        delete [] tab_list;   ///usuwamy tablice list

    time_during = (double) (time2) / (CLOCKS_PER_SEC/1000);       ///konwertujemy czas trwania na zmienna double i obliczamy

    ///*****|-> wypisujemy czas trwania algorytmu BF dla listy sasiedztwa strumieniem wyjscie
    output << "Czas trwania dla listy przy: "<<vertex<<" i: "<<edge*100/(vertex * (vertex - 1) /2)<<"% wypelnienia: "<<time_during<<" [ms]"<<endl;

}

void find_way_matrix(int** graph, int& vertex, int& edge, int& start, fstream& output)   // funkcja do znalezienia najkrotszej drogi dla macierzy sasiedztwa
{
    double time_during;                 //***|-> czas trwania
    clock_t time1, time2;             ///***|-> zmienne odpowiadajace za pomiar czasu

    int** graph_m;                     ///***|-> tworzymy macierz sasiedztwa a nastepnie alokujemy dla niej pamiec

    graph_m = new int *[vertex];
    for(int v = 0; v < vertex; ++v)
    {
        graph_m[v] = new int [vertex];
    }

    graph_adjmatrix(vertex, edge, start, graph, graph_m);   ///funkcja odpowiedzialna za tworzenie macierzy sasiedztwa

    time1 = clock();                          ///inicjujemy odmierzanie czasu

    BF_matrix(graph_m, vertex, edge, start); ///funkcja odpowiedzialna za szukanie najkrotszej sciezki algorytmem
                                              ///Bellmana-Forda dla macierzy sasiedztwa

    time2 = clock() - time1;        ///koniec odmierzania czasu trwania algorytmu

    for(int v = 0; v < vertex; ++v) ///***|-> usuwamy macierz sasiedztwa
    {
        delete [] graph_m[v];
    }
    delete [] graph_m;

    time_during = (double) (time2) / (CLOCKS_PER_SEC/1000);       ///konwertujemy czas trwania [w milisekundach] na zmienna double i obliczamy
   ///*****|-> wypisujemy czas trwania algorytmu BF dla listy sasiedztwa strumieniem wyjscie
   output << "Czas trwania dla macierzy przy: "<<vertex<<" i: "<<edge*100/(vertex * (vertex - 1) /2)<<"% wypelnienia: "<<time_during<<" [ms]"<<endl; 
   

}

void find_way(ifstream& input)
{
    fstream output;
    int vertex, edge, start;
    int** graph;

    output.open("pomiar.txt", ios_base::app);  ///otwieramy strumien wyjsciowy do pliku pomiar.txt
                                                ///z prawem do zapisu w formie dopisywania danych
    for(int i = 0; i < 100; i++)
    {


        input >> vertex;             ///wczytujemy kolejno: liczbe wierzcholkow, krawedzi i wierzcholek startowy
        input >> edge;
        input >> start;

        graph = new int* [edge];    ///alokowanie pamieci dla tablicy dynamicznej dwuwymiarowej

        for(int c = 0; c < edge; c++)
        {
            graph[c] = new int [3];
        }

        load_graph(graph, vertex, edge, start, input);          ///inicjujemy wczytywanie grafu z pliku
        find_way_list(graph, vertex, edge, start, output);        ///inicjujemy szukanie drogi dla listy sasiedztwa
        //find_way_matrix(graph, vertex, edge, start, output);      ///inicjujemy szukanie drogi dla macierzy sasiedztwa

        for(int z = 0; z < edge; ++z) ///|-> usuwamy tablice dynamiczna przechowujaca dane grafu z pliku
        {
            delete [] graph[z];
        }
        delete [] graph;
    }
    output.close();    ///****|-> zamykamy strumien wyjsciowy
}

void measurement(ifstream& stream)  // funkcja wywoluje szereg funkcji odpowiedzialnych za wczytanie danych 
{                                     // oraz za odnalezienie najkrotszej drogi i zapisanie jej do pliku z wynikami

    stream.open("dane.txt", ios_base::in); ///otwarcie strumienia do pliku z danymi wejsciowymi dla grafow
                                             ///z prawem do odczytu danych

    find_way(stream);           ///**|-> uruchamiamy proces odnajdywania drogi dla grafow

    stream.close();                        ///zamykamy strumien danych

}



int main()
{
    int choice = 3;          ///zainicjowanie zmiennej odpowiedzialnej za menu wyboru

    fstream dane;
    ifstream input;       ///zainicowanie zmiennych odpowiedzialnych za strumienie danych

    srand(time(NULL));      ///ustawienie pomiaru czasu od czasu zero

    do
    {
        ///*******************************|->menu wczytywania danych
        do
        {

            cout << "Wybierz opcje: 1. wygeneruj dane do pliku  2. pomiar czasu\n";
            cin >> choice;
            if(choice > 2 || choice < 1) cout << "Nie ma takiej opcji, wybierz ponownie\n";

        }while( choice > 2 || choice < 1 );

        switch( choice )
        {
            case 1: generate_data(dane);///generuje dane i zapisuje je do pliku
            case 2: measurement(input);///wywoluje szereg funkcji odpowiedzialnych za wczytanie danych
                            ///oraz za odnalezienie najkrotszej drogi i zapisanie jej do pliku z wynikami
        }


    }while( choice > 2 || choice < 1 );


    return 0;
}
