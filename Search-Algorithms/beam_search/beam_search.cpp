#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
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

    // Beam Search Utility function to find a path from source to destination
    void beamSearch(const std::string &source, const std::string &destination, int beamWidth) {
        std::vector<std::vector<std::string>> beam;  // Current candidates (the beam)
        beam.push_back({source});  // Start with the initial path from the source
        std::set<std::string> visited;  // To keep track of visited nodes
        visited.insert(source);

        std::cout << "Starting Beam Search from " << source << " to " << destination << " with beam width " << beamWidth << "...\n";

        while (!beam.empty()) {
            std::vector<std::vector<std::string>> nextBeam;  // Next candidates for the next level

            // Explore all paths in the current beam
            for (const auto &path : beam) {
                std::string current = path.back();  // Get the last node in the current path

                // If we reach the destination, print the path
                if (current == destination) {
                    std::cout << "Path found: ";
                    for (const auto &node : path) {
                        std::cout << node << " -> ";
                    }
                    std::cout << "END" << std::endl;
                    return;
                }

                // Get the neighbors and sort them lexicographically
                std::vector<std::string> neighbors = adjList[current];
                std::sort(neighbors.begin(), neighbors.end());

                // Generate new paths for each unvisited neighbor
                for (const auto &neighbor : neighbors) {
                    if (visited.find(neighbor) == visited.end()) {  // Not visited
                        std::vector<std::string> newPath = path;
                        newPath.push_back(neighbor);  // Create a new path
                        nextBeam.push_back(newPath);  // Add the new path to the next beam
                    }
                }
            }

            // Sort nextBeam paths by their last node (lexicographically)
            std::sort(nextBeam.begin(), nextBeam.end(), [](const std::vector<std::string> &a, const std::vector<std::string> &b) {
                return a.back() < b.back();
            });

            // Limit the next beam to the beam width
            if (nextBeam.size() > beamWidth) {
                nextBeam.resize(beamWidth);
            }

            // Update the visited set and the current beam for the next iteration
            for (const auto &path : nextBeam) {
                visited.insert(path.back());
            }
            beam = nextBeam;  // Update the beam
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

    // Now ask the user for source, destination, and beam width
    std::string source, destination;
    int beamWidth;

    std::cout << "Enter the source variable: ";
    std::getline(std::cin, source);
    std::cout << "Enter the destination variable: ";
    std::getline(std::cin, destination);
    std::cout << "Enter the beam width: ";
    std::cin >> beamWidth;

    // Perform Beam Search from source to destination
    graph.beamSearch(source, destination, beamWidth);

    return 0;
}
