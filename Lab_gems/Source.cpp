#define N 10
#define numb_color 5
#include<vector>
#include<random>
#include<string>


std::random_device rd;
std::mt19937 gen(rd());

std::vector<std::string> Colors = { "red","blue","green","violet","pink" };
std::vector<std::vector<int>> Board(N, std::vector<int>(N, 0));

class Point {
private:
	int x;
	int y;
public:
	Point() :x(0), y(0) {}
	Point(int startX, int startY) : x(startX), y(startY) {}
};

class obj {
public:
	void replace(obj Another) {

	}
	void set_color(int color_) {
		color = color_;
	}
	int get_color() {
		return color;
	}
private:
	Point P;
	int color;
};

class scene {
private:
	int get_index(int row, int col) {
		return N * row + col;
	}
public:
	std::vector<obj> ALL_obj;
	scene() {
		ALL_obj.resize(N * N);
	}

	void init() {
		int C;
		for (int i = 0;i < N;i++) {
			for (int j = 0;j < N;j++) {
				bool flag = 0;
				do {
					std::uniform_int_distribution<int> distrib(0, 4);
					C = distrib(gen);
					if (j >= 2 &&
						ALL_obj[get_index(i, j - 1)].get_color() == C &&
						ALL_obj[get_index(i, j - 2)].get_color() == C) {
						flag = true;
					}

					if (i >= 2 &&
						ALL_obj[get_index(i - 1, j)].get_color() == C &&
						ALL_obj[get_index(i - 2, j)].get_color() == C) {
						flag = true;
					}
				} while (flag);

				ALL_obj[get_index(i, j)].set_color(C);
			}
		}
	}
};
int main() {
	return 0;
}