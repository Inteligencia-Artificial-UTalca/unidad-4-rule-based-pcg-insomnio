#include <iostream>
#include <vector>
#include <random>   // For random number generation
#include <chrono>   // For seeding the random number generator

// Define Map as a vector of vectors of integers.
// You can change 'int' to whatever type best represents your cells (e.g., char, bool).
using Map = std::vector<std::vector<int>>;

/**
 * @brief Prints the map (matrix) to the console.
 * @param map The map to print.
 */
void printMap(const Map& map) {
    std::cout << "--- Current Map ---" << std::endl;
    for (const auto& row : map) {
        for (int cell : row) {
            // Adapt this to represent your cells meaningfully (e.g., ' ' for empty, '#' for occupied).
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

/**
 * @brief Function to implement the Cellular Automata logic.
 * It should take a map and return the updated map after one iteration.
 * @param currentMap The map in its current state.
 * @param W Width of the map.
 * @param H Height of the map.
 * @param R Radius of the neighbor window (e.g., 1 for 3x3, 2 for 5x5).
 * @param U Threshold to decide if the current cell becomes 1 or 0.
 * @return The map after applying the cellular automata rules.
 */
Map cellularAutomata(const Map& currentMap, int W, int H, int R, double U) {
    Map newMap = currentMap; // Initially, the new map is a copy of the current one

    // TODO: IMPLEMENTATION GOES HERE for the cellular automata logic.
    // Iterate over each cell and apply the transition rules.
    // Remember that updates should be based on the 'currentMap' state
    // and applied to the 'newMap' to avoid race conditions within the same iteration.

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            int count = 0;
            for (int dy = -R; dy <= R; ++dy) {
                for (int dx = -R; dx <= R; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < W && ny >= 0 && ny < H)
                        count += currentMap[ny][nx];
                }
            }

            double ratio = count / double((2 * R + 1) * (2 * R + 1) - 1);
            newMap[y][x] = (ratio > U) ? 1 : 0;
        }
    }

    return newMap;
}

/**
 * @brief Function to implement the Drunk Agent logic.
 * It should take a map and parameters controlling the agent's behavior,
 * then return the updated map after the agent performs its actions.
 *
 * @param currentMap The map in its current state.
 * @param W Width of the map.
 * @param H Height of the map.
 * @param J The number of times the agent "walks" (initiates a path).
 * @param I The number of steps the agent takes per "walk".
 * @param roomSizeX Max width of rooms the agent can generate.
 * @param roomSizeY Max height of rooms the agent can generate.
 * @param probGenerateRoom Probability (0.0 to 1.0) of generating a room at each step.
 * @param probIncreaseRoom If no room is generated, this value increases probGenerateRoom.
 * @param probChangeDirection Probability (0.0 to 1.0) of changing direction at each step.
 * @param probIncreaseChange If direction is not changed, this value increases probChangeDirection.
 * @param agentX Current X position of the agent (updated by reference).
 * @param agentY Current Y position of the agent (updated by reference).
 * @return The map after the agent's movements and actions.
 */
 
 // Draw rooms on the map
 void digRoom(Map& map, int x, int y, int maxW, int maxH, int mapW, int mapH) {
    int rw = rand() % (maxW - 1) + 2;
    int rh = rand() % (maxH - 1) + 2;

    for (int i = y - rh / 2; i <= y + rh / 2; ++i) {
        for (int j = x - rw / 2; j <= x + rw / 2; ++j) {
            if (i >= 0 && i < mapH && j >= 0 && j < mapW)
                map[i][j] = 1;
        }
    }
}

Map drunkAgent(const Map& currentMap, int W, int H, int J, int I, int roomSizeX, int roomSizeY,
               double probGenerateRoom, double probIncreaseRoom,
               double probChangeDirection, double probIncreaseChange,
               int& agentX, int& agentY) {
    Map newMap = currentMap; // The new map is a copy of the current one

    // TODO: IMPLEMENTATION GOES HERE for the Drunk Agent logic.
    // The agent should move randomly.
    // You'll need a random number generator.
    // Consider:
    // - How the agent moves (possible steps).
    // - What it does if it encounters a border or an obstacle (if applicable).
    // - How it modifies the map (e.g., leaving a trail, creating rooms, etc.).
    // - Use the provided parameters (J, I, roomSizeX, roomSizeY, probabilities)
    //   to control its behavior.

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> random01(0.0, 1.0);
    std::uniform_int_distribution<> directionPick(0, 3);

    std::vector<std::pair<int, int>> directions = {{1,0},{-1,0},{0,1},{0,-1}};
    int dx = 1, dy = 0;

    for (int j = 0; j < J; ++j) {
        for (int i = 0; i < I; ++i) {
            if (agentX < 0 || agentX >= W || agentY < 0 || agentY >= H) break;

            newMap[agentY][agentX] = 1;

            if (random01(rng) < probGenerateRoom) {
                digRoom(newMap, agentX, agentY, roomSizeX, roomSizeY, W, H);
                probGenerateRoom = 0.1;
            } else {
                probGenerateRoom += probIncreaseRoom;
            }

            if (random01(rng) < probChangeDirection) {
                int dir = directionPick(rng);
                dx = directions[dir].first;
                dy = directions[dir].second;
                probChangeDirection = 0.2;
            } else {
                probChangeDirection += probIncreaseChange;
            }

            agentX += dx;
            agentY += dy;

            agentX = std::max(0, std::min(W - 1, agentX));
            agentY = std::max(0, std::min(H - 1, agentY));
        }
    }

    return newMap;
}

int main() {
    std::cout << "--- CELLULAR AUTOMATA AND DRUNK AGENT SIMULATION ---" << std::endl;

    // --- Initial Map Configuration ---
    int mapRows = 10;
    int mapCols = 20;
    Map myMap(mapRows, std::vector<int>(mapCols, 0)); // Map initialized with zeros

    // TODO: IMPLEMENTATION GOES HERE: Initialize the map with some pattern or initial state.
    // For example, you might set some cells to 1 for the cellular automata
    // or place the drunk agent at a specific position.

    // Drunk Agent's initial position
    int drunkAgentX = mapRows / 2;
    int drunkAgentY = mapCols / 2;
    // If your agent modifies the map at start, you could do it here:
    // myMap[drunkAgentX][drunkAgentY] = 2; // Assuming '2' represents the agent

    std::cout << "\nInitial map state:" << std::endl;
    printMap(myMap);

    // --- Simulation Parameters ---
    int numIterations = 5; // Number of simulation steps

    // Cellular Automata Parameters
    int ca_W = mapCols;
    int ca_H = mapRows;
    int ca_R = 1;      // Radius of neighbor window
    double ca_U = 0.5; // Threshold

    // Drunk Agent Parameters
    int da_W = mapCols;
    int da_H = mapRows;
    int da_J = 5;      // Number of "walks"
    int da_I = 10;     // Steps per walk
    int da_roomSizeX = 5;
    int da_roomSizeY = 3;
    double da_probGenerateRoom = 0.1;
    double da_probIncreaseRoom = 0.05;
    double da_probChangeDirection = 0.2;
    double da_probIncreaseChange = 0.03;


    // --- Main Simulation Loop ---
    for (int iteration = 0; iteration < numIterations; ++iteration) {
        std::cout << "\n--- Iteration " << iteration + 1 << " ---" << std::endl;

        // TODO: IMPLEMENTATION GOES HERE: Call the Cellular Automata and/or Drunk Agent functions.
        // The order of calls will depend on how you want them to interact.

        myMap = cellularAutomata(myMap, mapCols, mapRows, ca_R, ca_U);
        myMap = drunkAgent(myMap, mapCols, mapRows, da_J, da_I,
                           da_roomSizeX, da_roomSizeY,
                           da_probGenerateRoom, da_probIncreaseRoom,
                           da_probChangeDirection, da_probIncreaseChange,
                           drunkAgentX, drunkAgentY);
        printMap(myMap);

        // You can add a delay to visualize the simulation step by step
        // #include <thread> // For std::this_thread::sleep_for
        // #include <chrono> // For std::chrono::milliseconds
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "\n--- Simulation Finished ---" << std::endl;
    return 0;
}