#include <chrono>
#include <cmath>
#include <deque>
#include <iostream>
#include <vector>

std::vector<int> getGoal(int size, int empty) {
    std::vector<int> tiles(size * size);

    int cnt = 1;

    if (empty == -1) {
        empty = size * size - 1;
    }

    for (int i = 0; i < size * size; i++) {
        if (i == empty) {
            continue;
        }

        tiles[i] = cnt++;
    }

    return tiles;
}

bool isSolvable(const int size, const std::vector<int>& state) {
    int inversions = 0;
    int zeroRow = -1;

    std::vector<int> order;

    for (int i = 0; i < size * size; i++) {
        if (state[i] == 0) {
            zeroRow = i / size;
        }
        else {
            order.push_back(state[i]);
        }
    }

    for (int i = 0; i < size * size - 1; i++) {
        while (order[i] != i + 1) {
            std::swap(order[i], order[order[i] - 1]);
            inversions++;
        }
    }

    if (size % 2 == 1) {
        return inversions % 2 == 0;
    }
    else {
        return (inversions + zeroRow) % 2 == 1;
    }
}

int findManhattan(const int size, const std::vector<int>& tilesMapping, const std::vector<int>& goalMapping) {
    int sum = 0;

    for (int i = 1; i < size * size; i++) {
        sum += abs(tilesMapping[i] / size - goalMapping[i] / size);
        sum += abs(tilesMapping[i] % size - goalMapping[i] % size);
    }

    return sum;
}

int findCorrect(const int size, const std::vector<int>& tiles, const std::vector<int>& goal) {
    int cnt = 0;

    for (int i = 0; i < size * size; i++) {
        if (tiles[i] != 0 && tiles[i] == goal[i]) {
            cnt++;
        }
    }

    return cnt;
}

bool DFS(const int size, int cost, int correctPlaced, const int threshold, int& nextThreshold,
    const char prev, std::deque<char>& path,
    std::vector<int>& curr, const std::vector<int>& goal,
    std::vector<int>& currMapping, const std::vector<int>& goalMapping) {

    if (correctPlaced == size * size - 1) {
        return true;
    }

    if (cost > threshold) {
        nextThreshold = std::min(nextThreshold, cost);
        return false;
    }

    const int emptyX = currMapping[0] / size;
    const int emptyY = currMapping[0] % size;

    int deltaCorrect;
    int deltaManhattan;

    if (emptyX != size - 1 && prev != 'd') {
        path.push_back('u');

        int numX = emptyX + 1;
        int num = curr[numX * size + emptyY];

        if (curr[numX * size + emptyY] == goal[numX * size + emptyY]) {
            deltaCorrect = -1;
        }
        else if (curr[numX * size + emptyY] == goal[emptyX * size + emptyY]) {
            deltaCorrect = 1;
        }
        else {
            deltaCorrect = 0;
        }

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size);

        std::swap(curr[numX * size + emptyY], curr[emptyX * size + emptyY]);
        currMapping[0] = numX * size + emptyY;
        currMapping[num] = emptyX * size + emptyY;

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size)
            - deltaManhattan;

        if (DFS(size, 1 + cost + deltaManhattan, correctPlaced + deltaCorrect, threshold, nextThreshold, 'u',
            path, curr, goal, currMapping, goalMapping)) {
            return true;
        }

        std::swap(curr[numX * size + emptyY], curr[emptyX * size + emptyY]);
        currMapping[0] = emptyX * size + emptyY;
        currMapping[num] = numX * size + emptyY;

        path.pop_back();
    }

    if (emptyX != 0 && prev != 'u') {
        path.push_back('d');

        int numX = emptyX - 1;
        int num = curr[numX * size + emptyY];

        if (curr[numX * size + emptyY] == goal[numX * size + emptyY]) {
            deltaCorrect = -1;
        }
        else if (curr[numX * size + emptyY] == goal[emptyX * size + emptyY]) {
            deltaCorrect = 1;
        }
        else {
            deltaCorrect = 0;
        }

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size);

        std::swap(curr[numX * size + emptyY], curr[emptyX * size + emptyY]);
        currMapping[0] = numX * size + emptyY;
        currMapping[num] = emptyX * size + emptyY;

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size)
            - deltaManhattan;

        if (DFS(size, 1 + cost + deltaManhattan, correctPlaced + deltaCorrect, threshold, nextThreshold, 'd',
            path, curr, goal, currMapping, goalMapping)) {
            return true;
        }

        std::swap(curr[numX * size + emptyY], curr[emptyX * size + emptyY]);
        currMapping[0] = emptyX * size + emptyY;
        currMapping[num] = numX * size + emptyY;

        path.pop_back();
    }

    if (emptyY != size - 1 && prev != 'r') {
        path.push_back('l');

        int numY = emptyY + 1;
        int num = curr[emptyX * size + numY];

        if (curr[emptyX * size + numY] == goal[emptyX * size + numY]) {
            deltaCorrect = -1;
        }
        else if (curr[emptyX * size + numY] == goal[emptyX * size + emptyY]) {
            deltaCorrect = 1;
        }
        else {
            deltaCorrect = 0;
        }

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size);

        std::swap(curr[emptyX * size + numY], curr[emptyX * size + emptyY]);
        currMapping[0] = emptyX * size + numY;
        currMapping[num] = emptyX * size + emptyY;

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size)
            - deltaManhattan;

        if (DFS(size, 1 + cost + deltaManhattan, correctPlaced + deltaCorrect, threshold, nextThreshold, 'l',
            path, curr, goal, currMapping, goalMapping)) {
            return true;
        }

        std::swap(curr[emptyX * size + numY], curr[emptyX * size + emptyY]);
        currMapping[0] = emptyX * size + emptyY;
        currMapping[num] = emptyX * size + numY;

        path.pop_back();
    }

    if (emptyY != 0 && prev != 'l') {
        path.push_back('r');

        int numY = emptyY - 1;
        int num = curr[emptyX * size + numY];

        if (curr[emptyX * size + numY] == goal[emptyX * size + numY]) {
            deltaCorrect = -1;
        }
        else if (curr[emptyX * size + numY] == goal[emptyX * size + emptyY]) {
            deltaCorrect = 1;
        }
        else {
            deltaCorrect = 0;
        }

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size);

        std::swap(curr[emptyX * size + numY], curr[emptyX * size + emptyY]);
        currMapping[0] = emptyX * size + numY;
        currMapping[num] = emptyX * size + emptyY;

        deltaManhattan = abs(currMapping[num] / size - goalMapping[num] / size)
            + abs(currMapping[num] % size - goalMapping[num] % size)
            - deltaManhattan;

        if (DFS(size, 1 + cost + deltaManhattan, correctPlaced + deltaCorrect, threshold, nextThreshold, 'r',
            path, curr, goal, currMapping, goalMapping)) {
            return true;
        }

        std::swap(curr[emptyX * size + numY], curr[emptyX * size + emptyY]);
        currMapping[0] = emptyX * size + emptyY;
        currMapping[num] = emptyX * size + numY;

        path.pop_back();
    }

    return false;
}

std::deque<char> IDAStar(const int size, std::vector<int>& tiles, const std::vector<int>& goal) {
    std::vector<int> tilesMapping(size * size);
    std::vector<int> goalMapping(size * size);

    for (int i = 0; i < size * size; i++) {
        tilesMapping[tiles[i]] = i;
    }

    for (int i = 0; i < size * size; i++) {
        goalMapping[goal[i]] = i;
    }

    int correct = findCorrect(size, tiles, goal);
    int manhattan = findManhattan(size, tilesMapping, goalMapping);
    int threshold = manhattan;

    std::deque<char> path;

    while (true) {
        int nextThreshold = INT_MAX;

        if (DFS(size, manhattan, correct, threshold, nextThreshold, ' ',
            path, tiles, goal, tilesMapping, goalMapping)) {
            break;
        }

        threshold = nextThreshold;
    }

    return path;
}

int main() {
    int n, empty;

    std::cin >> n >> empty;

    int size = std::sqrt(n + 1);

    std::vector<int> tiles(size * size);

    for (int i = 0; i < size * size; i++) {
        std::cin >> tiles[i];
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<int> goal = getGoal(size, empty);

    if (!isSolvable(size, tiles)) {
        std::cout << -1 << std::endl;

        return 0;
    }

    std::deque<char> path = IDAStar(size, tiles, goal);

    auto finish = std::chrono::high_resolution_clock::now();

    std::cout << path.size() << std::endl;

    while (!path.empty()) {
        const char curr = path.front();
        path.pop_front();

        switch (curr) {
        case 'u':
            std::cout << "up" << std::endl;
            break;
        case 'd':
            std::cout << "down" << std::endl;
            break;
        case 'l':
            std::cout << "left" << std::endl;
            break;
        case 'r':
            std::cout << "right" << std::endl;
            break;
        default:;
        }
    }

    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
    std::cout << static_cast<double>(microseconds.count()) / 1000000 << std::endl;

    return 0;
}
