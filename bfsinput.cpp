#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>
#include <fstream>

using namespace std;

class Graph {
public:
    int numVertices;
    vector<vector<int>> adjacencyList;
    vector<bool> visited;

    Graph(int vertices) : numVertices(vertices) {
        adjacencyList.resize(numVertices);
    }

    void addEdge(int u, int v) {
        adjacencyList[u].push_back(v);
        adjacencyList[v].push_back(u); // undirected
    }

    void resetVisited() {
        visited.assign(numVertices, false);
    }

    void bfs(int startVertex) {
        queue<int> q;
        q.push(startVertex);
        visited[startVertex] = true;

        while (!q.empty()) {
            int current = q.front(); q.pop();
            cout << current << " ";

            for (int neighbor : adjacencyList[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }

    void parallelBfs(int startVertex) {
        queue<int> q;
        q.push(startVertex);
        visited[startVertex] = true;

        while (!q.empty()) {
            int current = q.front(); q.pop();
            cout << current << " ";

            #pragma omp parallel for
            for (int i = 0; i < adjacencyList[current].size(); i++) {
                int neighbor = adjacencyList[current][i];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }
};

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Could not open input file.\n";
        return 1;                                                                                                                                       
    }

    int vertices, edges;
    inputFile >> vertices >> edges;

    Graph graph(vertices);
    for (int i = 0; i < edges; ++i) {
        int u, v;
        inputFile >> u >> v;
        graph.addEdge(u, v);
    }

    int startVertex;
    inputFile >> startVertex;
    inputFile.close();

    graph.resetVisited();
    cout << "\nSequential BFS traversal: ";
    auto startTimeSeq = chrono::high_resolution_clock::now();
    graph.bfs(startVertex);
    auto endTimeSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endTimeSeq - startTimeSeq;
    cout << "\nExecution Time (Sequential): " << durationSeq.count() << " ms\n";

    graph.resetVisited();
    cout << "\nParallel BFS traversal: ";
    auto startTimePar = chrono::high_resolution_clock::now();
    graph.parallelBfs(startVertex);
    auto endTimePar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endTimePar - startTimePar;
    cout << "\nExecution Time (Parallel): " << durationPar.count() << " ms\n";

    return 0;
}
