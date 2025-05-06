// Design and implement Parallel Breadth First Search based on existing algorithms using
// OpenMP. Use a Tree or an undirected graph for BFS.

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>

using namespace std;

class Graph {
public:
    int numVertices;
    vector<vector<int>> adjacencyList;
    vector<bool> visited;

    Graph(int vertices) : numVertices(vertices) {
        adjacencyList.resize(numVertices);
        //visited.resize(numVertices, false);
    }

    void addEdge(int u, int v) {
        adjacencyList[u].push_back(v);
        adjacencyList[v].push_back(u);  // Since it's an undirected graph
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
    int vertices, edges;
    cout << "Enter number of vertices and edges: ";
    cin >> vertices >> edges;

    Graph graph(vertices);
    cout << "Enter " << edges << " edges (u v):\n";
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }

    int startVertex;
    cout << "Enter starting vertex for BFS: ";
    cin >> startVertex;

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
