#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <cstdlib>  // For system()
#include <limits>   // For std::numeric_limits

class Graph {
public:
    std::unordered_map<std::string, std::vector<std::string>> adjList;

    // Add an edge between two variables (for an undirected graph)
    void addEdge(const std::string &var1, const std::string &var2) {
        adjList[var1].push_back(var2);
        adjList[var2].push_back(var1);
    }

    // Display the graph in the terminal (ASCII representation)
    void displayGraphASCII() {
        std::cout << "Graph (ASCII Art Representation):\n";
        std::cout << "----------------------------------\n";
        for (const auto &node : adjList) {
            std::cout << node.first << " --> ";
            for (const auto &neighbor : node.second) {
                std::cout << neighbor << " ";
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
                dotFile << "  " << node.first << " -- " << neighbor << ";\n";
            }
        }
        dotFile << "}\n";
        dotFile.close();

        std::cout << "Graph exported to " << filename << " successfully." << std::endl;
    }

    // Heuristic function for Oracle (simple lexicographical heuristic)
    double heuristic(const std::string &node, const std::string &goal) {
        return std::abs(static_cast<int>(node[0]) - static_cast<int>(goal[0]));
    }

    // Oracle algorithm to find a path from source to destination
    void oracle(const std::string &source, const std::string &destination) {
        std::priority_queue<std::pair<double, std::vector<std::string>>> openSet; // Min-heap for exploring nodes
        std::set<std::string> closedSet;  // To keep track of visited nodes
        openSet.push({0.0, {source}}); // Start with the initial path from the source

        std::cout << "Starting Oracle Search from " << source << " to " << destination << "...\n";

        while (!openSet.empty()) {
            // Get the path with the lowest cost
            auto current = openSet.top();
            openSet.pop();
            std::vector<std::string> path = current.second;
            std::string currentNode = path.back();

            // If we reach the destination, print the path
            if (currentNode == destination) {
                std::cout << "Path found: ";
                for (const auto &node : path) {
                    std::cout << node << " -> ";
                }
                std::cout << "END" << std::endl;
                return;
            }

            // Mark the current node as visited
            closedSet.insert(currentNode);

            // Explore the neighbors
            for (const auto &neighbor : adjList[currentNode]) {
                if (closedSet.find(neighbor) != closedSet.end()) {
                    continue; // Ignore already visited nodes
                }

                // Create a new path for this neighbor
                std::vector<std::string> newPath = path;
                newPath.push_back(neighbor);

                // Calculate costs
                double gCost = newPath.size(); // g(n) is the number of steps from the source
                double hCost = heuristic(neighbor, destination); // h(n) is the heuristic
                double fCost = gCost + hCost; // f(n) = g(n) + h(n)

                // Push the new path into the open set with the cost
                openSet.push({-fCost, newPath}); // Use negative because std::priority_queue is a max-heap
            }
        }

        std::cout << "No path found from " << source << " to " << destination << ".\n";
    }
};

int main() {
    Graph graph;

    // Define the relationships (edges between variables)
    graph.addEdge("x", "y");
    graph.addEdge("y", "z");
    graph.addEdge("x", "a");
    graph.addEdge("a", "b");
    graph.addEdge("b", "z");
    graph.addEdge("a", "y");

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

    std::cout << "Enter the source variable: ";
    std::getline(std::cin, source);
    std::cout << "Enter the destination variable: ";
    std::getline(std::cin, destination);

    // Perform Oracle Search from source to destination
    graph.oracle(source, destination);

    return 0;
}
