#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<string>
#include<stdio.h>
#include"Grammar.h"


using namespace std;

// ���ţ�������ֵ�����ͣ����ս�����ս����
class Word {
public:
	int type;// ����
	string value;// ֵ
	Word() {
		type = 0;
		value = "";
	}
	Word(int _type, string _value) {
		type = _type;
		value = _value;
	}
	bool operator<(Word s) const {
		if (type != s.type) {
			return type < s.type;
		}
		return value < s.value;
	}
};

class LEXUtil{
public:
	static void LEX(string argv) {
		string op = "a.exe " + argv;
		system(op.c_str());
	}
	static void load_lex() {
		string regular = "flex regular.l";
		// ���ɴʷ�������
		cout << "�ʷ���������.........................." << endl;
		if (system(regular.c_str()) == 0) {
			cout << "�����ļ�:" << regular << " ������...." << endl;
		}
		else {
			cout << "�����ļ�: " << regular << " ����ʧ��...." << endl;
		}
		if (system("gcc lex.yy.c") == 0) {
			cout << "�ʷ���������" << regular << "������...." << endl;
		}
		else {
			cout << "�ʷ���������" << regular << "����ʧ��...." << endl;
		}
	}

	static vector<Word> getWordsFromProduction( string pro, GrammarPlus& gp ) {
		FILE* fp;
		fopen_s(&fp,"Lexoutput.txt", "r");
		




		fclose(fp);
	}
private:
	
};

