#pragma once
#include<iostream>
#include<time.h>
#include<stdlib.h>
using namespace std;

class Cube {
private:
	bool ifHaveBomb;												//该方块是否含有炸弹
	bool ifOpen;													//该方块有无被玩家翻开
	int nearBombNumber;												//该方块周围8格含有炸弹的方块的数量
public:
	Cube() { ifHaveBomb = false;ifOpen = false;nearBombNumber = 0; }
	void setOpen() { ifOpen = true; }								//将Open的值改为true
	bool getOpen() { return ifOpen; }								//获取ifOpen的值
	void setNearBombNumber(int number) { nearBombNumber = number; }	//给nearBombNumber赋值
	void haveBomb() { ifHaveBomb = true; }							//给方块放置地雷
	bool getIfHaveBomb() { return ifHaveBomb; }						//获取ifHaveBomb的值
	int getNearBombNumber() { return nearBombNumber; }				//获取nearBombNumber的值
	void resetCube() { ifHaveBomb = false;ifOpen = false;nearBombNumber = 0; }
																	//重置方块
};

class Map {
public:
	int MAXX;														//雷区的宽
	int MAXY;														//雷区的高
	int BOMBNUMBER;													//地雷数量
	Cube **map;
public:
	Map(int maxx, int maxy, int bn);
	~Map();
	int getBN() { return BOMBNUMBER; }								//获取BOMBNUMBER的值
	void setBomb();													//生成bombNumber个炸弹并且放进随机的方块中
	void show();													//显示地雷阵
	int checkAndSetNearBombNumber(int x, int y);					//检查并设置当前方块周围的雷数量
	void gameStart();												//初始化游戏
	bool player(bool &life);										//玩家输入坐标翻开方块
	void autoOpen(int x, int y);									//玩家翻开的方块为不含雷且周围无雷的方块时，自动翻开周围无雷的方块
	void message(bool life);										//玩家游戏结束后输出的信息
	bool ifWin();													//判断玩家是否扫雷成功
	void showBomb();												//游戏结束后显示地雷位置
};

Map::Map(int maxx, int maxy, int bn) {
	MAXX = maxx;
	MAXY = maxy;
	BOMBNUMBER = bn;
	map = new Cube*[MAXX];
	for (int i = 0;i < MAXX;i++)
		map[i] = new Cube[MAXY];
}

Map::~Map() {
	delete []map;
}

void Map::setBomb() {
//生成bombNumber个炸弹并且放进随机的方块中
	srand(time(0));
	int temp = getBN();
	while (temp--) {
		int x = rand() % MAXX, y = rand() % MAXY;
		if (map[x][y].getIfHaveBomb() != true) map[x][y].haveBomb();
		else temp++;
	}
}

int Map::checkAndSetNearBombNumber(int x, int y) {
//检查当前方块周围的雷数量
	int num = 0;

	if (map[x][y].getIfHaveBomb() == true) return 0;				//若该方块有地雷，则不用判断它周围有几个雷
	else {															//用两个循环当前方块周围8格扫一遍
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int nx = x + i;
				int ny = y + j;
				if (!(ny == y && nx == x) && (nx >= 0 && nx <= MAXX - 1) && (ny >= 0 && ny <= MAXY - 1)) {
					if (map[nx][ny].getIfHaveBomb()) num++;
				}
			}
		}
		map[x][y].setNearBombNumber(num);							//设置该方块附近的地雷的数量
		return 0;
	}
}

void Map::gameStart() {
//初始化游戏
	for (int i = 0;i < MAXX;i++) {
		for (int j = 0;j < MAXY;j++) {
			map[i][j].resetCube();
		}
	}
	setBomb();
	for (int i = 0;i < MAXX;i++) {
		for (int j = 0;j < MAXY;j++) {
			checkAndSetNearBombNumber(i, j);
		}
	}
}

void Map::show() {
	for (int i = 0;i < MAXX;i++) {
		for (int j = 0;j < MAXY;j++) {
			if (map[i][j].getOpen() == true) {
				if (map[i][j].getIfHaveBomb() == false) {			//挖开的无雷的方块显示该方块周围多少个方块含雷，若为0则显示空格
					if (map[i][j].getNearBombNumber() == 0) cout << " " << " ";
					else cout << map[i][j].getNearBombNumber() << " ";
				}
				if (map[i][j].getIfHaveBomb() == true) cout << "X" << " ";	
																	//挖开的有雷的方块显示X
			}
			if (map[i][j].getOpen() == false) cout << "?" << " ";	//未挖开的方块显示?
		}
		cout << endl;
	}
}

void Map::autoOpen(int x, int y) {
//玩家翻开的方块为不含雷且周围无雷的方块时，自动翻开周围无雷的方块
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int nx = x + i;
			int ny = y + j;
			if (!(ny == y && nx == x) && (nx >= 0 && nx <= MAXX - 1) &&
				(ny >= 0 && ny <= MAXY - 1) && map[nx][ny].getOpen() == false) {
				if (map[nx][ny].getNearBombNumber() == 0) {			//递归自动翻开周围无雷方块
					map[nx][ny].setOpen();
					autoOpen(nx, ny);
				}
				else map[nx][ny].setOpen();
			}
		}
	}

}

bool Map::player(bool &life) {
//玩家输入坐标翻开方块
	int x, y;
	cout << "请输入坐标(x,y),x和y用空格隔开" << endl;
	cin >> x >> y;
	if ((x < 0) || (x > MAXX - 1) || (y < 0) || (y > MAXY - 1)) {	//当玩家输入的坐标超出范围时
		cout << "该坐标不存在，请重新输入坐标" << endl;
	}
	if (map[x][y].getIfHaveBomb() == true) {						//当玩家翻开的方块有地雷时
		map[x][y].setOpen();
		show();
		life = false;
		return false;
	}
	else if (map[x][y].getOpen() == false) {						//当玩家翻开的方块无雷时
		if (map[x][y].getNearBombNumber() == 0) {					//玩家翻开的方块为不含雷且周围无雷的方块时，自动翻开周围无雷的方块
			autoOpen(x, y);
			map[x][y].setOpen();
			show();
		}
		else {
			map[x][y].setOpen();
			show();
		}
	}
	else if (map[x][y].getOpen() == true) {							//当玩家输入已翻开方块的坐标时
		show();
		cout << "该方块已被挖开，请再次输入坐标" << endl;
	}
	ifWin();
	return true;
}

void Map::message(bool result) {
	if (result == true) {											//玩家胜利时输出的信息
		showBomb();
		cout << "Winner Winner Chicken Dinner!" << endl;
	}
	else {															//玩家失败时输出的信息
		showBomb();
		cout << "Better Luck Next Time!" << endl;
	}
}

bool Map::ifWin() {
//判断玩家是否扫雷成功达到游戏结束条件
	int num = 0;
	for (int i = 0;i < MAXX;i++) {
		for (int j = 0;j < MAXY;j++) {
			if (map[i][j].getOpen() == false) {
				num++;
			}
		}
	}
	if (num == BOMBNUMBER) return true;
	else return false;
}

void Map::showBomb() {
//游戏结束后显示地雷位置
	cout << endl;
	for (int i = 0;i < MAXX;i++) {
		for (int j = 0;j < MAXY;j++) {
			if (map[i][j].getIfHaveBomb() == true) map[i][j].setOpen();
		}
	}
	show();
}