#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
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

    // BFS Utility function to explore all paths from source to destination and stop after finding paths at the current level
    void bfsLexicographical(const std::string &source, const std::string &destination) {
        std::queue<std::vector<std::string>> q;  // Queue of paths
        std::set<std::string> visited;  // Keep track of visited nodes
        bool destinationFound = false;

        // Initialize the queue with the path starting from the source
        q.push({source});

        std::cout << "Searching for paths from " << source << " to " << destination << "...\n";

        while (!q.empty() && !destinationFound) {
            int levelSize = q.size();  // Track the size of the current level
            for (int i = 0; i < levelSize; ++i) {
                // Get the first path from the queue
                std::vector<std::string> path = q.front();
                q.pop();

                // Get the last node in the current path
                std::string lastNode = path.back();

                // If we reached the destination, print the path
                if (lastNode == destination) {
                    std::cout << "Path found: ";
                    for (const auto &node : path) {
                        std::cout << node << " -> ";
                    }
                    std::cout << "END" << std::endl;
                    destinationFound = true;
                }

                // Explore all unvisited neighbors of the last node
                std::vector<std::string> neighbors = adjList[lastNode];
                std::sort(neighbors.begin(), neighbors.end());  // Sort lexicographically

                for (const auto &neighbor : neighbors) {
                    if (std::find(path.begin(), path.end(), neighbor) == path.end()) {  // Not visited in this path
                        std::vector<std::string> newPath = path;
                        newPath.push_back(neighbor);  // Create a new path including this neighbor
                        q.push(newPath);  // Add the new path to the queue
                    }
                }
            }
        }

        if (!destinationFound) {
            std::cout << "No path found from " << source << " to " << destination << ".\n";
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

    // Perform lexicographical BFS from source to destination
    graph.bfsLexicographical(source, destination);

    return 0;
}
