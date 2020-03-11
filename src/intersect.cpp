#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <list>
#include <math.h>
#include <map>

using namespace std;
class line {
	public:
		double a;
		double b;
		int i;
		line(double x1,double y1,double x2,double y2 ) {
			if (x1 == x2) {
				i = 1;
				a = x1;
				b = 0;
			}
			else {
				i = 0;
				a = (y1 - y2) / (x1 - x2);
				b = y1 - a * x1;
			}
		}
};

class circle {
	public:
		double x;
		double y;
		double r;
		circle(double a,double b,double c) {
			x = a;
			y = b;
			r = c;
		}
};

class Node {
private: 
	double x;
	double y;
public:
	void setx(double a) {
		x = a;
	}
	void sety(double b) {
		y = b;
	}
	double getx() {
		return x;
	}
	double gety() {
		return y;
	}
};

int update(Node node, map<double, map<double, Node>> *Nodemap) {
	if ((*Nodemap).count(node.getx()) == 0) {
		map<double, Node> temp;
		temp.insert(pair<double, Node>(node.gety(),node));///mapStudent.insert(pair<int, string>(000, "student_zero"));
		(*Nodemap).insert(pair<double, map<double, Node>>(node.getx(), temp));
		return 1;
	}
	else {
		map<double, Node> temp = (*Nodemap)[node.getx()];
		if (temp.count(node.gety()) == 0) {
			temp[node.gety()] = node;
			(*Nodemap)[node.getx()] = temp;
			return 1;
		}
		else {
			return 0;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	string input;
	string output;
	string buffer;
	char type;
	double x1, y1, x2, y2, x, y, r;
	int count = 0;
	Node node;
	list<line> L1;
	list <line>::iterator L1_iter;
	list<circle> L2;
	list<circle>::iterator L2_iter;
	map<double, map<double, Node>> Nodemap;
	input = argv[2];
	output = argv[4];
	ifstream infile;
	infile.open(input);
	ofstream outfile;
	outfile.open(output);
	if (infile.eof() || outfile.eof()) {
		cout << "Open file failed" << endl;
	}
	getline(infile, buffer);
	int n = stoi(buffer);
	int i = 0;
	while (i < n) {
		getline(infile, buffer);
		if (buffer[0] == 'C') {
			if (sscanf(buffer.c_str(), "%c %lf %lf %lf", &type, &x, &y, &r) == NULL) {
				return 0;
			}
			circle cir(x,y,r);
			L2.push_back(cir);
		}
		else {
			if (sscanf(buffer.c_str(), "%c %lf %lf %lf %lf", &type, &x1, &y1, &x2, &y2) == NULL) {
				return 0;
			}
			line li(x1, y1, x2, y2);
			L1.push_back(li);
		}
		i++;
	}
	for (L1_iter = L1.begin(); L1_iter != L1.end(); L1_iter++) {///line with line
			list <line>::iterator temp = L1_iter;
			for (++temp; temp != L1.end(); temp++) {
				if (temp == L1.end()) {
					break;
				}
				if ((*L1_iter).i == 0 && (*temp).i == 0) {
					if ((*L1_iter).a == (*temp).a) {
						continue;
					}
					x = ((*L1_iter).b - (*temp).b) / ((*temp).a - (*L1_iter).a);
					y = (*L1_iter).a * x + (*L1_iter).b;
				}
				else if ((*L1_iter).i == 1 && (*temp).i == 0) {
					x = (*L1_iter).a;
					y = (*temp).a * x + (*temp).b;
				}
				else if ((*L1_iter).i == 0 && (*temp).i == 1) {
					x = (*temp).a;
					y = (*L1_iter).a * x + (*L1_iter).b;
				}
				else {
					continue;
				}
				node.setx(x);
				node.sety(y);
				count = update(node, &Nodemap) + count;
			}
	}
	double a, b, c_x, c_y;
	for (L1_iter = L1.begin(); L1_iter != L1.end(); L1_iter++) {//////line and circle
		a = (*L1_iter).a;
		b = (*L1_iter).b;
		i = (*L1_iter).i;
		for (L2_iter = L2.begin(); L2_iter != L2.end(); L2_iter++) {
			c_x = (*L2_iter).x;
			c_y = (*L2_iter).y;
			r = (*L2_iter).r;
			if (i == 1) {
				double temp = pow(r, 2) - pow(a - c_x, 2);
				if (temp < 0) {
					continue;
				}
				else if (temp == 0) {
					x = a;
					y = c_y;
				}
				else {
					x = a;
					y = sqrt(temp) + c_y;
					//////update
					node.setx(x);
					node.sety(y);
					count = update(node, &Nodemap) + count;
					y = -sqrt(temp) + c_y;
					//////update
					node.setx(x);
					node.sety(y);
					count = update(node, &Nodemap) + count;
				}
			}
			else {
				double temp1 = 1 + pow(a,2);
				double temp2 = 2 * a * (b - c_y) - 2 * c_x;
				double temp3 = pow(b - c_y, 2) + pow(c_x, 2) - pow(r, 2);
				double delt = pow(temp2, 2) - 4 * temp1 * temp3;
				if (delt < 0) {
					continue;
				}
				else if (delt == 0) {
					x = -temp2 / (2 * temp1);
					y = a * x + b;
					node.setx(x);
					node.sety(y);
					count = update(node, &Nodemap) + count;
				}
				else {
					x = (-temp2 + sqrt(delt)) / (2 * temp1);
					y = a * x + b;
					/////update;
					node.setx(x);
					node.sety(y);
					count = update(node, &Nodemap) + count;
					x = (-temp2 - sqrt(delt)) / (2 * temp1);
					y = a * x + b;
					/////update;
					node.setx(x);
					node.sety(y);
					count = update(node, &Nodemap) + count;
				}
			}
		}
	}
	double r1, r2,c;
	for (L2_iter = L2.begin(); L2_iter != L2.end(); L2_iter++) {///circle with circle
		x1 = (*L2_iter).x;
		y1 = (*L2_iter).y;
		r1 = (*L2_iter).r;
		list <circle>::iterator temp = L2_iter;
		for (++temp; temp != L2.end(); temp++) {
			if (temp == L2.end()) {
				break;
			}
			x2 = (*temp).x;
			y2 = (*temp).y;
			r2 = (*temp).r;
			a = 2 * (x1 - x2);
			b = 2 * (y1 - y2);
			c = pow(x2, 2) - pow(x1, 2) + pow(y2, 2) - pow(y1, 2) + pow(r1, 2) - pow(r2, 2);
			if (x1 == x2 && y1 == y2) {//same center
				continue;
			}
			else if ((fabs(a*x1+b*y1+c)/sqrt(pow(a,2)+pow(b,2))) > r1 || 
				(fabs(a * x2 + b * y2 + c) / sqrt(pow(a, 2) + pow(b, 2))) > r2){
				continue;
			}
			else {
				if (b == 0) {
					x = -c / a;
					y = y1 + sqrt(pow(r1, 2) - pow(x - x1, 2));
					/////update
					node.setx(x);
					node.sety(y);
					count = update(node, &Nodemap) + count;
					y = y2 + sqrt(pow(r1, 2) - pow(x - x1, 2));
					/////update
					node.setx(x);
					node.sety(y);
					count = update(node, &Nodemap) + count;
				}
				else {
					a = -a / b;
					b = -c / b;
					double temp1 = 1 + pow(a, 2);
					double temp2 = 2 * a * (b - y1) - 2 * x1;
					double temp3 = pow(b - y1, 2) + pow(x1, 2) - pow(r1, 2);
					double delt = pow(temp2, 2) - 4 * temp1 * temp3;
					if (delt < 0) {
						continue;
					}
					else if (delt == 0) {
						x = -temp2 / (2 * temp1);
						y = a * x + b;
					}
					else {
						x = (-temp2 + sqrt(delt)) / (2 * temp1);
						y = a * x + b;
						/////update;
						node.setx(x);
						node.sety(y);
						count = update(node, &Nodemap) + count;
						x = (-temp2 - sqrt(delt)) / (2 * temp1);
						y = a * x + b;
						/////update;
						node.setx(x);
						node.sety(y);
						count = update(node, &Nodemap) + count;
					}
				}
			}
		}
	}
	outfile << count;
	infile.close();
	outfile.close();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
