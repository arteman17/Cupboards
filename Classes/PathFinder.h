#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

class PathFinder {
public:
    static std::vector<int> findAvailableMoves(int startPoint,
        const std::vector<std::pair<int, int>>& connections,
        const std::unordered_map<int, int>& occupiedPoints);

    static std::vector<int> findPath(int fromPoint, int toPoint,
        const std::vector<std::pair<int, int>>& connections,
        const std::unordered_map<int, int>& occupiedPoints);

private:
    static std::unordered_map<int, std::vector<int>> buildGraph(
        const std::vector<std::pair<int, int>>& connections);
};