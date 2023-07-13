// GRAFY
//----------------------------------------------------------------------
// Autor: Tair Yerniyazov,                            Krakow, 30.05.2023
//----------------------------------------------------------------------

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <stdlib.h>
#include "sets.hpp"

// WIERZCHOŁEK
class Vertex {
private:
    int number;
public:
    int weight = 0; 
    std::string label; 
    Vertex(int n) {number = n;}
    void setWeight(int w) {weight = w;}
    int Number() {return number;}
};

// KRAWĘDŹ
class Edge { 
private: 
    Vertex* v0; 
    Vertex* v1; 
public:
    int weight;
    std::string label;
    Edge(Vertex *V0, Vertex* V1) {v0 = V0; v1 = V1;}
    Vertex* V0() {return v0;}
    Vertex* V1() {return v1;}
    Vertex* Mate(Vertex *v) {return (v == v0) ? v1 : v0;}
};

// GRAF
class GraphAsMatrix {
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
    std::vector<std::vector<Edge*>> adjacencyMatrix;
    int numberOfRows, numberOfColumns, numberOfVertices, numberOfEdges;
public:
    GraphAsMatrix(int n, int m, bool withoutEdges) {
        if (n < 1 || m < 1) {
            std::cerr << "Podano niepoprawne wymiary siatki\n";
            exit(-1);
        }
        numberOfVertices = n * m;
        numberOfColumns = m;
        numberOfRows = n;
        // Początkowa inicjalizacja macierzy sąsiedztwa i wektora wierzchołkow
        for (int r = 0; r < numberOfVertices; r++) {
            vertices.push_back(new Vertex(r));
            adjacencyMatrix.push_back(*(new std::vector<Edge*>));
            for (int c = 0; c < numberOfVertices; c++)
                adjacencyMatrix[r].push_back(nullptr);
        }
        // Inicjalizacja siatki (krawędzie)
        if (!withoutEdges) {
            for (int r = 0; r < numberOfRows; r++) {
                for (int c = 0; c < numberOfColumns; c++) {
                    int u = r * m + c;
                    if (c != numberOfColumns - 1)
                        AddEdge(u, u + 1); 
                    if (r != numberOfRows - 1)
                        AddEdge(u, u + m); 
                }
            }
        }
    }

    int NumberOfVertices() {
        return numberOfVertices;
    }

    int NumberOfEdges() {
        return numberOfEdges;
    }

    bool IsEdge(int u, int v) {
        if (adjacencyMatrix[u][v] != nullptr)
            return true;
        return false;
    }   

    void MakeNull() {
        for (int i = 0; i < numberOfVertices; i++) {
            for (int j = 0; j < numberOfVertices; j++) {
                if (adjacencyMatrix[i][j] != nullptr)
                    delete adjacencyMatrix[i][j];
            }
        }
        vertices.clear();
    } 
                                             
    void AddEdge(int u, int v) {
		if ((u < numberOfVertices) && (v < numberOfVertices)) {
			if (adjacencyMatrix[u][v] == nullptr) {
				numberOfEdges++;
				Edge* edge = new Edge(vertices[u], vertices[v]);
				adjacencyMatrix[u][v] = edge;
                adjacencyMatrix[v][u] = edge;
                edges.push_back(edge);
			}
		}
    }

    Edge* SelectEdge (int u, int v) {
		if ((u < numberOfVertices) && (v < numberOfVertices))
			return adjacencyMatrix[u][v];
        return nullptr;
	}    

    Vertex* SelectVertex(int v) {
     	if (v < numberOfVertices)
			return vertices[v];		  
        return nullptr;	
    }

    // ==================== METODA RYSUJĄCA GRAF/DRZEWO ROZPINAJĄCE ========================
    void DrawTree(bool debugging) {
        for (int r = 0; r < numberOfRows; r++) {
            int m = numberOfColumns;
            // Połączenia poziome w biezącym wierszu
            for (int c = 0; c < numberOfColumns; c++) {
                int u = r * m + c;
                if (debugging) 
                    std::cout << vertices[u] -> Number();
                else
                    std::cout << "*";
                if (c != numberOfColumns - 1)
                    std::cout << (adjacencyMatrix[u][u + 1] != nullptr ? " ---- " : "      ");
            }
            std::cout << "\n";
            // Połączenia pionowe pod biezącym wierszem
            for (int c = 0; c < numberOfColumns; c++) {
                int u = r * m + c;
                if (r != numberOfRows - 1)
                    std::cout << (adjacencyMatrix[u][u + m] != nullptr ? "|      " : "       ");
            }
            std::cout << "\n";
        }
    }

    // =================== METODA IMPLEMENTUJĄCA ALGORYTM KRUSKAL'A ========================
    GraphAsMatrix Kruskal(bool debugging) {
        // Tworzenie nowego podziału 
        std::vector<SetAsArray*> sets{};

        // Inicjalizacja pustego drzewa rozpinającego
        GraphAsMatrix spanningTree(numberOfRows, numberOfColumns, true);

        // Tworzenie pierwotnego ciągu podziałow
        for (int i = 0; i < numberOfVertices; i++) {
            SetAsArray* set = new SetAsArray(numberOfVertices);
            set -> Insert(vertices[i] -> Number());
            sets.push_back(set);
        }

        // Wybór losowej krawędzi i łączenie zbiorów
        srand(time(NULL));
        while (sets[0] -> Count() < numberOfVertices) {
            int edgeChoice = rand() % edges.size();
            Edge* selectedEdge = edges[edgeChoice];

            // Szukamy wierzchołek należących do krawędzi w zbiorach bieżącego podziału
            int cursorA, cursorB;
            for (int s = 0; s < sets.size(); s++) {
                if (sets[s] -> IsMember(selectedEdge -> V0() -> Number()))
                    cursorA = s;
                if (sets[s] -> IsMember(selectedEdge -> V1() -> Number()))   
                    cursorB = s;
            }

            // Sprawdzamy znalezione zbiory na tożsamość przez porównywanie wskaźników
            if (cursorA != cursorB) {
                // Łączymy zbiory
                SetAsArray* sumSet = new SetAsArray(*(sets[cursorA]), *(sets[cursorB]));
                // Aktualizujemy podział
                bool sumConfirmed = false;
                for (int s = 0; s < sets.size(); s++) {
                    if (*sets[s] <= *sumSet) {
                        if (sumConfirmed) {
                            sets.erase(sets.begin() + s);
                            break;
                        }
                        sets[s] = sumSet;
                        sumConfirmed = true;
                    }
                }
                // Dodajemy krawędź do drzewa rozpinającego
                spanningTree.AddEdge(selectedEdge -> V0() -> Number(), selectedEdge -> V1() -> Number()); 
            }

            // Debuggowanie (ta część wyświetla tok działania algorytmu) 
            if (debugging) {
                std::cout << "\n\nWYLOSOWANA KRAWĘDŹ: " << selectedEdge -> V0() -> Number() 
                << " --- " << selectedEdge -> V1() -> Number() << "\n";
                std::cout << "Liczba nierozważonych krawędzi: " << edges.size() << "\n";
                std::cout << "Stan ciągu podziałów zbioru wierchołków: \n";
                for (int s = 0; s < sets.size(); s++) {
                    sets[s] -> Print();
                }
                edges.erase(edges.begin() + edgeChoice);
            }
        }
        // Zwrócenie gotowego drzewa rozpinającego
        return spanningTree;
    }                              

    // ========================= METODA RYSUJĄCA GOTOWY LABIRINT ===========================
    void DrawLabirynth() {
        // Odczytujemy szkielet dokumentu PostScript i zapisujemy do pliku wynikowego
        std::ifstream in("PostScript_base.txt");
        std::ofstream out("Labyrinth.ps");
        while(!in.eof()){
            std::string line;
            getline(in, line);
            out << line << "\n";
        }
        // Zapisujemy wartości zmiennych do pliku PostScript
        out << "/totalRows " << std::to_string(2 * numberOfRows  - 1) << " def\n";
        out << "/totalCols " << std::to_string(numberOfColumns) << " def\n";

        for (int r = 0; r < numberOfRows; r++) {
            int m = numberOfColumns;

            // Ścianki pionowe (w miejscu brakujących krawędziach poziomych) w biezącym wierszu
            for (int c = 0; c < numberOfColumns; c++) {
                int u = r * m + c;
                if (c != numberOfColumns - 1) {
                    if (adjacencyMatrix[u][u + 1] == nullptr) {
                        // Dopisujemy wywołanie procedury do pliku PostScript
                        out << "1 1 " + std::to_string(c) + " " + std::to_string(2 * r) + " drawLine\n"; 
                    }
                }
            }

            // Ścianki poziome (w miejscu brakujących krawędziach pionowych) pod biezącym wierszem
            for (int c = 0; c < numberOfColumns; c++) {
                int u = r * m + c;
                if (r != numberOfRows - 1) {
                    if (adjacencyMatrix[u][u + m] == nullptr) {
                        // Dopisujemy wywołanie procedury do pliku PostScript
                        out << "0 1 " + std::to_string(c) + " " + std::to_string(2 * r + 1) + " drawLine\n"; 
                    }
                }
            }
        }

        // Dodadnie komendy do rysowania ramki i tekstu
        out << "\n0 0 moveto\n"
               "3 setlinewidth\n"
               "0.4 0 0.8 setrgbcolor\n"
               "0 totalRows 1 add 20 mul rlineto\n"
               "totalCols 40 mul 0 rlineto\n"
               "0 totalRows 1 add 20 mul -1 mul rlineto\n"
               "totalCols 40 mul -1 mul 0 rlineto\n"
               "stroke\n";

        // Dodanie napisu  
        out << "\n0 0 0 setrgbcolor\n"
               "/Courier-Bold findfont 30 scalefont setfont\n"
               "0 totalRows 1 add 20 mul 20 add moveto\n"
               "(Labirynt Kruskal'a) show\n";

        // Wyświetlanie obrazku wektorowego do pliku PostScript
        out << "\nshowpage\n";
        in.close();
        out.close();

        // Komunikat o zakończeniu generowania obrazku
        std::cout << "\nGotowy labirint został zapisany w pliku \"Labyrinth.ps\"\n";
    }
};

#endif
