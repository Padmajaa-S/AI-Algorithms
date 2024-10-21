#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <set>

// Structure to represent a node in the graph
class Node {
public:
    std::string name;
    std::vector<std::vector<std::pair<Node*, int>>> successors; // Each vector represents a group (AND/OR) with costs
    bool isSolved;
    bool isOrNode; // True if OR node, false if AND node
    int cost; // The cost of the node

    Node(std::string n, bool orNode = true) : name(n), isSolved(false), isOrNode(orNode), cost(std::numeric_limits<int>::max()) {}

    // Add a set of successors to this node (each vector represents a group of AND/OR successors)
    void addSuccessors(const std::vector<std::pair<Node*, int>>& group) {
        successors.push_back(group);
    }
};

// Function to print the path and cost of the solution
void printSolution(Node* node) {
    if (!node) return;
    std::cout << "Node: " << node->name << " | Cost: " << node->cost << std::endl;
}

// Heuristic function (simple example: length of the node's name)
int heuristic(Node* node) {
    return node->name.length();
}

// AO* algorithm
int aoStar(Node* node, std::set<Node*>& explored) {
    // If node is solved or already explored, return its cost
    if (node->isSolved) return node->cost;
    if (explored.find(node) != explored.end()) return node->cost;

    explored.insert(node); // Add the current node to the explored list

    // If it has no successors, it is a leaf node
    if (node->successors.empty()) {
        node->cost = heuristic(node);
        node->isSolved = true;
        return node->cost;
    }

    // Explore all successor groups to find the minimum cost path
    int minCost = std::numeric_limits<int>::max();
    std::vector<Node*> bestGroup;

    // Explore each AND/OR group
    for (const auto& group : node->successors) {
        int groupCost = 0;
        std::vector<Node*> groupNodes;
        for (const auto& successor : group) {
            int childCost = aoStar(successor.first, explored); // Recursively call AO* on the child
            groupCost += childCost + successor.second; // Add the transition cost
            groupNodes.push_back(successor.first);
        }

        // If this group's cost is lower than the current minimum, update it
        if (groupCost < minCost) {
            minCost = groupCost;
            bestGroup = groupNodes;
        }
    }

    // Update the node's cost to the minimum found
    node->cost = minCost;
    node->isSolved = true; // Once a node is solved, it doesn't need to be explored again
    explored.erase(node);  // Remove from explored set as it is fully solved

    return node->cost;
}

int main() {
    // Create graph nodes
    Node* A = new Node("A", true);  // Root node (OR node)
    Node* B = new Node("B", true);  // OR node
    Node* C = new Node("C", false); // AND node
    Node* D = new Node("D", true);  // OR node
    Node* E = new Node("E", true);  // OR node
    Node* F = new Node("F", true);  // OR node
    Node* G = new Node("G", false); // AND node
    Node* H = new Node("H", true);  // OR node

    // Define successors (AND/OR structure)
    A->addSuccessors({{B, 1}, {C, 1}}); // A -> (B, C) (OR)
    B->addSuccessors({{D, 2}, {E, 3}}); // B -> (D, E) (OR)
    C->addSuccessors({{F, 1}, {G, 1}}); // C -> (F AND G)
    D->addSuccessors({{H, 4}});         // D -> (H)
    G->addSuccessors({{H, 2}});         // G -> (H)

    // Run AO* starting from the root node A
    std::set<Node*> explored;
    int totalCost = aoStar(A, explored);

    // Print the final solution path and cost
    std::cout << "Optimal cost for reaching the goal: " << totalCost << std::endl;
    printSolution(A);

    // Clean up dynamically allocated nodes
    delete A;
    delete B;
    delete C;
    delete D;
    delete E;
    delete F;
    delete G;
    delete H;

    return 0;
}
