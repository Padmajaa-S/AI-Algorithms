#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <limits>

class Graph {
public:
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjList;

    // Add an edge between two nodes with a given cost
    void addEdge(const std::string &node1, const std::string &node2, int cost) {
        adjList[node1].push_back({node2, cost});
        adjList[node2].push_back({node1, cost});
    }

    // Heuristic function (simple: return the length difference between the current node and the goal)
    int heuristic(const std::string &current, const std::string &goal) {
        return std::abs(static_cast<int>(current.length()) - static_cast<int>(goal.length()));
    }

    // Best-First Search algorithm
    void bestFirstSearch(const std::string &source, const std::string &goal) {
        // Priority queue to store frontier nodes, ordered by heuristic value (min-heap)
        std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<>> pq;
        
        // Set to keep track of visited nodes
        std::set<std::string> visited;

        // Start with the source node
        pq.push({heuristic(source, goal), source});
        std::unordered_map<std::string, std::string> cameFrom;  // For reconstructing the path

        std::cout << "Starting Best-First Search from " << source << " to " << goal << "...\n";

        while (!pq.empty()) {
            auto current = pq.top();
            pq.pop();

            std::string currentNode = current.second;
            int currentCost = current.first;

            // If we reached the goal, reconstruct and print the path
            if (currentNode == goal) {
                std::cout << "Goal reached: " << goal << std::endl;
                reconstructPath(cameFrom, source, goal);
                return;
            }

            // Mark the node as visited
            if (visited.find(currentNode) != visited.end()) continue;
            visited.insert(currentNode);

            // Explore the neighbors
            for (const auto &neighbor : adjList[currentNode]) {
                std::string neighborNode = neighbor.first;
                int edgeCost = neighbor.second;

                if (visited.find(neighborNode) == visited.end()) {
                    // Calculate heuristic value for the neighbor node and add to the priority queue
                    int hCost = heuristic(neighborNode, goal);
                    pq.push({hCost, neighborNode});
                    cameFrom[neighborNode] = currentNode;  // Track the path
                }
            }
        }

        std::cout << "No path found from " << source << " to " << goal << ".\n";
    }

    // Function to reconstruct the path from the source to the goal
    void reconstructPath(const std::unordered_map<std::string, std::string> &cameFrom, const std::string &start, const std::string &goal) {
        std::vector<std::string> path;
        std::string currentNode = goal;

        // Trace the path from goal to start
        while (currentNode != start) {
            path.push_back(currentNode);
            currentNode = cameFrom.at(currentNode);
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());

        std::cout << "Path: ";
        for (const auto &node : path) {
            std::cout << node << " -> ";
        }
        std::cout << "END" << std::endl;
    }
};

int main() {
    Graph graph;

    // Define the edges and their costs (undirected graph)
    graph.addEdge("A", "B", 1);
    graph.addEdge("A", "C", 2);
    graph.addEdge("B", "D", 4);
    graph.addEdge("B", "E", 1);
    graph.addEdge("C", "F", 5);
    graph.addEdge("D", "G", 2);
    graph.addEdge("E", "H", 3);
    graph.addEdge("F", "I", 2);
    graph.addEdge("H", "G", 1);
    graph.addEdge("I", "G", 1);

    // Start Best-First Search from node A to node G
    std::string source = "A";
    std::string goal = "G";

    graph.bestFirstSearch(source, goal);

    return 0;
}
