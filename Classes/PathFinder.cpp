#include "PathFinder.h"
#include <algorithm>
#include "cocos2d.h"

std::unordered_map<int, std::vector<int>> PathFinder::buildGraph(
    const std::vector<std::pair<int, int>>& connections) {

    std::unordered_map<int, std::vector<int>> graph;

    for (const auto& connection : connections) {
        graph[connection.first].push_back(connection.second);
        graph[connection.second].push_back(connection.first);
    }

    return graph;
}

std::vector<int> PathFinder::findAvailableMoves(int startPoint,
    const std::vector<std::pair<int, int>>& connections,
    const std::unordered_map<int, int>& occupiedPoints) {

    auto graph = buildGraph(connections);
    std::vector<int> availableMoves;
    std::unordered_set<int> visited;

    // Используем BFS с проверкой всего пути
    std::queue<int> queue;
    queue.push(startPoint);
    visited.insert(startPoint);

    while (!queue.empty()) {
        int currentPoint = queue.front();
        queue.pop();

        if (currentPoint != startPoint && occupiedPoints.count(currentPoint) == 0) {
            availableMoves.push_back(currentPoint);
        }

        // Проверяем всех соседей
        for (int neighbor : graph[currentPoint]) {
            if (visited.find(neighbor) != visited.end() || 
                occupiedPoints.count(neighbor) != 0) continue;
            visited.insert(neighbor);
            queue.push(neighbor);
        }
    }

    return availableMoves;
}

std::vector<int> PathFinder::findPath(int fromPoint, int toPoint,
    const std::vector<std::pair<int, int>>& connections,
    const std::unordered_map<int, int>& occupiedPoints) {

    auto graph = buildGraph(connections);

    // Если точки совпадают
    if (fromPoint == toPoint) {
        return { fromPoint };
    }

    // Если конечная точка занята - путь невозможен
    if (occupiedPoints.count(toPoint) > 0) {
        return {};
    }

    std::unordered_map<int, int> parent;
    std::queue<int> queue;
    std::unordered_set<int> visited;

    queue.push(fromPoint);
    parent[fromPoint] = -1;
    visited.insert(fromPoint);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (current == toPoint) {
            break;
        }

        for (int neighbor : graph[current]) {
            // Пропускаем посещенные точки
            if (visited.find(neighbor) != visited.end()) {
                continue;
            }

            // Пропускаем занятые точки (кроме начальной и конечной)
            if (neighbor != toPoint && occupiedPoints.count(neighbor) > 0) {
                continue;
            }

            parent[neighbor] = current;
            visited.insert(neighbor);
            queue.push(neighbor);
        }
    }

    // Восстановление пути
    std::vector<int> path;
    if (parent.find(toPoint) != parent.end()) {
        int current = toPoint;
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}