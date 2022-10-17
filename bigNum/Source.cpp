#include<iostream>
#include<string>
#include<stack>
#include<vector>
#include"BigNum.h"
using namespace std;
struct Variable {
	BigNum num;
	string name;
};
typedef struct Variable variable;
vector<variable> var;	//�sBigNum�ܼƸ��
int main()
{
	/*Integer x ="123 * 8 + 456";
	Decimal y = "-1.0 / 3 - 45 / 13.0";
	cout << x << endl << y << endl;*/

	bool isVar, isAsn, output, isInt, isFind;
	int index;
	string tmp;
	string input;
	string processed_input = "";
	vector<string> postfix;
	BigNum result;
	variable hold;
	while (getline(cin, input))
	{
		if (input == "") {
			continue;
		}
		//�M���ť�
		int countLeft = 0;
		int countRight = 0;

		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] != ' ') //�C�@���[�J
			{
				if (input[i] == '(')
				{
					countLeft++;
				}
				else if (input[i] == ')')
				{
					countRight++;
				}
				if (countRight > countLeft)
				{
					cout << "invalid input";
					exit(1);
				}
				processed_input += input[i];
			}
		}

		//���P�_�̫�@�ӬO���O�B��l
		if (processed_input[processed_input.length() - 1] == '+' || processed_input[processed_input.length() - 1] == '-' || processed_input[processed_input.length() - 1] == '*' || processed_input[processed_input.length() - 1] == '/' || processed_input[processed_input.length() - 1] == '^' )
		{
			cout << "invalid input";
			exit(1);
		}
		//�b�P�_�C�@�ӹB��l���ᦳ�S���X�{�s��B��l
		/*for (int i = 0; i < processed_input.length(); i++)
		{
			if (processed_input[i] == '+')
			{
				if (processed_input[i + 1] == '*' || processed_input[i + 1] == '/' || processed_input[i + 1] == '^' || processed_input[i + 1] == '!')
				{
					cout << "invalid input";
					exit(1);
				}
			}
			else if (processed_input[i] == '-')
			{
				if (processed_input[i + 1] == '*' || processed_input[i + 1] == '/' || processed_input[i + 1] == '^' || processed_input[i + 1] == '!')
				{
					cout << "invalid input";
					exit(1);
				}
			}
			else if (processed_input[i] == '*')
			{
				if (processed_input[i + 1] == '*' || processed_input[i + 1] == '/' || processed_input[i + 1] == '^' || processed_input[i + 1] == '!')
				{
					cout << "invalid input";
					exit(1);
				}
			}
			else if (processed_input[i] == '/')
			{
				if (processed_input[i + 1] == '*' || processed_input[i + 1] == '/' || processed_input[i + 1] == '^' || processed_input[i + 1] == '!')
				{
					cout << "invalid input";
					exit(1);
				}
			}
			else if (processed_input[i] == '!')
			{
				if (processed_input[i + 1] == '*' || processed_input[i + 1] == '/' || processed_input[i + 1] == '^' || processed_input[i + 1] == '!')
				{
					cout << "invalid input";
					exit(1);
				}
			}
		}*/

		//�P�_�}�Y���Ʀr
		if (processed_input[0] == '*' || processed_input[0] == '/' || processed_input[0] == '^' || processed_input[0] == '!')
		{
			cout << "invalid input";
			exit(1);
		}


		if (countLeft != countRight) //�P�_�̫ᥪ�k�A���|���|�@��
		{
			cout << "invalid input";
			exit(1);
		}
		//cout << processed_input << endl;

		//�B��set�������
		output = true;
		isInt = false;
		isVar = false;
		isAsn = false;
		if (processed_input[0] == 'S' && processed_input[1] == 'e' && processed_input[2] == 't') {
			output = false;
			isVar = true;
			if (processed_input[3] == 'I') {
				isInt = true;
			}
			processed_input.erase(processed_input.begin(), processed_input.begin() + 10);	//�Nset int dec ����
			for (int i = 0; i < processed_input.size(); i++) {
				if (processed_input[i] == '=') {
					processed_input.erase(processed_input.begin(), processed_input.begin() + i + 1);
					break;
				}
				else {
					tmp = tmp + processed_input[i];
				}
			}
			hold.name = tmp;	//�s�ܼƦW��
			for (int i = 0; i < var.size(); i++) {
				if (tmp == var[i].name) {
					cout << "invalid input";
					exit(1);
				}
			}
			//cout << processed_input << endl;

		}
		else if ((index = assign(processed_input)) != -1) {
			output = false;
			isAsn = true;
			for (int i = 0; i < index; i++) {
				tmp = tmp + processed_input[i];
			}
			processed_input.erase(processed_input.begin(), processed_input.begin() + index + 1);
		}

		postfix = infix_to_postfix(processed_input);
		/*for (int i = 0; i < postfix.size(); i++) {
			cout << postfix[i] << " ";
		}
		cout << endl << "eliminate\n";*/


		result = evaluate(postfix);
		if (isVar) {	//�N�ܼƸ�Ʀs�_��
			if (isInt) {
				result.set(result.getIntPart());
			}
			result.setDecimal(!isInt);
			hold.num = result;
			var.push_back(hold);
		}
		else if (isAsn) {
			if (isInt) {
				result.set(result.getIntPart());
			}
			result.setDecimal(!isInt);
			isFind = false;
			for (int i = 0; i < var.size(); i++) {
				if (tmp == var[i].name) {
					if (!var[i].num.isItDecimal()) {
						result.set(result.getIntPart());
					}
					isFind = true;
					var[i].num = result;
					break;
				}

			}
			if (!isFind) {
				cout << "invalid input";
				exit(1);
			}
		}
		if (output) {
			cout << result << endl;
		}
		postfix.clear();
		processed_input.clear();
		input.clear();
		tmp.clear();
	}

}