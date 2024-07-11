#pragma once

#include <vector>
#include <queue>
#include <bitset>
#include <cstring>
#include <sstream>
#include <functional>
#include <assert.h>

namespace dsa {
	class Graph {
	private:
		std::vector<std::vector<int>> graph_matrix;

	public:
		Graph()
			:graph_matrix{} {
		}

		~Graph() = default;

	public:
		void link(int node, const std::vector<int>& neighbours) {
			if (node >= graph_matrix.size())
				return;

			for (const auto& i : neighbours)
				graph_matrix.at(node).push_back(i);
		}

		void newNode(const std::vector<int>& neighbours) {
			graph_matrix.push_back(neighbours);
		}

		void reserve(int size) {
			int prev_size = graph_matrix.size();
			for (int i = 0; i < (size - prev_size); i++)
				graph_matrix.push_back({});
		}

		void setMatrix(std::vector<std::vector<int>>&& m) {
			graph_matrix = std::move(m);
		}

		void BFSTraversal(int start, const std::function<void(int)>& consumer) {
			if (start >= graph_matrix.size())
				return;

			std::queue<int> node_queue{};
			node_queue.push(start);

			// this way is easier than using a vector.reserve() or an array, just DONT FORGET
			// TO DELETE IT.
			assert(graph_matrix.size() == graph_matrix[0].size());
			bool* visited = new bool[graph_matrix.size()];
			memset(visited, 0, graph_matrix.size());

			visited[start] = true;

			while (!node_queue.empty()) {
				int node = node_queue.front();
				node_queue.pop();

				consumer(node);

				for (int i = 0; i < graph_matrix[node].size(); i++) {
					if (i != 0 && !visited[i]) {
						node_queue.push(i);
						visited[i] = true;
					}
				}
			}

			delete[] visited;
		}

	public:
		std::string graphToDot() {
			std::ostringstream oss{};

			oss << "digraph G {" << '\n';

			for (int i = 0; i < graph_matrix.size(); i++) {
				for (int j = 0; j < graph_matrix[i].size(); j++) {
					int distance = graph_matrix[i][j];
					if (distance == 0)
						continue;
					oss << '\t' << i << "->" << j << " [label=" << distance << "]\n";
				}
			}

			return oss.str();
		}

	private:
		/*
		* Node - Distance Pair, the first int is the node and the second is the distance
		*/
		using NodeDistancePair = std::tuple<int, int>;

		struct Compare {
			bool operator()(const NodeDistancePair& a, const NodeDistancePair& b) {
				return std::get<1>(a) > std::get<1>(b);
			}
		};

	public:

		using DijkstraSolution = std::tuple<std::vector<int>, std::vector<int>>;

		/*
		 * All shortest-path algorithms
		 */

		DijkstraSolution dijkstra(int start) {
			std::priority_queue <NodeDistancePair, std::vector<NodeDistancePair>, Compare> queued_nodes{};


			std::vector<int> distances(graph_matrix.size(), INT_MAX);
			std::vector<bool> visited(graph_matrix.size(), false);
			std::vector<int> parents(graph_matrix.size(), -1);

			queued_nodes.push({ start, 0 });
			distances[start] = 0;

			while (!queued_nodes.empty()) {
				auto [node, current_distance] = queued_nodes.top();
				queued_nodes.pop();
				visited[node] = true;

				if (current_distance > distances[node])
					continue;

				for (int i = 0; i < graph_matrix[node].size(); i++) {
					int new_posible_distance = distances[node] + graph_matrix[node][i];
					bool not_visited_yet = visited[i] == false;
					bool is_valid_node = graph_matrix[node][i] > 0 && distances[node] < INT_MAX;

					if (visited[i] == false && is_valid_node && new_posible_distance < distances[i]) {
						distances[i] = new_posible_distance;
						parents[i] = node;
						queued_nodes.push({ i, distances[i] });
					}
				}
			}

			return std::make_pair(parents, distances);
		}

	private:
		int minDistance(const std::vector<int>& distances, const std::vector<bool>& visited) {

			assert(distances.size() == visited.size());

			int min = INT_MAX;
			int node = 0;
			for (int i = 0; i < distances.size(); i++)
				if (distances[i] <= min && visited[i] == false)
					min = distances[i], node = i;
			return node;
		}
	};
}