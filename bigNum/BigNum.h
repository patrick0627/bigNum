#pragma once
#include<string>
#include<vector>
#include<algorithm>

using namespace std;
class BigNum;
bool preGEpost(const string, const string);
string bigAdd(const string, const  string);
string bigSub(const string, const string, bool&);
string bigMul(const string, const string);
string bigDiv(const string, const string);
int priority(char op);

bool isSign(string front, char now);

bool isNum(char x);

void PushNumber(vector<string>& number, string& input);

void special_poppush(vector<string>& number, stack<char>& op, bool LeftToRight);

void PopPushop(vector<string>& number, stack<char>& op, char input, bool LeftToRight);

bool isChar(char x);

vector<string> infix_to_postfix(string a);

vector<string> eliminate_sign(vector<string> input);

bool isNumber(string input);

BigNum getSquareRoot(BigNum num);

BigNum evaluate(vector<string> postfix);

int assign(string input);

class BigNum
{
public:
	BigNum();
	~BigNum();
	BigNum(string);
	BigNum(const BigNum*);
	BigNum(const char* input) {
		string tmp = input;
		string infix;
		vector<string> postfix;
		BigNum ans;
		infix.clear();
		for (int i = 0; i < tmp.size(); i++) {
			if (tmp[i] != ' ') {
				infix = infix + tmp[i];
			}
		}
		postfix = infix_to_postfix(infix);
		ans = evaluate(postfix);
		*this = ans;
	}

	BigNum operator=(const BigNum&);
	BigNum operator+(const BigNum&);
	BigNum operator-(const BigNum&);
	BigNum operator*(const BigNum&);
	BigNum operator/(const BigNum&);
	friend ostream& operator<<(ostream&, BigNum);
	friend istream& operator>>(istream&, BigNum&);


	string getDenominator() { return denominator; }
	string getNumerator() { return num; }
	string getDecimal();
	string getIntPart();
	bool validForPower(string& powerPart, bool& needRoot);

	void initial(BigNum b) {
		this->num = b.num;
		this->denominator = b.denominator;
		this->isDecimal = b.isDecimal;
		this->isPositive = b.isPositive;
	}
	void set(string input);
	void setDecimal(bool b) { isDecimal = b; }
	void transposeSign() { isPositive = !isPositive; }
	bool isItPositive() { return isPositive; }
	bool isItDecimal() { return isDecimal; }
private:
	string num;
	string denominator;
	bool isDecimal;
	bool isPositive;
};

//intent: default constructor
//pre;
//post:
BigNum::BigNum()
{
	isPositive = true;
	num = "0";
	denominator = "1";
	isDecimal = false;
}

//intent: destructor
//pre;
//post:
BigNum::~BigNum()
{

}

//intent: copy constructor
//pre; copied point
//post: none
BigNum::BigNum(const BigNum* b)
{
	this->num = b->num;
	this->denominator = b->denominator;
	this->isDecimal = b->isDecimal;
	this->isPositive = b->isPositive;
}

//intent: setting constructor
//pre; input num
//post: none
//請infix 轉 postfix 就做好防呆 
BigNum::BigNum(string input)
{
	int pointIndex = input.size();
	bool hasPoint = false;
	isPositive = true;
	//可過濾 前方冗長的正負
	while (input[0] == 'p' || input[0] == 'n')
	{
		if (input[0] == 'n')
		{
			isPositive = !isPositive;
		}
		input.erase(input.begin());
		if (input.size() < 2)
		{
			break;
		}
	}
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == '.')
		{
			pointIndex = i;
			hasPoint = true;
		}
		//防 非數字
		else if (input[i] > '9' || input[i] < '0')
		{
			cout << input << "=> Invalid Input for BigNum" << endl;
			return;
		}
	}

	//將小數 轉換為分數 分母暫且為10的次方數
	if (hasPoint)
	{
		string patchZero(input.size() - pointIndex - 1, '0');
		denominator = "1" + patchZero;
		input.erase(input.begin() + pointIndex);
		num = input;
		isDecimal = true;
	}
	else
	{
		denominator = "1";
		num = input;
		isDecimal = false;
	}

	//過濾numerator 前面為0的數
	for (int i = 0; i < num.size(); i++)
	{
		if (num[i] != '0')
		{
			num.assign(num, i);
			break;
		}
	}
}

//同setting constructor
//intent: setting input, transpose decimal to fraction
//pre; input num
//post: none
void BigNum::set(string input) {
	int pointIndex = input.size();
	bool hasPoint = false;
	isPositive = true;
	//可過濾 前方冗長的正負
	while (input[0] == 'p' || input[0] == 'n')
	{
		if (input[0] == 'n')
		{
			isPositive = !isPositive;
		}
		input.erase(input.begin());
		if (input.size() < 2)
		{
			break;
		}
	}
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == '.')
		{
			pointIndex = i;
			hasPoint = true;
		}
		//防 非數字
		else if (input[i] > '9' || input[i] < '0')
		{
			cout << input << "=> Invalid Input for BigNum" << endl;
			return;
		}
	}

	//將小數 轉換為分數 分母暫且為10的次方數
	if (hasPoint)
	{
		string patchZero(input.size() - pointIndex - 1, '0');
		denominator = "1" + patchZero;
		input.erase(input.begin() + pointIndex);
		num = input;
		isDecimal = true;
	}
	else
	{
		denominator = "1";
		num = input;
		isDecimal = false;
	}

	//過濾 numerator 前面為0的數
	for (int i = 0; i < num.size(); i++)
	{
		if (num[i] != '0')
		{
			num.assign(num, i);
			break;
		}
	}
}

string BigNum::getIntPart()
{
	string intPart;
	if (!this->isPositive)
	{
		intPart += '-';
	}
	string result = bigDiv(this->num, this->denominator);
	for (string::iterator it = result.begin(); it != result.end(); it++)
	{
		if (*it == '.')
		{
			intPart.assign(result, 0, distance(result.begin(), it));
			return intPart;
		}
	}
}

string BigNum::getDecimal()
{
	string numerator(this->num), denom(this->denominator), out;
	//去零頭
	while (numerator[0] == '0' && numerator.size() > 1)
	{
		numerator.erase(numerator.begin());
	}
	while (denom[0] == '0' && denom.size() > 1)
	{
		denom.erase(denom.begin());
	}

	string decimal = bigDiv(numerator, denom);
	char sign;
	(this->isPositive) ? sign = '+' : sign = '-';
	if (this->isDecimal)
	{
		out = sign + decimal;
	}
	else
	{
		out += sign;
		for (int i = 0; decimal[i] != '.'; i++)
		{
			out += decimal[i];
		}
	}

	return out;
}

bool BigNum::validForPower(string& powerPart, bool& needRoot)
{
	string floatPart, sign;
	(this->isPositive) ? sign = "" : sign = "-";

	//整數 直接回傳
	if (!this->isDecimal)
	{
		powerPart = sign + num;
		needRoot = false;
		return true;
	}
	//小數
	else
	{
		string result = bigDiv(this->num, this->denominator);
		for (string::iterator it = result.begin(); it != result.end(); it++)
		{
			if (*it == '.')
			{
				powerPart.assign(result, 0, distance(result.begin(), it));
				floatPart.assign(result, distance(result.begin(), it) + 1);
				string allZero(99, '0');
				if (floatPart == "5" + allZero)		// number = ...xxxxx.500000....
				{
					powerPart = sign + bigAdd(bigMul(powerPart, "2"), "1");
					needRoot = true;
					return true;
				}
				else if (floatPart == "0" + allZero)							// number = ...xxxxx.00000.....
				{
					needRoot = false;
					return true;
				}
				else								// number = .....xxxxx.xxxxx.....
				{
					needRoot = false;
					return false;
				}
			}
		}
	}

}

//BigNum operator overloading 
//===============================================
BigNum BigNum::operator=(const BigNum& b)
{
	this->num = b.num;
	this->denominator = b.denominator;
	this->isDecimal = b.isDecimal;
	this->isPositive = b.isPositive;
	return *this;
}
BigNum BigNum::operator+(const BigNum& addend)
{
	// 1.通分
	BigNum sum, a(*this), b(addend);
	sum.isDecimal = this->isDecimal + addend.isDecimal;
	if (sum.isDecimal)
	{
		sum.denominator = bigMul(this->denominator, addend.denominator);
	}


	// 2.分子相加
	// a+b, -a+(-b)
	if (!(a.isPositive ^ b.isPositive))
	{
		if (sum.isDecimal)
		{
			sum.num = bigAdd(bigMul(a.num, b.denominator), bigMul(b.num, a.denominator));
		}
		else
		{
			sum.num = bigAdd(a.num, b.num);
		}
		if (!a.isPositive)
		{
			sum.isPositive = false;
		}
	}
	else
	{
		string aNum, bNum;
		if (sum.isDecimal)
		{
			aNum = bigMul(a.num, b.denominator);
			bNum = bigMul(b.num, a.denominator);
		}
		else
		{
			aNum = a.num;
			bNum = b.num;
		}
		// a+(-b)
		if (a.isItPositive())
		{
			sum.num = bigSub(aNum, bNum, sum.isPositive);
		}
		// -a+b
		else
		{
			sum.num = bigSub(bNum, aNum, sum.isPositive);
		}
	}

	// 3.simplify 除10^i 去零尾 可優化************
	while (sum.num.back() == '0' && sum.denominator.back() == '0'
		&& sum.num.size() > 1 && sum.denominator.size() > 1)
	{
		sum.num.pop_back();
		sum.denominator.pop_back();
	}

	return sum;
}
BigNum BigNum::operator-(const BigNum& subend)
{
	// 1.通分
	BigNum difference, a(*this), b(subend);
	difference.isDecimal = this->isDecimal + subend.isDecimal;
	if (difference.isDecimal)
	{
		difference.denominator = bigMul(this->denominator, subend.denominator);
	}


	// 2.分子相減
	// (-a)-(+b) , (+a)-(-b)  , xor
	if (a.isPositive ^ b.isPositive)
	{
		if (difference.isDecimal)
		{
			difference.num = bigAdd(bigMul(a.num, b.denominator), bigMul(b.num, a.denominator));
		}
		else
		{
			difference.num = bigAdd(a.num, b.num);
		}
		if (!a.isPositive)
		{
			difference.isPositive = false;
		}
	}
	// a-(+b)  (-a)-(-b)
	else
	{
		string aNum, bNum;
		if (difference.isDecimal)
		{
			aNum = bigMul(a.num, b.denominator);
			bNum = bigMul(b.num, a.denominator);
		}
		else
		{
			aNum = a.num;
			bNum = b.num;
		}
		// a-(+b) = a-b
		if (a.isItPositive())
		{
			difference.num = bigSub(aNum, bNum, difference.isPositive);
		}
		// (-a)-(-b) = b-a
		else
		{
			difference.num = bigSub(bNum, aNum, difference.isPositive);
		}
	}

	// 3.simplify 除10^i 去零尾 可優化************
	while (difference.num.back() == '0' && difference.denominator.back() == '0'
		&& difference.num.size() > 1 && difference.denominator.size() > 1)
	{
		difference.num.pop_back();
		difference.denominator.pop_back();
	}

	return difference;
}
BigNum BigNum::operator*(const BigNum& b)
{
	BigNum product;
	// nor 
	product.isPositive = !(this->isPositive ^ b.isPositive);
	product.isDecimal = this->isDecimal + b.isDecimal;

	if (product.isDecimal)
	{
		//分母 分子 互乘
		product.denominator = bigMul(this->denominator, b.denominator);
		product.num = bigMul(this->num, b.num);

		//分母分子 去0尾
		while (product.num.back() == '0' && product.denominator.back() == '0'
			&& product.num.size() > 1 && product.denominator.size() > 1)
		{
			product.num.pop_back();
			product.denominator.pop_back();
		}
	}
	else
	{
		product.num = bigMul(this->num, b.num);
	}
	return product;
}
BigNum BigNum::operator/(const BigNum& b)
{
	BigNum quotient;
	// nor 
	quotient.isPositive = !(this->isPositive ^ b.isPositive);
	quotient.isDecimal = this->isDecimal + b.isDecimal;

	if (quotient.isDecimal)
	{
		//分母 分子 交叉互乘乘
		quotient.denominator = bigMul(this->denominator, b.num);
		quotient.num = bigMul(this->num, b.denominator);

		//分母分子 去0尾 除10
		//分母分子 去0尾
		while (quotient.num.back() == '0' && quotient.denominator.back() == '0'
			&& quotient.num.size() > 1 && quotient.denominator.size() > 1)
		{
			quotient.num.pop_back();
			quotient.denominator.pop_back();
		}
	}
	else
	{
		string result = bigDiv(this->num, b.num);
		for (string::iterator it = result.begin(); it != result.end(); it++)
		{
			if (*it == '.')
			{
				quotient.num.assign(result, 0, distance(result.begin(), it));
				break;
			}
		}
	}
	return quotient;
}
ostream& operator<<(ostream& out, BigNum n)
{
	if (n.denominator == "0")
	{
		out << "invalid";
		return out;
	}
	string numerator(n.num), denom(n.denominator);
	//去零頭
	while (numerator[0] == '0' && numerator.size() > 1)
	{
		numerator.erase(numerator.begin());
	}
	while (denom[0] == '0' && denom.size() > 1)
	{
		denom.erase(denom.begin());
	}

	string decimal = bigDiv(numerator, denom);
	char sign;
	if (!n.isPositive) {
		sign = '-';
	}
	else {
		sign = NULL;
	}
	out << sign;
	int indexForPoint = 0;
	for (indexForPoint; decimal[indexForPoint] != '.'; indexForPoint++)
	{
		out << decimal[indexForPoint];
	}
	if (n.isDecimal)
	{
		string tmp;
		tmp.assign(decimal, indexForPoint, 101);
		out << tmp;
	}

	return out;
}
istream& operator>>(istream& in, BigNum& n)
{
	string input;
	in >> input;
	n.set(input);
	return in;
}
//*******************************************************


//unsigned大數 加減乘除
//==========================================================
bool preGEpost(const string pre, const string post)
{
	////避免 前面為0 a= 0002562 
	//int preNonZeroIndex = pre.size();
	//int postNonZeroIndex = post.size();
	//for (int i = 0; i < pre.size(); i++)
	//{
	//	if (pre[i] > '0' && pre[i]-'0'<10)
	//	{
	//		preNonZeroIndex = i;
	//		break;
	//	}
	//}
	//for (int i = 0; i < post.size(); i++)
	//{
	//	if (post[i] > '0' && post[i] - '0' < 10)
	//	{
	//		postNonZeroIndex = i;
	//		break;
	//	}
	//}
	int preNonZeroIndex = 0;
	int postNonZeroIndex = 0;

	//unsigned且去零頭 length長者較大 , 依樣長則比大小
	int preLen = pre.size() - preNonZeroIndex;
	int postLen = post.size() - postNonZeroIndex;
	if (preLen > postLen)
	{
		return true;
	}
	else if (preLen < postLen)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < preLen; i++)
		{
			int difference = pre[preNonZeroIndex + i] - post[postNonZeroIndex + i];
			if (difference > 0)
			{
				return true;
			}
			else if (difference == 0)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
	}
}
string bigAdd(const string adder, const  string addend)
{
	string sum, big, small;
	if (adder.size() > addend.size())
	{
		big = adder;
		small = addend;
	}
	else
	{
		big = addend;
		small = adder;
	}
	reverse(big.begin(), big.end());
	reverse(small.begin(), small.end());
	sum = big;
	bool carry = false;

	for (int i = 0; i < small.size(); i++)
	{
		sum[i] += small[i] - '0' + carry;
		carry = false;
		if (sum[i] > '9')
		{
			carry = true;
			sum[i] -= 10;
		}
		if (i == small.size() - 1 && carry)
		{
			if (sum.size() > small.size())
			{
				small += '0';
			}
			else
			{
				sum += '1';
			}
		}
	}
	reverse(sum.begin(), sum.end());
	return sum;
}
string bigSub(const string subtractor, const  string subend, bool& signPos)
{
	string a(subtractor), b(subend);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	//去0尾  51582600 => 515826
	while (a.back() == '0' && a.size() > 2)
	{
		a.pop_back();
	}
	while (b.back() == '0' && b.size() > 2)
	{
		b.pop_back();
	}

	//判斷誰大 設a為大方 b為小方 及正負號
	if (a.size() < b.size())
	{
		signPos = false;
		swap(a, b);
	}
	else if (a.size() == b.size() && !preGEpost(subtractor, subend))
	{
		signPos = false;
		swap(a, b);
	}
	else {
		signPos = true;
	}

	//a-b
	//943001
	//149
	//804990
	string difference(a);
	bool borrow = false;
	for (int i = 0; i < b.size(); i++)
	{
		difference[i] = '0' + a[i] - b[i] - borrow;
		borrow = false;
		if (difference[i] < '0')
		{
			borrow = true;
			difference[i] += 10;
		}
		if (i == b.size() - 1 && borrow)
		{
			if (difference.size() > b.size())
			{
				b += '0';
			}
			else
			{
				cout << "warnnnnnning 大數剪法 出錯";
			}
		}
	}

	//去0尾 再反轉
	while (difference.back() == '0' && difference.size() > 1)
	{
		difference.pop_back();
	}
	reverse(difference.begin(), difference.end());

	return difference;
}
string bigMul(const string multiplier, const string multiplcand)
{
	string product(multiplcand.size() + multiplier.size() + 1, '0');
	string a(multiplier), b(multiplcand);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	for (int i = 0; i < b.size(); i++)
	{
		//乘法
		int mCarry = 0;
		string buffer(i, '0');	//右位移
		int mEnd = b[i] - '0';
		for (int j = 0; j < a.size(); j++)
		{
			int result = mCarry + (a[j] - '0') * mEnd;
			buffer += '0' + (result % 10);
			mCarry = result / 10;
		}
		if (mCarry)
		{
			buffer += '0' + mCarry;
		}

		//將乘完結果加入prduct
		int aCarry = false;
		for (int i = 0; i < buffer.size(); i++)
		{
			product[i] += buffer[i] - '0' + aCarry;
			aCarry = false;
			if (product[i] > '9')
			{
				aCarry = true;
				product[i] -= 10;
			}
			if (i == buffer.size() - 1 && aCarry)
			{
				if (product.size() > buffer.size())
				{
					buffer += '0';
				}
				else
				{
					product += '1';
				}
			}
		}
	}
	while (product.back() == '0' && product.size() > 1)
	{
		product.pop_back();
	}
	reverse(product.begin(), product.end());
	return product;
}
string bigDiv(const string  divisor, const string dividend)				// 除至小數後100位, divisor & divend 絕對不能為 000xxxx 有零頭的形式
{
	string quotient(""), dEnd, dSor(divisor);
	int powerTenOfDividend = 0;

	//將 dividend 前面的非零項取出 作為 dEnd,  50000=> 5
	for (int i = 0; i < dividend.size(); i++)
	{
		if (dividend[i] != '0')
		{
			dEnd += dividend[i];
		}
		else {
			powerTenOfDividend = dividend.size() - i;
			break;
		}
	}

	if (dEnd == "")
	{
		cout << "dividend is zero, couldn't divide";
	}
	else
	{
		//處理 小數前的除法
		//===========================================
		int DifferenceOfdigits = dSor.size() - dEnd.size();
		if (DifferenceOfdigits > -1)
		{
			//處理 dividend 進位
			string patchZero(DifferenceOfdigits, '0');
			dEnd += patchZero;
			//小數前
			for (int i = 0; i <= DifferenceOfdigits; i++)
			{
				char times = '0';
				while (1)
				{
					bool couldSub = true;
					string result = bigSub(dSor, dEnd, couldSub);
					if (couldSub)
					{
						dSor = result;
						times += 1;
					}
					else
					{
						break;
					}
				}
				//dEnd shift right
				if (i != DifferenceOfdigits)
				{
					dEnd.pop_back();
				}
				//避免商數 前項加到0 
				if (quotient == "" && times == '0')
				{
					continue;
				}
				quotient += times;
			}
		}
		//如果divisor 比dividend小 則小數點前為0
		if (quotient == "")
		{
			quotient = "0";
		}
		//*******************************************

		//處理 小數後100位
		//===========================================
		////加小數點 若不夠位 要補0
		if (quotient.size() <= powerTenOfDividend)
		{
			string patchZero(powerTenOfDividend - quotient.size(), '0');
			quotient = "0." + patchZero + quotient;
		}
		else {
			quotient.insert(quotient.end() - powerTenOfDividend, '.');
		}
		////繼續除法 將小數點後100位加上去
		for (int i = powerTenOfDividend; i < 100; i++)
		{
			//已整除 補完剩餘0的項跳出
			if (dSor == "0")
			{
				string patchZero(100 - i, '0');
				quotient += patchZero;
				break;
			}
			char times = '0';
			//divisor shift left
			dSor += '0';
			while (1)
			{
				bool couldSub = true;
				string result = bigSub(dSor, dEnd, couldSub);
				if (couldSub)
				{
					dSor = result;
					times += 1;
				}
				else
				{
					break;
				}
			}
			quotient += times;
		}//*******************************************
	}
	return quotient;
}
//**************************************************************

int priority(char op)
{
	switch (op)
	{
	case'!':
		return 2;
	case'^':
		return 3;
	case'p':case'n':
		return 4;
	case'*':case'/':
		return 5;
	case'+':case'-':
		return 6;
	}
}

bool isSign(string front, char now)
{
	if (front == "(" || front == "+" || front == "-" || front == "*" || front == "/" || front == "\0" || front == "^")
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isNum(char x)
{
	if (x - '0' >= 0 && x - '0' <= 9)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PushNumber(vector<string>& number, string& input)
{
	if (input != "(" && input != "")
	{
		number.push_back(input);
		input = "";
	}
}

void special_poppush(vector<string>& number, stack<char>& op, bool LeftToRight)
{
	if (LeftToRight == true)
	{
		while (op.size() != 0 && op.top() != '(')
		{
			number.push_back(string(1, op.top()));
			op.pop();
		}
	}
	else
	{
		while (op.size() != 0 && op.top() != '(')
		{
			number.push_back(string(1, op.top()));
			op.pop();
		}
	}
}

void PopPushop(vector<string>& number, stack<char>& op, char input, bool LeftToRight)
{
	if (LeftToRight == true)
	{
		while (op.size() != 0 && op.top() != '(' && priority(op.top()) <= priority(input))
		{
			number.push_back(string(1, op.top()));
			op.pop();
		}
	}
	else
	{
		while (op.size() != 0 && op.top() != '(' && priority(op.top()) < priority(input))
		{
			number.push_back(string(1, op.top()));
			op.pop();
		}
	}
}

bool isChar(char x)
{
	if ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<string> infix_to_postfix(string a)
{
	//string處理完要清掉 放進stack後清掉

	stack<char> op; //放運算元
	vector<string> number; //放數字
	string unprocessed_number = "";
	string prechar = "";
	string infix = a;
	//infix += ")";
	//op.push('(');

	for (int i = 0; i < infix.length(); i++)
	{
		if (infix[i] == '+' || infix[i] == '-')
		{
			if (isSign(prechar, infix[i]) == true) //正負號
			{
				if (infix[i] == '+')
				{
					op.push('p');
				}
				else
				{
					op.push('n');
				}
			}
			else //加減符號
			{
				PushNumber(number, unprocessed_number);
				PopPushop(number, op, infix[i], true); //加減的運算規則是從左邊到右邊
				op.push(infix[i]);
			}
		}
		else if (infix[i] == '*' || infix[i] == '/')
		{
			PushNumber(number, unprocessed_number);
			PopPushop(number, op, infix[i], true); //加減的運算規則是從左邊到右邊
			op.push(infix[i]);
		}
		else if (infix[i] == '!')
		{
			PushNumber(number, unprocessed_number);
			PopPushop(number, op, infix[i], true); //加減的運算規則是從左邊到右邊
			op.push(infix[i]);
		}
		else if (infix[i] == '^')
		{
			PushNumber(number, unprocessed_number);
			PopPushop(number, op, infix[i], false); //加減的運算規則是從右邊到左邊
			op.push(infix[i]);
		}
		else if (infix[i] == '(')
		{
			op.push(infix[i]);
		}
		else if (infix[i] == ')')
		{
			PushNumber(number, unprocessed_number);
			//PopPushop(number, op, infix[i], true); //加減的運算規則是從左邊到右邊
			special_poppush(number, op, true);
			if (op.top() == '(')
			{
				op.pop();
			}
		}
		else if (isNum(infix[i]) || infix[i] == '.' || isChar(infix[i]))
		{
			unprocessed_number += infix[i];
		}
		prechar = infix[i];
	}
	if (unprocessed_number.length() != 0)
	{
		number.push_back(unprocessed_number);
		unprocessed_number = "";
	}
	while (op.size() != 0)
	{
		number.push_back(string(1, op.top()));
		op.pop();
	}
	/*for (int i = 0; i < number.size(); i++)
	{
		cout << number[i];
	}*/
	//cout << number.size() << endl;
	/*cout << endl;*/

	return number;
}

vector<string> eliminate_sign(vector<string> input) {
	int count = 0, offset;
	for (int i = 0; i < input.size(); i++) {
		if (i != input.size() - 1 && (input[i + 1] == "n" || input[i + 1] == "p")) {
			offset = 1;
			count = 0;
			while (i + offset < input.size() && (input[i + offset] == "n" || input[i + offset] == "p")) {
				if (input[i + offset] == "n") {
					count++;
				}
				offset++;
			}
			if (count % 2 == 1) {
				input[i] = "n" + input[i];
			}
			input.erase(input.begin() + i + 1, input.begin() + i + offset);
			/*for (int i = 0; i < input.size(); i++) {
				cout << input[i] << " ";
			}
			cout << endl;*/
		}
	}
	return input;
}

bool isNumber(string input) {
	for (int i = 0; i < input.size(); i++) {
		if (input[i] < '0' || input[i] > '9') {
			if (input[i] != 'n' && input[i] != '.') {
				return false;
			}
		}
	}
	return true;
}

BigNum getSquareRoot(BigNum num) {
	BigNum answer;
	string ans, s;
	s = num.getDecimal();
	if (s[0] == '+') {
		s.erase(s.begin(), s.begin() + 1);
	}
	int len1;
	s.length() % 2 == 0 ? len1 = s.length() / 2 : len1 = s.length() / 2 + 1;
	s.insert(s.end(), 300, '0');
	int len = s.length();
	int i, j;
	int x[300] = {}, y[600] = {};
	for (i = 0, j = len - 1; j >= 0; i++)
	{
		x[i] = s[j] - '0';
		if (j - 1 >= 0)    x[i] = x[i] + (s[j - 1] - '0') * 10;
		if (j - 2 >= 0)    x[i] = x[i] + (s[j - 2] - '0') * 100;
		if (j - 3 >= 0)    x[i] = x[i] + (s[j - 3] - '0') * 1000;
		j -= 4;
	}
	int xlen = len, ylen = 0, head = 0;
	while (xlen >= 0 && x[xlen] == 0) xlen--;
	for (j = (len - 1) / 8, i = j * 2; j >= 0; j--, i -= 2)
	{
		ylen++;
		for (int p = ylen; p >= 1; p--)
			y[p] = y[p - 1];
		y[0] = 0;
		if (xlen < j)
		{
			if (!head)
				ans.push_back('0'), head = 1;
			else
				ans = ans + "0000";
			continue;
		}
		int l = 0, r = 9999, p;
		int z[300]; // z = (y*10 + p)*p;
		while (l <= r)
		{
			p = (l + r) / 2;
			y[0] += p;
			z[0] = 0;
			for (int q = 0; q <= ylen + 5; q++)
			{
				z[q] += p * y[q];
				z[q + 1] = z[q] / 10000;
				z[q] %= 10000;
			}
			int chflag = 0;
			for (int q = ylen + 5; q >= 0; q--)
			{
				if (z[q] > x[i + q])
				{
					chflag = 1;
					break;
				}
				else if (z[q] < x[i + q])
				{
					chflag = 0;
					break;
				}
			}
			y[0] -= p;
			if (chflag)
				r = p - 1;
			else
				l = p + 1;
		}
		p = l - 1;
		y[0] = p;
		z[0] = 0;
		for (int q = 0; q <= ylen + 5; q++)
		{
			z[q] += p * y[q];
			z[q + 1] = z[q] / 10000;
			z[q] %= 10000;
		}
		for (int q = ylen + 5; q >= 0; q--)
			x[i + q] -= z[q];
		for (int q = 0; q <= ylen + 5; q++)
		{
			while (x[i + q] < 0)
				x[i + q] += 10000, x[i + q + 1]--;
		}
		y[0] += p;
		for (int q = 0; q <= ylen + 5; q++)
		{
			if (y[q] >= 10000)
			{
				y[q + 1] += y[q] / 10000;
				y[q] %= 10000;
			}
		}
		ylen += 5;
		while (ylen >= 0 && y[ylen] == 0)    ylen--;
		while (xlen >= 0 && x[xlen] == 0)    xlen--;
		if (!head)
		{
			ans = ans + to_string(p);
			head = 1;
		}
		else
		{
			string str = to_string(p);
			for (int w = 0; w < 4 - str.length(); w++)
				ans.push_back('0');
			ans = ans + str;
		}
	}
	ans.insert(len1, ".");
	answer.set(ans);
	return ans;

	/*BigNum guess_1, guess_2, r, two, diff, result, zero;
	zero.set("0");
	diff.set("0.01");
	two.set("2.0");
	guess_1 = num / two;
	guess_2 = guess_1;
	r = num / guess_2;
	guess_2 = (guess_2 + r) / two;
	result= guess_1 - guess_2;
	result.abs();
	while (!result.isZero() && (result-diff).isItPositive()) {
		guess_1 = guess_2;
		r = num / guess_2;
		guess_2 = (guess_2 + r) / two;
		result = guess_1 - guess_2;
		result.abs();
		if (result.isZero()) {
			cout << "幹林剌\n";
		}
	}

	return guess_2;*/

}

BigNum evaluate(vector<string> postfix) {
	BigNum result, tmp;
	vector <BigNum> stack;
	bool isVar, isFind;
	stack.clear();
	for (int i = 0; i < postfix.size(); i++) {
		if (postfix[i] == "n") {
			if (stack.size() < 1) {
				cout << "invalid input";
				exit(1);
			}
			stack[stack.size() - 1].transposeSign();
		}
		else if (postfix[i] == "+") {
			if (stack.size() < 2) {
				cout << "invalid input";
				exit(1);
			}
			result = stack[stack.size() - 2] + stack[stack.size() - 1];
			stack.pop_back();
			stack.pop_back();
			stack.push_back(result);
		}
		else if (postfix[i] == "-") {
			if (stack.size() < 2) {
				cout << "invalid input";
				exit(1);
			}
			result = stack[stack.size() - 2] - stack[stack.size() - 1];
			stack.pop_back();
			stack.pop_back();
			stack.push_back(result);
		}
		else if (postfix[i] == "*") {
			if (stack.size() < 2) {
				cout << "invalid input";
				exit(1);
			}
			result = stack[stack.size() - 2] * stack[stack.size() - 1];
			stack.pop_back();
			stack.pop_back();
			stack.push_back(result);
		}
		else if (postfix[i] == "/") {
			if (stack.size() < 2) {
				cout << "invalid input";
				exit(1);
			}
			double value = stod(stack[stack.size() - 1].getDecimal());
			if (value == 0.0) {
				cout << "invalid input";
				exit(1);
			}
			result = stack[stack.size() - 2] / stack[stack.size() - 1];
			stack.pop_back();
			stack.pop_back();
			stack.push_back(result);
		}
		else if (postfix[i] == "^") {
			if (stack.size() < 2) {
				cout << "invalid input";
				exit(1);
			}
			bool isPositive = true;
			double power = stod(stack[stack.size() - 1].getDecimal());
			if (power < 0) {
				isPositive = false;
			}
			power = abs(power);
			result.set("1");
			if (power - (int)power == 0) {
				for (int i = 0; i < power; i++) {
					result = result * stack[stack.size() - 2];
				}
			}
			else {
				power *= 2;
				for (int i = 0; i < power; i++) {
					result = result * stack[stack.size() - 2];
				}
				result = getSquareRoot(result);
			}
			if (!isPositive) {
				tmp.set("1.0");
				result = tmp / result;
			}
			stack.pop_back();
			stack.pop_back();
			stack.push_back(result);
		}
		else if (postfix[i] == "!") {
			if (stack.size() < 1) {
				cout << "invalid input";
				exit(1);
			}
			double power = stod(stack[stack.size() - 1].getDecimal());
			string hold;
			if (power - (int)power != 0 || power < 0) {	//小數階層
				cout << "invalid input";
				exit(1);
			}
			result.set("1");
			for (int i = 1; i <= power; i++) {
				hold = to_string(i);
				tmp.set(hold);
				result = result * tmp;
			}
			stack.pop_back();
			stack.push_back(result);
		}
		else if (isNumber(postfix[i])) {
			tmp.set(postfix[i]);
			stack.push_back(tmp);
		}
		else if (postfix[i] == "p") {

		}
		else {	//為變數
		}
	}

	return stack[0];
}

int assign(string input) {
	for (int i = 0; i < input.size(); i++) {
		if (input[i] == '=') {
			return i;
		}
	}
	return -1;
}

class Integer :public BigNum
{
public:
	Integer(const char* input) {
		string tmp = input;
		string infix;
		vector<string> postfix;
		BigNum ans;
		infix.clear();
		for (int i = 0; i < tmp.size(); i++) {
			if (tmp[i] != ' ') {
				infix = infix + tmp[i];
			}
		}
		postfix = infix_to_postfix(infix);
		ans = evaluate(postfix);
		this->initial(ans);
	}

};

class Decimal:public BigNum
{
public:
	Decimal(const char* input) {
		string tmp = input;
		string infix;
		vector<string> postfix;
		BigNum ans;
		infix.clear();
		for (int i = 0; i < tmp.size(); i++) {
			if (tmp[i] != ' ') {
				infix = infix + tmp[i];
			}
		}
		postfix = infix_to_postfix(infix);
		ans = evaluate(postfix);
		this->initial(ans);
	}
};

