#include <string>
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
	if (argc != 4) {
		cerr << "Usage: " << argv[0] << " [Result output file] [Answer file] [output accuracy file]" << endl;
		return 1;
	}
	ifstream result(argv[1], ios::in), answer(argv[2], ios::out);
	string res_mod, ans_mod;
	int correct = 0, total = 0;
	result >> res_mod;
	answer >> ans_mod;
	while(answer) {
		if (res_mod == ans_mod) ++correct;
		++ total;
		// Discard the probablility
		result >> res_mod;
		result >> res_mod;
		answer >> ans_mod;
	}
	ofstream output(argv[3], ios::out);
	output << correct / (double) total << endl;
	output.close();
	result.close();
	answer.close();
	return 0;
}
