// PROJEKT (Algorytm Kruskal'a w zastosowaniu)
//----------------------------------------------------------------------
// Autor: Tair Yerniyazov,                            Krakow, 30.05.2023
//----------------------------------------------------------------------

#include "graph.hpp"
#include <cstring>

void graphTest(bool debugging, int n, int m) {
    std::cout << "================================================================\n\n";
    GraphAsMatrix* graph = new GraphAsMatrix(n, m, false); 

    if (debugging) {
        std::cout << "\nPONIŻEJ PRZEDSTAWIONY JEST CAŁY TOK DZIAŁANIA ALGORYTMU\n\n";
        std::cout << "Pierwotny graf (zgenerowana siatka):\n\n";
        graph -> DrawTree(true);
        GraphAsMatrix spanningTree = graph -> Kruskal(true);
        std::cout << "\nDrzewo rozpinające:\n\n";
        spanningTree.DrawTree(true);   
        spanningTree.DrawLabirynth();  
    } else {
        std::cout << "Pierwotny graf (zgenerowana siatka):\n\n";
        graph -> DrawTree(false);
        GraphAsMatrix spanningTree = graph -> Kruskal(false); 
        std::cout << "\nDrzewo rozpinające:\n\n";
        spanningTree.DrawTree(false);
        spanningTree.DrawLabirynth();
    }


    std::cout << "\n================================================================\n";
}

int main(int argc, char *argv[]) {
    // >>> Debugging
    // graphTest(true, 4, 3);

    // >>> Deployment
    graphTest(false, 15, 15);

    return 0;
}
