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

//����������ȼ�
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
	//�ȿ��ǼӼ��˳��������ţ��ڿ���ָ����������ȵ�
	default:
		break;
	}
	return level;
}

//��׺ת��׺��ʵ��
void InToPos(string s, queue<char>& q) {//�����д����ӷ������ֺ������
	int size = s.length();//���length()�������ڲ���������ôʵ�ֵģ�ԭ����ʲô��
	stack<char> oper;//ջ����ѹ��������������ŵ�
	for (int i = 0; i < size; i++) {
		if (s[i] >= '0' && s[i] <= '9') { 
			q.push(s[i]); 
			while (s[i + 1] >= '0' && s[i + 1] <= '9') {
				q.push(s[++i]);
			}
			q.push(' ');
		}
		//��������Ҫ�����ı�
		//�������������ʱ��ͺ�����Ҫ���к�׺���ʽ�����ʱ��Ҫ�����ͣ�������string��so transform(string->����) 
		//����������ô�ѱ���һ���ӷ���34ת��Ϊ������34
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
							int level1 = Level(oper.top());//ջ����
							if (level > level1) break;//level �ǵ�ǰ�� level1��ջ���ģ�ǰһ����
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

//��׺���ʽ�ļ���
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
			q.pop();//�˴���ɾ����ɾ���ַ�����Ŀո�
		}
		else {//�Ӽ��˳�����
			int num1 = num.top();
			num.pop();
			int num2 = num.top();
			num.pop();
			int num3 = 0;//������Ҫ���浽С�������λ�������
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
			q.pop();//�˴�ɾ�����������
			q.pop();//�˴�ɾ�������������Ŀո�
		}
	}
	return num.top();
}

//��׺ת����ʵ�֣�ֻ����ת����ʵ��
TreeNode* PosToTree(queue<char> q) {
	stack<TreeNode*> Node;
	while (!q.empty()) {
		//if (q.front() == ' ') continue;//�˴������
		if (q.front() >= '0' && q.front() <= '9') {
			int count = 0;
			string full;
			full = q.front();
			q.pop();
			while (q.front() >= '0' && q.front() <= '9') {
				full.push_back(q.front());
				q.pop();
			}
			q.pop();//�˴�ɾ�������ַ�����Ŀո�
			TreeNode* temp = new TreeNode();
			if (temp == NULL) return NULL;
			temp->data = full;
			temp->leftChild = NULL;
			temp->rightChild = NULL;
			Node.push(temp);
		}
		else {
			//����һ���ڵ�A���������Ȼ���������ɾ������ɾ���������Ŀո�
			//��A���Һ���ΪNode.top(),��Node.pop(),��A������ΪNode.top(),��Node.pop(),�ڰ�A�ڵ�ѹ��ջ��
			TreeNode* temp = new TreeNode();
			temp->data = q.front();
			q.pop();//�˴�ɾ�����������
			q.pop();//�˴�ɾ�������������Ŀո�
			temp->rightChild = Node.top();
			Node.pop();
			temp->leftChild = Node.top();
			Node.pop();
			Node.push(temp);
		}
	}
	return Node.top();
}

//����������� ��֤���������Ƿ���ȷ
void InOrder(TreeNode* t) {
	if (t) {
		InOrder(t->leftChild);
		cout << t->data << " ";
		InOrder(t->rightChild);
	}
}

/*-----�����ʽ�ļ���------*/

//�ӷ�����ת������
int StringToInt(TreeNode* t) {
	string s = t->data;
	int size = s.length();
	int num = 0;
	for (int i = 0; i < size; i++) {
		num = num*10 + s[i] - '0';
	}
	return num;
}

//�ú��������˼����������ʽ
void PosOrderUseCal(TreeNode* t) {
	if (t) {
		PosOrderUseCal(t->leftChild);
		PosOrderUseCal(t->rightChild);
		//дһ���ַ�����ת�������ĺ���������ȥ���ǽڵ�
		if (t->leftChild != NULL && t->rightChild != NULL) {
			int num1 = StringToInt(t->leftChild);//��
			int num2 = StringToInt(t->rightChild);//��
			int num3 = 0;
			if (t->data == "+") num3 = num1 + num2;
			else if (t->data == "-") num3 = num1 - num2;
			else if (t->data == "*") num3 = num1 * num2;
			else if (t->data == "/") num3 = num1 / num2;
			//�ٰѵõ���������(num3)תΪstring����
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
			//�ڰ�t�ڵ�ָ�������еĲ�������Ϊs1;
			t->data = s1;
			//�ٰ�t�ڵ�������ӽڵ��Ϊ��
			t->leftChild = NULL;
			t->rightChild = NULL;
		}
	}
}

//�����ʽ�ļ���
int CalculateByTree(TreeNode* T) {
	TreeNode* temp = new TreeNode();
	temp = T;
	PosOrderUseCal(temp);
	//�ٰ�temp�е�string������ת��Ϊint����
	int answer=StringToInt(temp);
	delete temp;
	return answer;
}

/*-----������ʽ�Ƿ�ϸ�-----*/

//1.����ñ��ʽ�Ƿ�ϸ�(���ŵ�ƥ�估��������õ��Ƿ�ϸ�)
//2.���������������string s;
//3.����Բ���Ŷ�����ջ����Խ������������������飬���������������
//ֻ��Ҫ���ַ�����ԣ����Ǹ��ַ��������������������
//��Ҫ������ջ��һ���ǹ���Բ���ŵ�ջ��һ���ǹ����������ջ����÷ֿ���˼������һ��������

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
	bool judge = 1;//�ж������
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

	//�ȼ�����ʽ�Ƿ��׼
	if (!Judge(s)) {
		cout << "�Ƿ����ʽ���޷����㣡" << endl;
		return 0;
	}

	queue<char> posOrder;
	InToPos(s, posOrder);

	int answer = CalculateByPos(posOrder);
	cout << s <<" = " <<answer << endl;

	TreeNode* T = PosToTree(posOrder);//����������ӷ����Ĵ����ѽ��
	cout << "�������������:";
	InOrder(T);
	cout << endl;

	cout << "��" << s << "�������ʽ����Ľ��Ϊ��" << CalculateByTree(T) << endl;
	
	while (!posOrder.empty()) {
		cout << posOrder.front();
		posOrder.pop();
	}

	return 0;
}
