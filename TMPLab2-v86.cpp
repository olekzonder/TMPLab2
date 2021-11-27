
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

void getArray(int** matrix, int n, int m) //Ввести массив из файла удобнее, чем из клавиатуры
{
	ifstream file("matrix.txt");
	if (!file)
		cout << "Файл matrix.txt не найден." << endl;
	else
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				file >> matrix[i][j];
}

int fordfalkerson(int n, int m, int** matrix, bool* used, queue <int> bfsbuf, int from, int to) {
	cout << "----------" << endl;
	vector <vector <int>> positions; //Хранение положений в матрице инцидентности для её последующей модификации
	int* parents = new int[n];
	parents[from] = -1;
	int* lengths = new int[n];
	fill_n(lengths, n, 0);
	//В основе алгоритма лежит поиск в ширину (BFS)
	while (!bfsbuf.empty()) {
		int cur = bfsbuf.front(); //Первый элемент очереди
		bfsbuf.pop(); //Удаление из очереди
		used[cur] = true; //Вершина уже использовалась
		for (int i = 0; i < m; i++) {
			if (matrix[cur][i] <= 0) continue;
			int j;
			for (j = 0; j < n; j++) if (j != cur && matrix[j][i] != 0) break;
			if (used[j] == true) continue;
			lengths[j] = matrix[cur][i];
			parents[j] = cur;
			positions.push_back({ cur,j,i });
			bfsbuf.push(j);
			used[j] = true;
		}
	}
	//Поиск пути
	vector<int> path, pathlen;
	int minimum;
	if (!used[to]) return -1; //Путь не найден
	else {
		for (int i = to; i != -1; i = parents[i]) {
			path.push_back(i); //Определение пути, пробегаясь по массиву
			if (lengths[i] != 0) pathlen.push_back(lengths[i]);
		}
		reverse(path.begin(), path.end());
		reverse(pathlen.begin(), pathlen.end());
		cout << "Найденный путь: ";
		for (int i = 0; i < path.size(); i++) cout << path[i] << ' ';
		cout << endl;
		cout << "Длины путей: ";
		for (int i = 0; i < pathlen.size(); i++) cout << pathlen[i] << ' ';
		minimum = *min_element(pathlen.begin(), pathlen.end());
		cout << endl;
		cout << "Минимальная пропускная способность Cmin: " << minimum;
	}
	//Модификация матрицы инцидентности
	for (int i = 0; i < path.size() - 1; i++) {
		for (int j = 0; j < positions.size(); j++) {
			if (positions[j][0] == path[i] && positions[j][1] == path[i + 1]) {
				matrix[positions[j][0]][positions[j][2]] -= minimum;
				matrix[positions[j][1]][positions[j][2]] += minimum;
			}
		}
	}
	cout << endl;
	return minimum;
}

void print(int** matrix, int n, int m) {
	cout << endl << "Используемая матрица инцидентности: " << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) cout << matrix[i][j] << ' ';
		cout << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	int n, m;
	cout << "Введите кол-во вершин > ";
	cin >> n;
	cout << "Введите кол-во дуг > ";
	cin >> m;
	int** matrix;
	matrix = new int* [n];

	for (int i = 0; i < n; i++) {
		matrix[i] = new int[m];
	}
	getArray(matrix, n, m);
	print(matrix, n, m);
	bool* used = new bool[n];
	int* parents = new int[n];
	int* lengths = new int[n];
	int from, to;
	cout << "Источник > ";
	cin >> from;
	cout << "Сток > ";
	cin >> to;
	queue <int> bfsbuf;
	bfsbuf.push(from);
	fill_n(used, n, false);
	int stream;
	int answer = 0;
	while (true) {
		stream = fordfalkerson(n, m, matrix, used, bfsbuf, from, to);
		if (stream == -1) break;
		answer += stream;
		fill_n(used, n, false);
	}

	if (answer == 0) cout << "Невозможно определить минимальный разрез сети!";
	else cout << "Минимальный разрез сети: " << answer;
	return 0;
}