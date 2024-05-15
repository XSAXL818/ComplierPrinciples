#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<map>
#include<vector>
#include<stdio.h>
#include<stack>
#include<set>
#include<queue>
#include<algorithm>
#include"Grammar.h"

using namespace std;
// NFA��״̬��������NFAת��ΪDFA
typedef set<string> NFASTATUS;


/// <summary>
/// ���ڱ�ʾ��㼰��ĳ�������ϵ��ַ�
/// </summary>
class DFAKey {
public:
	string ver;
	char edge;
	DFAKey(string _ver, char _edge) {
		ver = _ver;
		edge = _edge;
	}
	DFAKey() {
		ver = "";
		edge = 0;
	}
	bool operator<(DFAKey in) const;
};
class NFAtoDFAKey {
public:
	NFASTATUS NFAStatus;
	char input;

	bool operator<(NFAtoDFAKey key) const {
		if (input != key.input) {
			return input < key.input;
		}
		return NFAStatus < key.NFAStatus;
	}
	NFAtoDFAKey(NFASTATUS status, char in) {
		NFAStatus = status;
		input = in;
	}
	NFAtoDFAKey() {};
};


class RecordClosure{
public:
	string symbol;// ����
	set<string> dependent;// �̺��ķ���
	set<string> closure;// �հ�

	RecordClosure(string _symbol, set<string> _dependent, set<string> _closure);
	bool operator<(RecordClosure rc) const;

};

class NFA {
public:
	set<string> statusSet;//״̬����
	set<char> inputLetter;//�����ַ�����
	map< DFAKey, set<string> > mapTable;//ӳ�伯��
	set<string> start;//��ʼ״̬����
	set<string> finalSet;//��ֹ״̬����
	// ״̬��@_closure,��״̬���ܿ��ַ��ܵ��������״̬
	map< string, set<string> > nullClosure;
	// ״̬��letter_closure,��״̬����letter�ַ����ܵ��������״̬
	map< DFAKey, set<string> > letterClosure;
	NFA(set<string> _statusSet, set<char> _inputLetter, map< DFAKey, set<string> > _mapTable, set<string> _start, set<string> _finalSet);
	NFA(){}
	/// <summary>
	/// ��ȡ״̬��@_closure
	/// </summary>
	void initClosure();
	void printNFA();
	void fwriteNFA(string filePath);
private:
	/// <summary>
	/// ��ʼ����¼��
	/// </summary>
	/// <param name="vrc"></param>
	void initRecordClosure(vector<RecordClosure>& vrc);
	/// <summary>
	/// ��ʼ���ձհ�
	/// </summary>
	/// <param name="vrc"></param>
	void initNullClosure(vector<RecordClosure>& vrc);
	/// <summary>
	/// ɾ���̺����п���ֱ������հ���������뵽nullClousure��
	/// </summary>
	/// <param name="vrc"></param>
	void eraseNullDependent(vector<RecordClosure>& vrc);
	/// <summary>
	/// �������@_closure���ַ�������̺����dependent��
	/// </summary>
	/// <param name="vrc"></param>
	void replaceDependent(vector<RecordClosure>& vrc);
	/// <summary>
	/// ��ӡ��¼��
	/// </summary>
	/// <param name="vrc"></param>
	void printRecordClosure(vector<RecordClosure>& vrc);
	/// <summary>
	/// ��ӡ@_closure
	/// </summary>
	void printNullClosure();
	/// <summary>
	/// ��ʼ��x_closure
	/// </summary>
	void initLetterClosure();
	void printLetterClosure();
	void eraseNullClosure();
};

class DFA {
public:
	set<string> statusSet;//״̬����
	set<char> inputLetter;//�����ַ�����
	map<DFAKey, string> mapTable;//ӳ�伯��
	string start;//��ʼ״̬
	set<string> finalSet;//��ֹ״̬����
	DFA(){}
	DFA(set<string> _statusSet, set<char> _inputLetter, map<DFAKey, string> _mapTable, string _start, set<string> _finalSet);
	void printDFA();
	void printMatDFA();
	void simplification();
	void fwriteDFA( string filePath );
private:
	vector<set<string> > divideTwoGroup(set<string> group, set< set<string> >& allGroup);
	set<string> getDFAStatusSetLetterClosure(set<string>& statusSet, char letter);

};

/// <summary>
/// NFAת��ΪDFA���м��ʾ����Ԫ��
/// </summary>
class NFAtoDFA {
public:
	set< NFASTATUS > statusSet;//״̬����
	set<char> inputLetter;//�����ַ�����
	map<NFAtoDFAKey, NFASTATUS> mapTable;//ӳ�伯��
	NFASTATUS start;//��ʼ״̬����
	set< NFASTATUS > finalSet;//��ֹ״̬����
	map<NFASTATUS, string> statusID;//��״̬���ϵ����±��
	vector< NFASTATUS > stateID;//����״̬���ϵı��
	void printNFAtoDFA();
	DFA toDFA();
private:
	void printNFAStatus(const NFASTATUS& sta);
	void printFinalSet();
	void renumber();
	void printMapTable();
	void printStart();
	void printStatusSet();
};

class FAUtil
{
public:
	static void readDFA( vector<DFA>& dfas, string filePatg);
	static void readNFA( vector<NFA>& nfas, string filePath );
	static void printDFAs(vector<DFA>& dfas);
	static void printNFAs(vector<NFA>& nfas);
	static void DFADerivedSentence(DFA& dfa,string str);
	static void printMatDFA( DFA& dfa );
	static void printMatNFA(NFA& nfa);
	static DFA NFAToDFA( NFA& nfa );
	static void printGroupStack(stack< set<string> > st);
	static bool isInclude( set<string> s1, set<string> s2 );
private:
	static bool allNum(string str);
	static int stringToNum(string str);
	static NFAtoDFA equivalentReplacement(NFA& nfa);
	
};