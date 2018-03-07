#include<iostream>
#include<vector>
#include<string>
#include<strstream>
using namespace std;

char str[30];
int len;
vector<string> vstr;
vector<string> stack;
string priTable[8][8] = 
	{
		">",">","<","<","<",">","<",">",
		">",">","<","<","<",">","<",">",
		">",">",">",">","<",">","<",">",
		">",">",">",">","<",">","<",">",
		"<","<","<","<","<","=","<","",
		">",">",">",">","",">","",">",
		">",">",">",">","",">","",">",
		"<","<","<","<","<","","<","=",
	};//优先表

bool IsOperator(char c)//判断是否有非法算符
{
	if((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='(')||(c==')')||(c=='='))
		return true;
	return false;
}
bool IsOperator2(string s)//判断算符使用合理性
{
	if((s=="+")||(s=="-")||(s=="*")||(s=="/")||(s=="="))
		return true;
	return false;
}

bool IsVt(string s)//判断是不是终结符
{
	if((s=="+")||(s=="-")||(s=="*")||(s=="/")||(s=="=")||(s=="(")||(s==")"))
		return true;
	else if((s[0]<=57)&&(s[0]>=48))
		return true;
	return false;
}

void Judge()//判断运算符使用是否正确
{
	int i = 0;
	if(vstr[vstr.size()-1]!="=")//无=,自动补上
	{
		vstr.push_back("=");
	}
	if(IsOperator2(vstr[0])||(vstr[0]==")"))//首个是+-*/)=报错
	{
		cout<<"运算错误"<<endl;
		exit(0);	
	}
	while((i+1)<vstr.size())//连续运算符报错
	{
		if(IsOperator2(vstr[i])&&IsOperator2(vstr[i+1]))
		{
			cout<<"运算错误"<<endl;
			exit(0);
		}
		i++;
	}
	
}

string GetPri(string s1, string s2)//确定优先级
{
	int col;
	if(s2=="+")
		col = 0;
	else if(s2 == "-")
		col = 1;
	else if(s2 == "*")
		col = 2;
	else if(s2 == "/")
		col = 3;
	else if(s2 == "(")
		col = 4;
	else if(s2 == ")")
		col = 5;
	else if((s2[0]<=57)&&(s2[0]>=48))
		col = 6;
	else if(s2 == "=")
		col = 7;

	if(s1=="+")
		return priTable[0][col];
	else if(s1=="-")
		return priTable[1][col];
	else if(s1=="*")
		return priTable[2][col];
	else if(s1=="/")
		return priTable[3][col];
	else if(s1=="(")
		return priTable[4][col];
	else if(s1==")")
		return priTable[5][col];
	else if((s1[0]<=57)&&(s1[0]>=48))
		return priTable[6][col];
	else if(s1=="=")
		return priTable[7][col];
}

void Split()//语法分析
{
	string newStr = "";
	int i = 0;
	while(i<len)
	{
		
		while((str[i]==32) && (i<len))//space
		{
			i++;
		}
		if((str[i]<=57) && (str[i]>=48) && (i<len))//0-9
		{
			while((str[i]<=57) && (str[i]>=48))
			{
				newStr += str[i];
				i++;
			}
		}
		else if((IsOperator(str[i])) && (i<len))
		{
			newStr += str[i];
			i++;	
		}
		else if(i<len)
		{
			cout<<"非法字符"<<endl;
			exit(0);
		}
		vstr.push_back(newStr);
		newStr = "";
	}
	//for(int j = 0; j<vstr.size(); j++)
	//{
	//	cout<<vstr[j]<<" ";
	//}

}

void Calculate()//算符优先计算
{
	int k = 0;//指向顶部
	int j;//指向顶部终结符
	int i = 0;//待进栈元素所在vector下标
	string sym;//待进栈元素
	string Q;//stack[j]上面的终结符
	string str;//规约后的值
	bool isVt = true;//若执行规约操作则设为f,如：元素3，在刚入栈时为vt，但在规约成新的3时，则不是vt了
	bool isRight = true;//运算是否出错的标志

	stack.push_back("=");
	//sym = vstr[0];

	cout<<endl<<"计算中间过程: "<<endl;
	do
	{
		sym = vstr[i];
		if(IsVt(stack[k])&&isVt)
			j = k;
		else
			j = k - 1;

		while(GetPri(stack[j], sym) == ">" &&(j>=0))//规约
		{
			do//确定哪些可规约
			{
				Q = stack[j];
				if(IsVt(stack[j-1]) && isVt 
					&& (stack[j]!=")"))//如果是(i)的形式，且i是已规约过的i，需要执行else
				{j = j-1;}
				else
					j = j-2;
			}while(GetPri(stack[j], Q)!="<");

			if((j+1)==k)//需要规约的只有一个i
			{
				str = Q;
				isVt = false;
			}
			else//3个需要规约i+i,i-i...
			{
				if(stack[j+2]=="+")//三个中间的是+，则两边的i执行+操作
				{
					strstream ss;//必须在此声明！！！
					int c=atoi(stack[j+1].c_str())+atoi(stack[j+3].c_str());
					ss<<c;
					ss>>str;
				}
				else if(stack[j+2]=="-")//三个中间的是-，则两边i的执行-操作
				{
					strstream ss;
					int c=atoi(stack[j+1].c_str())-atoi(stack[j+3].c_str());
					ss<<c;
					ss>>str;
				}
				else if(stack[j+2]=="*")
				{
					strstream ss;
					int c=atoi(stack[j+1].c_str())*atoi(stack[j+3].c_str());
					ss<<c;
					ss>>str;
				}
				else if(stack[j+2]=="/")
				{
					strstream ss;
					int c=atoi(stack[j+1].c_str())/atoi(stack[j+3].c_str());
					ss<<c;
					ss>>str;
					
				}
				else
				{
					str = stack[j+2];
				}

				isVt = false;
			}
			
			for(int n = j+1; n<=k; n++)//出
			{
				 
				stack.pop_back();
			}
			stack.push_back(str);//规约、进
			k = j+1;
			

			//测试输出
			for(int cc = 0; cc<stack.size(); cc++)
			{
				cout<<stack[cc];
			}
			cout<<endl;
		}
		
		if(((GetPri(stack[j], sym)=="<") || (GetPri(stack[j], sym)=="=")))//移进
		{
			stack.push_back(sym);
			k = k+1;
			
			//测试输出
			for(int cc = 0; cc<stack.size(); cc++)
			{
				cout<<stack[cc];
			}
			cout<<endl;
		}
		else
		{
			isRight = false;
			cout<<"错误"<<endl;
		}

		i++;
		isVt = true;
	}while((sym!="=")&&(i<vstr.size()));
	
	if(isRight)
		cout<<endl<<"最终结果: "<<stack[1]<<endl;
	else
		cout<<endl<<"运算错误"<<endl;
}


void main()
{
	cout<<"请输入算术表达式(数字之间不要输空格)："<<endl;
	gets(str); 
	len = strlen(str);
	Split();//词法分析
	Judge();//判断算术表达式
	Calculate();//算符优先计算
}