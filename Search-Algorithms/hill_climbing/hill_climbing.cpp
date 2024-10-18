#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>  // For system()

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

    // Hill Climbing Utility function to find a path from source to destination
    void hillClimbing(const std::string &source, const std::string &destination) {
        std::string current = source;  // Start from the source node
        std::vector<std::string> path;  // To store the path taken
        path.push_back(current);
        std::set<std::string> visited;  // Keep track of visited nodes
        visited.insert(current);

        std::cout << "Starting Hill Climbing from " << source << " to " << destination << "...\n";

        while (true) {
            if (current == destination) {
                std::cout << "Path found: ";
                for (const auto &node : path) {
                    std::cout << node << " -> ";
                }
                std::cout << "END" << std::endl;
                return;
            }

            std::vector<std::string> neighbors = adjList[current];
            std::sort(neighbors.begin(), neighbors.end());  // Sort lexicographically

            std::string nextNode = "";  // To store the next node
            for (const auto &neighbor : neighbors) {
                if (visited.find(neighbor) == visited.end()) {  // Not visited
                    nextNode = neighbor;  // Pick the first unvisited neighbor
                    break;
                }
            }

            // If no better neighbor is found, terminate
            if (nextNode.empty()) {
                std::cout << "No path found from " << source << " to " << destination << ".\n";
                return;
            }

            // Move to the next node
            path.push_back(nextNode);
            visited.insert(nextNode);
            current = nextNode;  // Update current node
        }
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

    // Perform Hill Climbing from source to destination
    graph.hillClimbing(source, destination);

    return 0;
}
