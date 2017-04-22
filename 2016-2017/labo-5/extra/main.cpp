
#include <iostream>
#include <fstream>
#include <random>

int main() {
	const int MAX = 1000;
	const int AMOUNT = 100000;

	int** hist = new int*[MAX];

	for (int i = 0; i < MAX; i++) {
		hist[i] = new int[MAX];
		for (int j = 0; j < MAX; j++) {
			hist[i][j] = 0;
		}
	}

	std::ofstream output("puntenverzameling.txt");
	if (!output.is_open()) {
		throw "Could not write to output file.";
	}

	srand((unsigned) time(0));

	for (int i = 0; i < AMOUNT; i++) {
		int x = (rand() % MAX);
		int y = (rand() % MAX);
		while (hist[x][y] == 1) {
			x = (rand() % MAX);
			y = (rand() % MAX);
		}
		hist[x][y] = 1;

		output << x << " " << y << "\r\n";
		std::cout << i << ": " << x << " " << y << "\r\n";
	}
	
	for (int i = 0; i < MAX; i++) {
		delete hist[i];
	}
	delete hist;

	return 0;
}

