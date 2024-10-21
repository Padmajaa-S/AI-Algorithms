
# AI Search Algorithms

This repository contains implementations of various AI search algorithms, including:

- **Breadth-First Search (BFS)**
- **Depth-First Search (DFS)**
- **Beam Search**
- **Oracle Search**
- **Hill Climbing**
- **Branch and Bound**
- **Branch and Bound with Extended List**
- **Branch and Bound with Estimated Heuristics**
- * A* Search**
- **Best-First Search**
## Gaming Algorithms
- **Alpha-Beta Pruning** (for game trees)
## GRAPH
![graph_output](https://github.com/user-attachments/assets/6b269fa7-5c5e-418b-912f-e28692e9b366)


## Algorithms

### 1. Breadth-First Search (BFS)

BFS explores the neighbor nodes at the present depth prior to moving on to nodes at the next depth level. This algorithm is useful for finding the shortest path in an unweighted graph.

### 2. Depth-First Search (DFS)

DFS explores as far as possible along each branch before backing up. It uses a stack data structure and can be implemented recursively.

### 3. Beam Search

Beam Search is a heuristic search algorithm that explores a graph by expanding the most promising nodes. It limits the number of nodes at each level to a fixed beam width.

### 4. Oracle Search

Oracle Search uses a pre-defined set of optimal paths to guide the search process. This method can greatly speed up search in scenarios where optimal solutions are known in advance.

### 5. Hill Climbing

Hill Climbing is an iterative algorithm that starts with an arbitrary solution and makes incremental changes to improve it. It can be susceptible to local maxima.

### 6. Branch and Bound

Branch and Bound is a general algorithm for finding optimal solutions to various optimization problems by systematically enumerating candidate solutions.

### 7. Branch and Bound with Extended List

This variant enhances the standard Branch and Bound approach by maintaining an extended list of candidates to optimize the search process.

### 8. Branch and Bound with Estimated Heuristics

This implementation incorporates heuristic estimates to guide the search more efficiently, improving the performance of the basic Branch and Bound method.

### 9. A* Search

A* is an informed search algorithm that uses heuristics to find the least-cost path from a start node to a target node. It combines features of Dijkstra’s Algorithm and BFS.

### 10. Best-First Search

Best-First Search selects the node that appears to be the best according to a given heuristic. This method is efficient in exploring graphs with optimal paths.


###  Alpha-Beta Pruning

Alpha-Beta Pruning is an optimization technique for the minimax algorithm that reduces the number of nodes evaluated in the search tree, enhancing performance in game-playing AI.
