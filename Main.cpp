#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <limits>

#include "Graph.h"

static std::string graphToDot(const std::vector<std::vector<int>>& graph) {

	std::ostringstream oss;

	oss << "graph {" << '\n';

	for (int i = 0; i < graph.size(); i++) {
		oss << "\tn" << i << " -> ";

		const auto& row = graph[i];

		for (int j = 0; j < row.size(); j++)
			oss << "n" << row[j] << ((j == (row.size() - 1)) ? ";\n" : ", ");
	}

	oss << "}";

	return oss.str();
}

int main() {
	dsa::Graph g{};

	{
		std::vector<std::vector<int>> m = 
				{ { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
				{ 4, 0, 8, 0, 0, 0, 0, 11, 0 },
				{ 0, 8, 0, 7, 0, 4, 0, 0, 2 },
				{ 0, 0, 7, 0, 9, 14, 0, 0, 0 },
				{ 0, 0, 0, 9, 0, 10, 0, 0, 0 },
				{ 0, 0, 4, 14, 10, 0, 2, 0, 0 },
				{ 0, 0, 0, 0, 0, 2, 0, 1, 6 },
				{ 8, 11, 0, 0, 0, 0, 1, 0, 7 },
				{ 0, 0, 2, 0, 0, 0, 6, 7, 0 }
			};
		g.setMatrix(std::move(m));
	}

	auto sol = g.dijkstra(0);
	
	std::vector<int> path{};
	for (int i = 8; i != -1; i = std::get<0>(sol)[i]) {
		path.push_back(i);
	}
	std::reverse(path.begin(), path.end());

	for (const auto& i : path)
		std::cout << i << ' -> ';

	std::cout << std::endl;

	return 0;
}