#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include <cstdlib>  // For system()

class Graph {
public:
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjList;

    // Add an edge with a weight between two variables (for an undirected graph)
    void addEdge(const std::string &var1, const std::string &var2, int weight) {
        adjList[var1].push_back({var2, weight});
        adjList[var2].push_back({var1, weight});
    }

    // Display the graph in the terminal (ASCII representation)
    void displayGraphASCII() {
        std::cout << "Graph (ASCII Art Representation):\n";
        std::cout << "----------------------------------\n";
        for (const auto &node : adjList) {
            std::cout << node.first << " --> ";
            for (const auto &neighbor : node.second) {
                std::cout << neighbor.first << "(" << neighbor.second << ") ";
            }
            std::cout << std::endl;
        }
        std::cout << "----------------------------------\n";
    }

    // Export the graph to a DOT file for Graphviz visualization
    void exportGraphToDot(const std::string &filename) {
        std::ofstream dotFile;
        dotFile.open(filename);

        dotFile << "graph G {\n";
        for (const auto &node : adjList) {
            for (const auto &neighbor : node.second) {
                if (node.first < neighbor.first) { // Avoid double printing edges in undirected graph
                    dotFile << "  " << node.first << " -- " << neighbor.first << " [label=\"" << neighbor.second << "\"];\n";
                }
            }
        }
        dotFile << "}\n";
        dotFile.close();

        std::cout << "Graph exported to " << filename << " successfully." << std::endl;
    }

    // Heuristic function (example: Manhattan distance based on alphabetical order)
    double heuristic(const std::string &node, const std::string &goal) {
        return std::abs(static_cast<int>(node[0]) - static_cast<int>(goal[0]));
    }

    // A* algorithm with heuristics and extended list
    void aStar(const std::string &source, const std::string &destination) {
        // Priority queue for open list: stores paths with f(n) = g(n) + h(n)
        std::priority_queue<std::tuple<double, int, std::vector<std::string>>,
                            std::vector<std::tuple<double, int, std::vector<std::string>>>,
                            std::greater<>> pq;

        // Set to keep track of fully expanded nodes (extended list)
        std::set<std::string> extendedList;

        // Start with the source node
        pq.push({0.0, 0, {source}});

        std::cout << "Starting A* Search from " << source << " to " << destination << "...\n";

        while (!pq.empty()) {
            // Extract the current path and cost
            auto [fCost, gCost, path] = pq.top();
            pq.pop();
            std::string currentNode = path.back();

            // If we reached the destination, print the path
            if (currentNode == destination) {
                std::cout << "Path found with total cost " << fCost << ": ";
                for (const auto &node : path) {
                    std::cout << node << " -> ";
                }
                std::cout << "END" << std::endl;
                return;
            }

            // Skip if this node has already been fully expanded
            if (extendedList.find(currentNode) != extendedList.end()) {
                continue;
            }

            // Mark the node as fully expanded (added to extended list)
            extendedList.insert(currentNode);

            // Explore the neighbors
            for (const auto &neighbor : adjList[currentNode]) {
                if (extendedList.find(neighbor.first) == extendedList.end()) {
                    // Create a new path to this neighbor
                    std::vector<std::string> newPath = path;
                    newPath.push_back(neighbor.first);

                    // Calculate g(n) (cost so far) and h(n) (heuristic estimate)
                    int newGCost = gCost + neighbor.second;
                    double hCost = heuristic(neighbor.first, destination);
                    double newFCost = newGCost + hCost;  // f(n) = g(n) + h(n)

                    // Push the new path into the priority queue
                    pq.push({newFCost, newGCost, newPath});
                }
            }
        }

        std::cout << "No path found from " << source << " to " << destination << ".\n";
    }
};

int main() {
    Graph graph;

    // Define the relationships (edges between variables) with weights
    graph.addEdge("A", "B", 4);
    graph.addEdge("A", "C", 2);
    graph.addEdge("B", "D", 5);
    graph.addEdge("C", "D", 8);
    graph.addEdge("C", "E", 10);
    graph.addEdge("D", "E", 2);
    graph.addEdge("D", "F", 6);
    graph.addEdge("E", "F", 3);

    // Export the graph to a DOT file for Graphviz visualization
    std::string dotFilename = "graph_output.dot";
    graph.exportGraphToDot(dotFilename);

    // Use Graphviz to generate a PNG image of the graph
    std::string graphImage = "graph_output.png";
    std::string dotCommand = "dot -Tpng " + dotFilename + " -o " + graphImage;
    system(dotCommand.c_str());  // Execute the dot command to generate the PNG image

    // Open the PNG image using the default image viewer
    #if defined(_WIN32) || defined(_WIN64)
        system(("start " + graphImage).c_str()); // Windows
    #elif defined(__APPLE__)
        system(("open " + graphImage).c_str());  // macOS
    #else
        system(("xdg-open " + graphImage).c_str());  // Linux
    #endif

    // Now ask the user for source and destination
    std::string source, destination;

    std::cout << "Enter the source node: ";
    std::getline(std::cin, source);
    std::cout << "Enter the destination node: ";
    std::getline(std::cin, destination);

    // Perform A* Search from source to destination
    graph.aStar(source, destination);

    return 0;
}
