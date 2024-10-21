#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <queue>
#include <limits>
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

    // Branch and Bound algorithm with extended list to find the shortest path
    void branchAndBoundExtended(const std::string &source, const std::string &destination) {
        // Min-heap to store paths and their associated costs
        std::priority_queue<std::pair<int, std::vector<std::string>>, std::vector<std::pair<int, std::vector<std::string>>>, std::greater<>> pq;
        
        // Set to keep track of nodes that have been fully expanded
        std::set<std::string> extendedList;
        
        // Initialize with the source node
        pq.push({0, {source}});

        std::cout << "Starting Branch and Bound Search from " << source << " to " << destination << "...\n";

        while (!pq.empty()) {
            auto current = pq.top();
            pq.pop();
            int currentCost = current.first;
            std::vector<std::string> path = current.second;
            std::string currentNode = path.back();

            // If we reach the destination, print the path and terminate
            if (currentNode == destination) {
                std::cout << "Path found with cost " << currentCost << ": ";
                for (const auto &node : path) {
                    std::cout << node << " -> ";
                }
                std::cout << "END" << std::endl;
                return;
            }

            // If the node is already fully expanded, skip it
            if (extendedList.find(currentNode) != extendedList.end()) {
                continue;
            }

            // Mark the current node as expanded
            extendedList.insert(currentNode);

            // Explore the neighbors
            for (const auto &neighbor : adjList[currentNode]) {
                if (extendedList.find(neighbor.first) == extendedList.end()) {
                    // Create a new path including the neighbor
                    std::vector<std::string> newPath = path;
                    newPath.push_back(neighbor.first);
                    int newCost = currentCost + neighbor.second;

                    // Push the new path into the priority queue
                    pq.push({newCost, newPath});
                }
            }
        }

        std::cout << "No path found from " << source << " to " << destination << ".\n";
    }
};

int main() {
    Graph graph;

    // Define the relationships (edges between variables) with weights
    graph.addEdge("x", "y", 4);
    graph.addEdge("y", "z", 6);
    graph.addEdge("x", "a", 2);
    graph.addEdge("a", "b", 5);
    graph.addEdge("b", "z", 3);
    graph.addEdge("a", "y", 1);

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

    // Perform Branch and Bound Search from source to destination
    graph.branchAndBoundExtended(source, destination);

    return 0;
}
