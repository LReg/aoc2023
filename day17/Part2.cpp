#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <map>
#include <cassert>
#include <algorithm>

struct Point{
    int x, y;
    bool operator<(const Point& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

struct State{
    int pathLength;
    Point p;
    short straightCount;
    Direction lastDir;
    //std::vector<Point> path;
    bool operator>(const State& other) const {
        return pathLength > other.pathLength;
    }
    bool operator==(const State& other) const {
        return p.x == other.p.x && p.y == other.p.y && lastDir == other.lastDir && straightCount == other.straightCount;
    }
    void print() {
        std::cout << "----------------" << std::endl;
        std::cout << "Position: " << p.x << ", " << p.y << std::endl;
        std::cout << "Path Length: " << pathLength << std::endl;
        std::cout << "Straight Count: " << straightCount << std::endl;
        std::cout << "Last Direction: " << lastDir << std::endl;
        std::cout << "----------------" << std::endl;
    }
};

std::priority_queue<State, std::vector<State>, std::greater<>> queue;
std::vector<std::vector<int>> map;
std::vector<State> visited;

/*void printPath(std::vector<Point> path) {
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            Point val {j, i};
            if (std::find(path.begin(), path.end(), val) != path.end())
                std::cout << "X";
            else
                std::cout << '.';
        }
        std::cout << std::endl;
    }
}*/


void dijkstra() {
    State start = {0, {0, 0 }, 0, DOWN};
    State start2 = {0, {0, 0 }, 0, RIGHT};
    queue.push(start);
    queue.push(start2);
    while (!queue.empty()) {
        State current = queue.top();
        queue.pop();
        // reached end
        if (current.p.x == map[0].size() - 1 && current.p.y == map.size() - 1 && current.straightCount >= 4 && current.straightCount <= 10) {
            std::cout << "End Position: " << current.p.x << ", " << current.p.y << std::endl;
            std::cout << "Found end with len of: " << current.pathLength << std::endl;
            /*std::cout << "path vector len " << current.path.size() << std::endl;
            for (auto &point : current.path) {
                std::cout << point.x << ", " << point.y << std::endl;
            }
            printPath(current.path);*/
            return;
        }
        for (int i = 0; i < 4; i++) {
            State newState {current};
            Direction dir = static_cast<Direction>(i);
            // opposite direction
            if ((dir == UP && current.lastDir == DOWN) || (dir == DOWN && current.lastDir == UP) || (dir == LEFT && current.lastDir == RIGHT) || (dir == RIGHT && current.lastDir == LEFT)) {
                continue;
            }
            // to long path length
            /*
             * Once an ultra crucible starts moving in a direction,
             * it needs to move a minimum of four blocks in that direction before
             * it can turn (or even before it can stop at the end).
             *  However, it will eventually start to get wobbly
             * :an ultra crucible can move a maximum of ten consecutive blocks without turning.
             */

            if (current.straightCount < 4 && dir != current.lastDir) {
                continue;
            }
            else if (current.straightCount == 10 && dir == current.lastDir) {
                continue;
            }
            else if (dir != current.lastDir)
                newState.straightCount = 1;
            else if (dir == current.lastDir) {
                newState.straightCount++;
            }
            Point newPoint {current.p};
            switch (dir) {
                case UP:
                    newPoint.y--;
                    break;
                case DOWN:
                    newPoint.y++;
                    break;
                case LEFT:
                    newPoint.x--;
                    break;
                case RIGHT:
                    newPoint.x++;
                    break;
                default:
                    assert(false);
            }
            assert(newPoint != current.p);
            // out of map
            if (newPoint.x < 0 || newPoint.x >= map[0].size() || newPoint.y < 0 || newPoint.y >= map.size()) {
                continue;
            }

            // new queue entry
            newState.p = newPoint;
            newState.lastDir = dir;
            newState.pathLength += map[newPoint.y][newPoint.x];
            //newState.path.push_back(newPoint);
            //std::cout << "pushing position: " << newPoint.x << ", " << newPoint.y << " with path length: " << newState.pathLength << std::endl;
            if (std::find_if(visited.begin(), visited.end(),
                             [&newState](State state) -> bool {
                return newState == state
                || (newState.p == state.p && newState.pathLength>=state.pathLength && newState.straightCount == state.straightCount && newState.lastDir == state.lastDir)
                ;
            }) != visited.end()) {
                continue;
            }
            if (newState.pathLength % 100 == 0)
                std::cout << newState.pathLength << std::endl;
            //newState.path.push_back(newPoint);
            queue.push(newState);
            visited.push_back(newState);
        }
    }
}

int main() {
    std::ifstream file("day17/input.txt");
    std::string line;
    while(std::getline(file, line)) {
        std::vector<int> row;
        for (char &val : line) {
            row.push_back(val - '0');
        }
        map.push_back(row);
    }
    for (auto &row : map) {
        for (int &val : row) {
            std::cout << val;
        }
        std::cout << std::endl;
    }
    dijkstra();
}