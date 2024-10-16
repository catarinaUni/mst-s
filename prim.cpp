#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <stdexcept>

struct Graph {
    int V;
    std::vector<std::vector<std::pair<int, double>>> adj;

    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int u, int v, double weight) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }
};

struct MinHeap {
    std::vector<int> heap;
    std::vector<int> pos;
    std::vector<double> chave;

    MinHeap(int V) : heap(V), pos(V), chave(V, std::numeric_limits<double>::max()) {
        for (int i = 0; i < V; ++i) {
            heap[i] = i;
            pos[i] = i;
        }
    }

    void heapify(int idx, int size) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < size && chave[heap[left]] < chave[heap[smallest]])
            smallest = left;

        if (right < size && chave[heap[right]] < chave[heap[smallest]])
            smallest = right;

        if (smallest != idx) {
            std::swap(heap[smallest], heap[idx]);
            pos[heap[smallest]] = smallest;
            pos[heap[idx]] = idx;
            heapify(smallest, size);
        }
    }

    int extractMin(int &size) {
        if (size == 0) return -1;

        int minVertex = heap[0];
        heap[0] = heap[size - 1];
        pos[heap[0]] = 0;
        size--;

        heapify(0, size);
        return minVertex;
    }

    void decreaseKey(int vertex, double newKey) {
        int idx = pos[vertex];
        chave[vertex] = newKey;

        while (idx > 0 && chave[heap[(idx - 1) / 2]] > chave[heap[idx]]) {
            std::swap(heap[idx], heap[(idx - 1) / 2]);
            pos[heap[idx]] = idx;
            idx = (idx - 1) / 2;
        }
        pos[vertex] = idx;
    }

    bool isEmpty(int size) const {
        return size == 0;
    }
};

std::vector<std::pair<int, int>> primMST(Graph &G, int r) {
    int V = G.V;
    std::vector<double> chave(V, std::numeric_limits<double>::max());
    std::vector<int> predecessor(V, -1);
    std::vector<bool> inMST(V, false);

    MinHeap heap(V);
    int size = V;

    chave[r] = 0;
    heap.decreaseKey(r, 0);

    std::cout << "Fluxo de execução do algoritmo:\n";

    while (!heap.isEmpty(size)) {
        int u = heap.extractMin(size);
        inMST[u] = true;

        std::cout << "Extrai: " << u << " com chave: " << chave[u] << "\n";

        for (const auto &[v, weight] : G.adj[u]) {
            if (!inMST[v] && weight < chave[v]) {
                chave[v] = weight;
                predecessor[v] = u;
                heap.decreaseKey(v, weight);

                std::cout << "Atualiza: " << v << " com nova chave: " << weight << ", predecessor: " << u << "\n";
            }
        }
    }

    std::vector<std::pair<int, int>> mstEdges;
    for (int v = 0; v < V; ++v) {
        if (predecessor[v] != -1) {
            mstEdges.push_back({predecessor[v], v});
        }
    }

    std::cout << "Chaves finais:\n";
    for (int i = 0; i < V; ++i) {
        std::cout << "Vértice " << i << ": chave = " << chave[i] << ", predecessor = " << predecessor[i] << "\n";
    }

    return mstEdges;
}

void printMST(const std::vector<std::pair<int, int>> &mstEdges) {
    std::cout << "Arestas na MST:\n";
    for (const auto &[u, v] : mstEdges) {
        std::cout << u << " -- " << v << "\n";
    }
}

void printGraph(const Graph &G) {
    std::cout << "Grafo lido:\n";
    for (int u = 0; u < G.V; ++u) {
        std::cout << u << ": ";
        for (const auto &[v, weight] : G.adj[u]) {
            std::cout << "(" << v << ", " << weight << ") ";
        }
        std::cout << "\n";
    }
}

void readGraphFile(const std::string &filename, Graph &graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo.");
    }

    int type;
    file >> type;
    if (type != 0 && type != 1) {
        throw std::runtime_error("Tipo de grafo inválido.");
    }

    int numVertices;
    file >> numVertices;
    if (numVertices < 0) {
        throw std::runtime_error("Número de vértices inválido.");
    }
    graph = Graph(numVertices);

    int u, v;
    double weight;
    while (file >> u >> v >> weight) {
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            throw std::runtime_error("Aresta com vértices fora do intervalo.");
        }
        graph.addEdge(u, v, weight);
    }

    file.close();
}

int main() {

    Graph G(0);
    try {
        readGraphFile("input.txt", G);
        printGraph(G);
    } catch (const std::exception &e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    std::vector<std::pair<int, int>> mstEdges = primMST(G, 0);

    printMST(mstEdges);

    return 0;
}
