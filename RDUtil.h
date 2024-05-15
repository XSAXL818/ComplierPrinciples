#pragma once
#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<stack>
#include<queue>
#include<map>
#include<algorithm>
#include"Grammar.h"

using namespace std;
constexpr auto NON = 0;
constexpr auto TERM = 1;

// ���ţ�������ֵ�����ͣ����ս�����ս����
class SignType {
public:
	int type;// ����
	string value;// ֵ
	SignType(){
		type = 0;
		value = "";
	}
	SignType(int _type, string _value){
		type = _type;
		value = _value;
	}
	bool operator<( SignType s ) const {
		if (type != s.type) {
			return type < s.type;
		}
		return value < s.value;
	}
};



class RDGrammar {
public:
	string start;//��ʼ��
	set<string> nons;// ���ս������
	set<string> terms;// �ս������
	set<string> str_terms;// ������ĵ���
	set<string> null_pro;// ���пղ���ʽ�ķ��ս������
	map<string, vector< vector<SignType> > > regulars;// ĳ�����ս���Ĺ���
	RDGrammar() {
		start = "";
		nons.clear();
		terms.clear();
		regulars.clear();
		str_terms.clear();
		null_pro.clear();
	}
	RDGrammar(string _start, set<string> _nons, set<string> _terms, set<string> _str_terms, set<string> _null_pro, map<string, vector< vector<SignType> > > _regulars) {
		start = _start;
		nons = _nons;
		terms = _terms;
		regulars = _regulars;
		str_terms = _str_terms;
		null_pro = _null_pro;
	}


	/// <summary>
	/// ��ӡ�ķ�
	/// </summary>
	void printRDGrammar();
};


class RDGUtil{
public:
	static pair<int, bool> stringToInt(string str);
	static vector<SignType> getSignTypeFromProduction(string production);
	static bool isDelimiter(char c);
	static bool isOperator();
	static RDGrammar grammarToRDGrammar(Grammar& g);



private:
	static string switchString(string src);
};

