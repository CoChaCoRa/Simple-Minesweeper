#include<iostream>
#include"Minesweeper.h"
using namespace std;

int main() {
	const int X = 10;
	const int Y = 10;
	const int BN = 10;
	
	Map A(X, Y, BN);
	A.gameStart();
	A.show();
	bool life = true;
	while (A.player(life) && !A.ifWin()) {}
	A.message(life && A.ifWin());
	int flag;
	cout << "输入1重玩，输入0结束" << endl;
	cin >> flag;
	while (flag != 0) {
		A.gameStart();
		A.show();
		bool life = true;
		while (A.player(life) && !A.ifWin()) {}
		A.message(life && A.ifWin());
		cout << "输入1重玩，输入0结束" << endl;
		cin >> flag;
	}
}