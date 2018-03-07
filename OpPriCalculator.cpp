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
	};//���ȱ�

bool IsOperator(char c)//�ж��Ƿ��зǷ����
{
	if((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='(')||(c==')')||(c=='='))
		return true;
	return false;
}
bool IsOperator2(string s)//�ж����ʹ�ú�����
{
	if((s=="+")||(s=="-")||(s=="*")||(s=="/")||(s=="="))
		return true;
	return false;
}

bool IsVt(string s)//�ж��ǲ����ս��
{
	if((s=="+")||(s=="-")||(s=="*")||(s=="/")||(s=="=")||(s=="(")||(s==")"))
		return true;
	else if((s[0]<=57)&&(s[0]>=48))
		return true;
	return false;
}

void Judge()//�ж������ʹ���Ƿ���ȷ
{
	int i = 0;
	if(vstr[vstr.size()-1]!="=")//��=,�Զ�����
	{
		vstr.push_back("=");
	}
	if(IsOperator2(vstr[0])||(vstr[0]==")"))//�׸���+-*/)=����
	{
		cout<<"�������"<<endl;
		exit(0);	
	}
	while((i+1)<vstr.size())//�������������
	{
		if(IsOperator2(vstr[i])&&IsOperator2(vstr[i+1]))
		{
			cout<<"�������"<<endl;
			exit(0);
		}
		i++;
	}
	
}

string GetPri(string s1, string s2)//ȷ�����ȼ�
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

void Split()//�﷨����
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
			cout<<"�Ƿ��ַ�"<<endl;
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

void Calculate()//������ȼ���
{
	int k = 0;//ָ�򶥲�
	int j;//ָ�򶥲��ս��
	int i = 0;//����ջԪ������vector�±�
	string sym;//����ջԪ��
	string Q;//stack[j]������ս��
	string str;//��Լ���ֵ
	bool isVt = true;//��ִ�й�Լ��������Ϊf,�磺Ԫ��3���ڸ���ջʱΪvt�����ڹ�Լ���µ�3ʱ������vt��
	bool isRight = true;//�����Ƿ����ı�־

	stack.push_back("=");
	//sym = vstr[0];

	cout<<endl<<"�����м����: "<<endl;
	do
	{
		sym = vstr[i];
		if(IsVt(stack[k])&&isVt)
			j = k;
		else
			j = k - 1;

		while(GetPri(stack[j], sym) == ">" &&(j>=0))//��Լ
		{
			do//ȷ����Щ�ɹ�Լ
			{
				Q = stack[j];
				if(IsVt(stack[j-1]) && isVt 
					&& (stack[j]!=")"))//�����(i)����ʽ����i���ѹ�Լ����i����Ҫִ��else
				{j = j-1;}
				else
					j = j-2;
			}while(GetPri(stack[j], Q)!="<");

			if((j+1)==k)//��Ҫ��Լ��ֻ��һ��i
			{
				str = Q;
				isVt = false;
			}
			else//3����Ҫ��Լi+i,i-i...
			{
				if(stack[j+2]=="+")//�����м����+�������ߵ�iִ��+����
				{
					strstream ss;//�����ڴ�����������
					int c=atoi(stack[j+1].c_str())+atoi(stack[j+3].c_str());
					ss<<c;
					ss>>str;
				}
				else if(stack[j+2]=="-")//�����м����-��������i��ִ��-����
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
			
			for(int n = j+1; n<=k; n++)//��
			{
				 
				stack.pop_back();
			}
			stack.push_back(str);//��Լ����
			k = j+1;
			

			//�������
			for(int cc = 0; cc<stack.size(); cc++)
			{
				cout<<stack[cc];
			}
			cout<<endl;
		}
		
		if(((GetPri(stack[j], sym)=="<") || (GetPri(stack[j], sym)=="=")))//�ƽ�
		{
			stack.push_back(sym);
			k = k+1;
			
			//�������
			for(int cc = 0; cc<stack.size(); cc++)
			{
				cout<<stack[cc];
			}
			cout<<endl;
		}
		else
		{
			isRight = false;
			cout<<"����"<<endl;
		}

		i++;
		isVt = true;
	}while((sym!="=")&&(i<vstr.size()));
	
	if(isRight)
		cout<<endl<<"���ս��: "<<stack[1]<<endl;
	else
		cout<<endl<<"�������"<<endl;
}


void main()
{
	cout<<"�������������ʽ(����֮�䲻Ҫ��ո�)��"<<endl;
	gets(str); 
	len = strlen(str);
	Split();//�ʷ�����
	Judge();//�ж��������ʽ
	Calculate();//������ȼ���
}