#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <stack>
#include <queue>
using namespace std;

struct TreeNode {
	string data;
	TreeNode* leftChild;
	TreeNode* rightChild;
};

//运算符的优先级
int Level(char s) {
	int level = 0;
	switch (s) {
	case '+':
	case '-':
		level = 1;
		break;
	case '*':
	case '/':
		level = 2;
		break;
	//先考虑加减乘除及带括号，在考虑指数，幂运算等等
	default:
		break;
	}
	return level;
}

//中缀转后缀的实现
void InToPos(string s, queue<char>& q) {//队列中存入子符串数字和运算符
	int size = s.length();//这个length()函数的内部机制是怎么实现的，原理是什么？
	stack<char> oper;//栈中是压入运算符和左括号的
	for (int i = 0; i < size; i++) {
		if (s[i] >= '0' && s[i] <= '9') { 
			q.push(s[i]); 
			while (s[i + 1] >= '0' && s[i + 1] <= '9') {
				q.push(s[++i]);
			}
			q.push(' ');
		}
		//在这里需要做出改变
		//问题在于输出的时候和后续需要进行后缀表达式计算的时，要用整型，而不是string。so transform(string->整型) 
		//还是在于怎么把比如一个子符串34转换为整型数34
		else {
			int level = Level(s[i]);
			if (oper.empty()) oper.push(s[i]);	
			else {
				if (s[i] == '(') oper.push(s[i]);
				else if (s[i] == ')') {
					while (oper.top() != '(') {
						q.push(oper.top());
						q.push(' ');
						oper.pop();
					}
					oper.pop();
				}
				else {
					while (!oper.empty()) {
						if (oper.top() == '(') break;
						else {
							int level1 = Level(oper.top());//栈顶的
							if (level > level1) break;//level 是当前的 level1是栈顶的，前一个的
							else {
								q.push(oper.top());
								q.push(' ');
								oper.pop();
							}
						}
					}
					oper.push(s[i]);
				}
			}
		}
	}
	while (!oper.empty()) {
		q.push(oper.top());
		q.push(' ');
		oper.pop();
	}
}

//后缀表达式的计算
int CalculateByPos(queue<char> q) {
	stack<int> num;
	while (!q.empty()) {
		if (q.front() >= '0' && q.front() <= '9') {
			int temp = 0;
			temp = q.front() - '0';
			q.pop();
			while (q.front() >= '0' && q.front() <= '9') {
				temp = temp * 10 + q.front() - '0';
				q.pop();
			}
			num.push(temp);
			q.pop();//此处的删除是删的字符数后的空格
		}
		else {//加减乘除运算
			int num1 = num.top();
			num.pop();
			int num2 = num.top();
			num.pop();
			int num3 = 0;//在这里要保存到小数点后四位，需更改
			switch (q.front()) {
			case '+':
				num3 = num2 + num1;
				break;
			case '-':
				num3 = num2 - num1;
				break;
			case '*':
				num3 = num2 * num1;
				break;
			case '/':
				num3 = num2 / num1;
				break;
			}
			num.push(num3);
			q.pop();//此处删除的是运算符
			q.pop();//此处删除的是运算符后的空格
		}
	}
	return num.top();
}

//后缀转树的实现，只进行转换的实现
TreeNode* PosToTree(queue<char> q) {
	stack<TreeNode*> Node;
	while (!q.empty()) {
		//if (q.front() == ' ') continue;//此处需更改
		if (q.front() >= '0' && q.front() <= '9') {
			int count = 0;
			string full;
			full = q.front();
			q.pop();
			while (q.front() >= '0' && q.front() <= '9') {
				full.push_back(q.front());
				q.pop();
			}
			q.pop();//此处删除的是字符数后的空格
			TreeNode* temp = new TreeNode();
			if (temp == NULL) return NULL;
			temp->data = full;
			temp->leftChild = NULL;
			temp->rightChild = NULL;
			Node.push(temp);
		}
		else {
			//先用一个节点A存运算符，然后最队列中删掉，再删掉运算符后的空格
			//在A的右孩子为Node.top(),在Node.pop(),在A的左孩子为Node.top(),在Node.pop(),在把A节点压入栈中
			TreeNode* temp = new TreeNode();
			temp->data = q.front();
			q.pop();//此处删除的是运算符
			q.pop();//此处删除的是运算符后的空格
			temp->rightChild = Node.top();
			Node.pop();
			temp->leftChild = Node.top();
			Node.pop();
			Node.push(temp);
		}
	}
	return Node.top();
}

//树的中序遍历 验证构建的树是否正确
void InOrder(TreeNode* t) {
	if (t) {
		InOrder(t->leftChild);
		cout << t->data << " ";
		InOrder(t->rightChild);
	}
}

/*-----树表达式的计算------*/

//子符串数转整型数
int StringToInt(TreeNode* t) {
	string s = t->data;
	int size = s.length();
	int num = 0;
	for (int i = 0; i < size; i++) {
		num = num*10 + s[i] - '0';
	}
	return num;
}

//用后序遍历的思想计算树表达式
void PosOrderUseCal(TreeNode* t) {
	if (t) {
		PosOrderUseCal(t->leftChild);
		PosOrderUseCal(t->rightChild);
		//写一个字符串数转整型数的函数，传进去的是节点
		if (t->leftChild != NULL && t->rightChild != NULL) {
			int num1 = StringToInt(t->leftChild);//左
			int num2 = StringToInt(t->rightChild);//右
			int num3 = 0;
			if (t->data == "+") num3 = num1 + num2;
			else if (t->data == "-") num3 = num1 - num2;
			else if (t->data == "*") num3 = num1 * num2;
			else if (t->data == "/") num3 = num1 / num2;
			//再把得到的整型数(num3)转为string型数
			string s;
			while (num3) {
				int temp1 = num3 % 10;
				num3 /= 10;
				s.push_back(temp1 + '0');
			}
			int size = s.length();
			string s1;
			for (int i = size - 1; i >= 0; i--) {
				s1.push_back(s[i]);
			}
			//在把t节点指向数据中的操作符改为s1;
			t->data = s1;
			//再把t节点的两个子节点搞为空
			t->leftChild = NULL;
			t->rightChild = NULL;
		}
	}
}

//树表达式的计算
int CalculateByTree(TreeNode* T) {
	TreeNode* temp = new TreeNode();
	temp = T;
	PosOrderUseCal(temp);
	//再把temp中的string类型数转换为int型数
	int answer=StringToInt(temp);
	delete temp;
	return answer;
}

/*-----检验表达式是否合格-----*/

//1.检验该表达式是否合格(括号的匹配及运算符运用的是否合格)
//2.传进来的是最初的string s;
//3.对于圆括号而言用栈便可以解决。而对于运算符检验，运算符的左右两边
//只需要有字符便可以，但是该字符不可以是其他的运算符
//需要开两个栈，一个是关于圆括号的栈，一个是关于运算符的栈，最好分开。思考合在一起的情况。

bool IsOperator(char oper) {
	return (oper == '+' || oper == '-' || oper == '*' || oper == '/');
}

bool JudgeForOper(const string s) {
	stack<char> oper;
	int size = s.length();
	for (int i = 0; i < size; i++) {
		if (s[i] == '(') oper.push(s[i]);
		else if (s[i] == ')') oper.pop();
	}
	if (!oper.empty()) return false;
	else return true;
}

bool JudgeForBracket(const string s) {
	stack<char> bracket;
	int size = s.length();
	if (IsOperator(s[size - 1])||IsOperator(s[0])) return false;
	bracket.push(s[0]);
	bool judge = 1;//判断整体的
	for (int i = 1; i < size - 1; i++) {
		if (IsOperator(s[i])&& (IsOperator(bracket.top()) || IsOperator(s[i + 1]))) {
			judge = 0;
			break;
		}
		else bracket.push(s[i]);
	}
	if (judge == 1) return true;
	else return false;
}

bool Judge(const string s) {
	return (JudgeForOper(s) && JudgeForBracket(s));
}

int main() {
	string s;
	cin >> s;

	//先检验表达式是否标准
	if (!Judge(s)) {
		cout << "非法表达式，无法计算！" << endl;
		return 0;
	}

	queue<char> posOrder;
	InToPos(s, posOrder);

	int answer = CalculateByPos(posOrder);
	cout << s <<" = " <<answer << endl;

	TreeNode* T = PosToTree(posOrder);//该问题出在子符串的处理，已解决
	cout << "该树的中序遍历:";
	InOrder(T);
	cout << endl;

	cout << "对" << s << "用树表达式计算的结果为：" << CalculateByTree(T) << endl;
	
	while (!posOrder.empty()) {
		cout << posOrder.front();
		posOrder.pop();
	}

	return 0;
}
