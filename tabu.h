#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using namespace std;

// init current solution
vector<int> init_current_solution(int n) {
    vector<int> current_solution;
    for (int i = 0; i < n; i++) {
        current_solution.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(current_solution.begin(), current_solution.end(), g);
    return current_solution;
}

double calculate_total_distance(const vector<int> &path,
                                const vector<vector<double>> &matrix) {
    double total_distance{};
    for (int i = 0; i < matrix.size() - 1; i++) {
        total_distance += matrix[path[i]][path[i + 1]];
    }
    total_distance += matrix[path[0]][path[path.size() - 1]];
    return total_distance;
}

vector<vector<int>> generate_neighbors(const vector<int> &path) {
    vector<vector<int>> neighbors;
    int n = path.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            vector<int> tmp_path(path);
            swap(tmp_path[i], tmp_path[j]);
            neighbors.push_back(tmp_path);
        }
    }
    return neighbors;
}

int find_best_neighbor(const vector<vector<int>> &neighbors,
                       const vector<vector<double>> &matrix) {
    double min_cost{calculate_total_distance(neighbors[0], matrix)};
    int best_neighbor_index{};
    for (int i = 1; i < neighbors.size(); i++) {
        double tmp_cost = calculate_total_distance(neighbors[i], matrix);
        if (tmp_cost < min_cost) {
            best_neighbor_index = i;
            min_cost = tmp_cost;
        }
    }
    return best_neighbor_index;
}

pair<int, int> determine_move(const vector<int> &current_solution,
                              const vector<int> &neighbor) {
    int i{}, j{};
    bool is_first = true;
    if (current_solution.size() != neighbor.size()) {
        cout << "size error!" << endl;
        return {0, 0};
    }
    for (int k = 0; k < current_solution.size(); k++) {
        if (current_solution[k] != neighbor[k]) {
            if (is_first) {
                i = k;
                is_first = false;
            } else {
                j = k;
                return {i, j};
            }
        }
    }
    cout << "determine move fail!" << endl;
    return {0, 0};
}

void print_searching_result(const vector<int> &best_solution,
                            const double &best_cost) {
    cout << "Best Cost = " << best_cost << endl;
    cout << "Best Solution:" << endl;
    for (auto &i : best_solution) {
        cout << i << " ";
    }
    cout << endl;
}

void tabu_search(const vector<vector<double>> &matrix, int iterations,
                 int tabu_tenu) {
    int n = matrix.size();
    vector<int> current_solution = init_current_solution(n);
    vector<int> best_solution = current_solution;
    double best_cost = calculate_total_distance(current_solution, matrix);
    vector<vector<int>> tabu_list(n, vector<int>(n, 0));

    for (int iteration = 0; iteration < iterations; iteration++) {
        auto neighbors = generate_neighbors(current_solution);
        auto best_neighbor = neighbors[find_best_neighbor(neighbors, matrix)];

        auto best_neighbor_cost =
            calculate_total_distance(best_neighbor, matrix);
        bool improved = false;  // Could best neighbor improve best solution
        if (best_neighbor_cost < best_cost) {
            best_cost = best_neighbor_cost;
            best_solution = best_neighbor;
            improved = true;
        }

        auto move = determine_move(current_solution, best_neighbor);
        if (tabu_list[move.first][move.second] == 0 or improved) {
            current_solution = best_neighbor;
            for (auto &i : tabu_list) {
                for (auto &j : i) {
                    if (j > 0) {
                        j--;
                    }
                }
            }
            tabu_list[move.first][move.second] = tabu_tenu;
        }
    }
    print_searching_result(best_solution, best_cost);
}