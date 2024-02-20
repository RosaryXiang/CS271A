#include "tabu.h"

void input(int& city_num, vector<vector<double>>& matrix,
           string testfile_name) {
    ifstream testfile(testfile_name);
    if (testfile.is_open()) {
        // Get number of city
        string s;
        getline(testfile, s);
        stringstream ss;
        ss << s;
        ss >> city_num;
        ss.clear();
        // get input matrix
        while (getline(testfile, s)) {
            ss << s;
            double tmp_db{};
            vector<double> tmp_vec;
            while (ss >> tmp_db) {
                tmp_vec.push_back(tmp_db);
            }
            matrix.push_back(tmp_vec);
            ss.clear();
        }
    }
    testfile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Please input testfile name.";
    }
    string testfile_name = argv[1];
    // declare
    vector<vector<double>> matrix;
    int city_num{};

    // get input
    input(city_num, matrix, testfile_name);

    // set number of iterations and tabu tenu
    int iterations = 100;
    int tabu_tenu = 5;
    tabu_search(matrix, iterations, tabu_tenu);
}
