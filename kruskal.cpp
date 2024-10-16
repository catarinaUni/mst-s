#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>

struct Edge {
    int u, v, weight;
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
};

struct Graph {
    int V;
    std::vector<Edge> edges;

    Graph(int V = 0) : V(V) {}

    void addEdge(int u, int v, int weight) {
        edges.push_back(Edge(u, v, weight));
    }

    void printGraph() const {
        std::cout << "Grafo lido:\n";
        for (const Edge &e : edges) {
            std::cout << e.u << " -- " << e.v << " == " << e.weight << "\n";
        }
        std::cout << std::endl;
    }
};

int findSet(int u, std::vector<int> &parent) {
    if (u != parent[u])
        parent[u] = findSet(parent[u], parent);
    return parent[u];
}

void unionSets(int u, int v, std::vector<int> &parent, std::vector<int> &rank) {
    int rootU = findSet(u, parent);
    int rootV = findSet(v, parent);

    if (rootU != rootV) {
        if (rank[rootU] < rank[rootV])
            parent[rootU] = rootV;
        else if (rank[rootU] > rank[rootV])
            parent[rootV] = rootU;
        else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}

std::vector<Edge> MSTKruskal(Graph &G) {
    std::vector<Edge> A;
    std::vector<int> parent(G.V), rank(G.V, 0);

    std::cout << "Executando Kruskal...\n";

    for (int v = 0; v < G.V; ++v)
        parent[v] = v;

    std::sort(G.edges.begin(), G.edges.end(), [](Edge a, Edge b) {
        return a.weight < b.weight;
    });

    for (Edge &e : G.edges) {
        int u = e.u;
        int v = e.v;

        std::cout << "Considerando aresta: " << u << " -- " << v << " com peso " << e.weight << "\n";

        if (findSet(u, parent) != findSet(v, parent)) {

            A.push_back(e);
            std::cout << "Aresta adicionada à MST: " << u << " -- " << v << "\n";

            unionSets(u, v, parent, rank);
        } else {
            std::cout << "Aresta descartada para evitar ciclo: " << u << " -- " << v << "\n";
        }
    }

    std::cout << "\nFinalizando Kruskal...\n";
    return A;
}

void printMSTSummary(const std::vector<Edge> &mst) {
    std::cout << "\nResumo final da MST:\n";
    int totalWeight = 0;
    for (const Edge &e : mst) {
        std::cout << e.u << " -- " << e.v << " == " << e.weight << "\n";
        totalWeight += e.weight;
    }
    std::cout << "Peso total da MST: " << totalWeight << "\n";
}

void readGraphFile(const std::string &filename, Graph &graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo.");
    }

    int graphType;
    file >> graphType;

    int numVertices;
    file >> numVertices;
    if (numVertices <= 0) {
        throw std::runtime_error("Número de vértices inválido.");
    }
    graph.V = numVertices;

    int u, v;
    float weight;

    while (file >> u >> v >> weight) {
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            throw std::runtime_error("Aresta com vértices fora do intervalo.");
        }
        graph.addEdge(u, v, static_cast<int>(weight));
    }

    file.close();
}


int main() {
    Graph G;

    try {
        readGraphFile("input.txt", G);
    } catch (const std::exception &e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    G.printGraph();

    std::vector<Edge> mst = MSTKruskal(G);

    printMSTSummary(mst);

    return 0;
}
