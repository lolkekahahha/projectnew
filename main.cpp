#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>

using namespace std;

// Структура для хранения графа
class Graph {
public:
    int vertices; // количество вершин
    vector<vector<int>> adjMatrix; // матрица смежности

    Graph(int v) : vertices(v) {
        adjMatrix.resize(v, vector<int>(v, 0)); // инициализация матрицы смежности
    }

    // Чтение матрицы смежности из файла
    void readAdjacencyMatrix(const string &filename) {
        ifstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < vertices; ++i) {
                for (int j = 0; j < vertices; ++j) {
                    file >> adjMatrix[i][j];
                }
            }
            file.close();
        } else {
            cerr << "error" << endl;
        }
    }

    // Функция для обхода графа методом BFS
    void BFS(int start, ofstream &outFile) {
        vector<int> parent(vertices, -1); // для хранения родителей вершин
        vector<bool> visited(vertices, false); // для проверки посещенности
        queue<int> q; // очередь для BFS
        vector<tuple<int, int>> directEdges; // прямые ребра
        vector<tuple<int, int>> backEdges; // обратные ребра
        vector<tuple<int, int>> crossEdges; // поперечные ребра

        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            // Обработка всех соседей
            for (int i = 0; i < vertices; ++i) {
                if (adjMatrix[node][i] == 1) {
                    if (!visited[i]) {
                        // Прямое ребро (ребро дерева обхода)
                        parent[i] = node;
                        visited[i] = true;
                        q.push(i);
                        directEdges.push_back(make_tuple(node, i));
                    } else {
                        // Тип ребра зависит от того, является ли вершина предком
                        if (parent[node] != i) {
                            if (parent[i] == node) {
                                // Обратное ребро
                                backEdges.push_back(make_tuple(node, i));
                            } else {
                                // Поперечное ребро
                                crossEdges.push_back(make_tuple(node, i));
                            }
                        }
                    }
                }
            }
        }

        // Запись результатов в файл
        outFile << "Прямые ребра (ребра дерева обхода):\n";
        for (const auto &edge : directEdges) {
            outFile << get<0>(edge) << " -> " << get<1>(edge) << endl;
        }

        outFile << "\nОбратные ребра:\n";
        for (const auto &edge : backEdges) {
            outFile << get<0>(edge) << " -> " << get<1>(edge) << endl;
        }

        outFile << "\nПоперечные ребра:\n";
        for (const auto &edge : crossEdges) {
            outFile << get<0>(edge) << " -> " << get<1>(edge) << endl;
        }
    }
};

int main() {
    // Открываем файл для записи результата
    ofstream outFile("C:/Users/madam/Documents/lab po proge/20lab/output.txt");

    if (!outFile.is_open()) {
        cerr << "Не удалось открыть файл для записи!" << endl;
        return -1;
    }

    // Пример чтения графа
    int vertices;
    ifstream inFile("C:/Users/madam/Documents/lab po proge/20lab/graph.txt"); // Файл с графом (матрица смежности)
    if (inFile.is_open()) {
        inFile >> vertices;
        Graph g(vertices);
        g.readAdjacencyMatrix("C:/Users/madam/Documents/lab po proge/20lab/graph.txt");

        int start;
        inFile >> start; // Вершина начала обхода
        inFile.close();

        // Запуск BFS и запись результата в файл
        g.BFS(start, outFile);
    } else {
        cerr << "Не удалось открыть файл для чтения!" << endl;
    }

    outFile.close();
    return 0;
}