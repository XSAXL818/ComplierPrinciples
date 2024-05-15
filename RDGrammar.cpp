#include<iostream>
#include<vector>
#include<string>
using namespace std;

void E();
void E1();
void F();
void T();
void T1();
int main()
{
	GetSymbol();
	E();
}
void E()
{
	T();
	E1();
} 
void E1()
{
	if(sym=="+")
	{	
		GetSymbol();
		T();
		E1();
	}
}
void F()
{
	if(sym=="(")
	{	
		GetSymbol();
		E();
		if(sym==")")
		{		
			GetSymbol();
		}
		else {
			Error();
		}
	}
	else if(sym="i")
	{
		GetSymbol();
	}
	else {
		Error();
	}
}
void T()
{
	F();
	T1();
}
void T1()
{
	if(sym=="*")
	{	
		GetSymbol();
		F();
		T1();
	}
}
