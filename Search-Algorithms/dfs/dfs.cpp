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

    // DFS Utility function to explore all paths from source to destination
    void dfsUtil(const std::string &currentNode, const std::string &destination, 
                 std::set<std::string> &visited, std::vector<std::string> &path) {
        // Mark the current node as visited and add it to the current path
        visited.insert(currentNode);
        path.push_back(currentNode);

        // If destination is reached, print the path
        if (currentNode == destination) {
            std::cout << "Path found: ";
            for (const auto &node : path) {
                std::cout << node << " -> ";
            }
            std::cout << "END" << std::endl;
        } else {
            // Explore all neighbors in lexicographical order
            std::vector<std::string> neighbors = adjList[currentNode];
            std::sort(neighbors.begin(), neighbors.end()); // Sort neighbors lexicographically

            for (const auto &neighbor : neighbors) {
                if (visited.find(neighbor) == visited.end()) { // If not visited
                    dfsUtil(neighbor, destination, visited, path);
                }
            }
        }

        // Backtrack: remove current node from path and mark it as unvisited
        path.pop_back();
        visited.erase(currentNode);
    }

    // Perform DFS to find all paths from source to destination in lexicographical order
    void dfsLexicographical(const std::string &source, const std::string &destination) {
        std::set<std::string> visited;
        std::vector<std::string> path;
        
        std::cout << "Searching for paths from " << source << " to " << destination << "...\n";
        dfsUtil(source, destination, visited, path);
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

    // Perform lexicographical DFS from source to destination
    graph.dfsLexicographical(source, destination);

    return 0;
}
