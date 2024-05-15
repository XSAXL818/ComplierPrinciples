#include<iostream>
#include<vector>
#include<algorithm>
#include<set> 
#include<map>
#include<stdio.h>
#include<queue>
#include<string>
#include <cstdlib>
#include"Grammar.h"
#include"GUtil.h"
#include"UI.h"
#include"FAUtil.h"
#include"RDUtil.h"
#include"CPPUtil.h"
#include"LEXUtil.h"
#include"OPGUtil.h"

using namespace std;

void f0(Grammar& g);
void f1(Grammar& g);
void f2(Grammar& g);
void f3(vector<DFA>& dfas);
void f4(vector<NFA>& nfas, vector<DFA>& dfas);
void f5(Grammar& g);
void f6(Grammar& g);
void f7(GrammarPlus& g);

//int funcID = 5;

int main() {
	vector<Grammar > grammars;
	vector< string > func;
	vector<NFA> nfas;
	vector<DFA> dfas;
	PUTIL::readGrammar(grammars, "input.txt");
	UI::readFunction(func, "function.txt");
	FAUtil::readDFA(dfas, "DFAInput.txt");
	FAUtil::readNFA(nfas, "NFAInput.txt");

	GrammarPlus gp;
	gp.productions = grammars[8].productions;
	gp.start = grammars[8].start;
	set<string> terms{ "if", "else", "then", ";", "e", "a"};
	gp.terms = terms;
	/*gp.nons = grammars[8].nons;
	for (char c : grammars[8].terms) {
		string t = "";
		t += c;
		gp.terms.insert(t);
	}*/

	while (1) {

		int sel;
		while (1) {
			cout << endl << "--------����1����������棬0ҡ��������----------" << endl;
			cin >> sel;
			if (sel == 1) {
				break;
			}
			else {
				return 0;
			}
		}
		
		pair<int, int> op = UI::select(grammars, func);
		if (op.first == -1) {
			return 0;
		}
		if (op.first > grammars.size()) {
			cout << "��ѡ����ȷ���ķ���" << endl;
			continue;
		}
		switch (op.second) {
		case 0:// �ӱ�Ƿ�
			f0(grammars[op.first]);
			break;
		case 1:// LL1�Զ������Ƶ�
			f1(grammars[op.first]);
			break;
		case 2:// ������ݹ�
			f2(grammars[op.first]);
			break;
		case 3:// DFA�Ƶ�
			f3(dfas);
			break;
		case 4:// NFAת��ΪDFA
			f4(nfas,dfas);
			break;
		case 5:// �ݹ��½�����
			f5(grammars[op.first]);
			break;
		case 6:// �������
			f6(grammars[op.first]);
			break;
		case 7:
			f7(gp);
			break;
		default:
			cout << "��������ȷ�Ĺ��ܱ�ţ�" << endl;
		}

	}
	return 0;
}


void f0(Grammar& g) {
	PUTIL::labelMethod(g.productions);
	cout << "----------�ӱ�Ƿ����������£�(����ļ�ΪlabelOutPut.txt)" << endl;
	PUTIL::fixNumber(g.productions);
	PUTIL::printGrammar(g);
	PUTIL::fwriteGrammar(g.productions, "labelOutPut.txt");
	cout << "--------------------------------" << endl;
}

void f1(Grammar& g) {

	cout << "��ѡ�ķ�Ϊ��\n";
	PUTIL::printGrammar(g);

	vector<string> terms;
	for (set<char>::iterator it = g.terms.begin(); it != g.terms.end(); it++) {
		string t = "";
		t += *it;
		terms.push_back(t);
	}
	terms.push_back("$");
	vector<ProductionFirst> vpf;
	map<LL1Key,string> LL1Table = PUTIL::getLL1Table(vpf,g.productions, terms);

	cout << "LL1�﷨���������£�" << endl;
	PUTIL::printLL1Table(LL1Table, g.productions, terms);
	string juzi;
	char c;
	while (1) {
		cout << "��ѡ�����¹���:\n";
		cout << "1.���̱�׼����\n2.��ȡ�ļ�(ÿ��Ϊһ����)\n3.�˳�\n";
		int op;
		cin >> op;
		//getchar();
		cin.get(c);
		if(op == 1) {
			while (1) {
				cout << "��������Ƶ��ĵľ��ӣ�(����@�˳�)" << endl;
				getline(cin, juzi);
				if (juzi == "@") {
					break;
				}
				int index = juzi.find(' ', 0);
				while (index != -1) {
					juzi.erase(index, 1);
					juzi.insert(index, "@");
					index++;
					index = juzi.find(' ', index);
				}
				PUTIL::topToBottom(juzi, LL1Table, g.productions, terms);
			}
			
		}
		else if (op == 2) {
			FILE* fp;
			fopen_s(&fp, "LL1input.txt", "r");
			if (fp == 0) {
				cout << "�ļ����ʧ��" << endl;
				break;
			}
			char str[100];
			while (fgets(str, sizeof(str), fp)) {
				string line = str;
				if (line[line.size() - 1] == '\n') {
					line.erase(line.size() - 1);
				}
				PUTIL::topToBottomNoProcess(line, LL1Table, g.productions, terms);

			}


			fclose(fp);
		}
		else if (op == 3) {
			break;
		}
	}
}
void f2(Grammar& g) {
	cout << "������ݹ���: " << endl;
	PUTIL::EliminateLeftRecursion(g.productions);
	PUTIL::fixNumber(g.productions);
	PUTIL::printGrammar(g);
	PUTIL::fwriteGrammar(g.productions, "output.txt");
}

void f3(vector<DFA>& dfas) {
	while (1) {
		FAUtil::printDFAs(dfas);
		cout << "������<DFA���,�ַ�>���в���������-1�˳�����" << endl;
		int opID=-1;
		string str;
		cin >> opID;
		cout << "opID: " << opID;
		if (opID == -1 || opID < 0 || opID > dfas.size() ) {
			break;
		}
		else {
			cin >> str;
			FAUtil::DFADerivedSentence(dfas[opID], str);
		}
		
	}
	
}
void f4(vector<NFA>& nfas, vector<DFA>& dfas) {
	while (1) {
		FAUtil::printNFAs(nfas);
		cout << "������<NFA���>���в���������-1�˳�����>" << endl;
		int opID;
		string str;
		cin >> opID;
		if (opID == -1) {
			break;
		}
		DFA dfa = FAUtil::NFAToDFA(nfas[opID]);
		string output = "DFAOutput.txt";
		cout << "DFA��������ļ�" << output <<"��" << endl;
		dfa.fwriteDFA(output.c_str());
		dfas.push_back(dfa);
	}
}
void LEX(string argv) {
	string op = "a.exe " + argv;
	system(op.c_str());
}
void load_lex() {
	string regular = "flex regular.l";
	// ���ɴʷ�������
	cout << "�ʷ���������.........................." << endl;
	if (system(regular.c_str()) == 0) {
		cout << "�����ļ�" << regular << "������...." << endl;
	}
	else {
		cout << "�����ļ�" << regular << "����ʧ��...." << endl;
	}
	if (system("gcc lex.yy.c") == 0) {
		cout << "�ʷ���������" << regular << "������...." << endl;
	}
	else {
		cout << "�ʷ���������" << regular << "����ʧ��...." << endl;
	}
}
/// <summary>
/// ���еݹ��½���������
/// </summary>
/// <param name="words"></param>
void executionRD(vector<string>& words) {
	// ����ݹ��½�����Դ�룬test.cpp�ļ�
	system("g++ \"D:\\Code\\CCode\\RDA\\RDanalysis.cpp\" -o \"D:\\Code\\CCode\\RDA\\RDanalysis.exe\"");
	cout << "��ȡ�ĵ���Ϊ: {";
	for (string s : words) {
		cout << s << ",";
	}
	cout << "}\n";
	// ���еݹ��½���������
	int ret = system("\"D:\\Code\\CCode\\RDA\\RDanalysis.exe\"");
	if (ret == 0) {
		cout << "********����һ�����ӣ�**********" << endl;
	}
	else {
		cout << "********�ⲻ��һ������!********" << endl;
	}
}
/// <summary>
/// �����ٱ���ݹ��½�����
/// </summary>
/// <param name="words"></param>
void executionRDNoCom(vector<string>& words) {
	//// ����ݹ��½�����Դ�룬test.cpp�ļ�
	cout << "��ȡ�ĵ���Ϊ: {";
	for (string s : words) {
		cout << s << ",";
	}
	cout << "}\n";
	// ���еݹ��½���������
	int ret = system("\"D:\\Code\\CCode\\RDA\\RDanalysis.exe\"");
	if (ret == 0) {
		cout << "********����һ�����ӣ�**********" << endl;
	}
	else {
		cout << "********�ⲻ��һ������!********" << endl;
	}
}
void f5(Grammar& g) {

	// ���شʷ�������
	LEXUtil::load_lex();
	// ��ȡ��ȡ�ִ��ָ�ĵ�������
	vector<string> words;
	string output = "Lexoutput.txt";// �ʷ���������Ľ������ļ�
	// ʹ�ôʷ������������ַ���
	while (1) {
		cout << "���ܣ�\n\t1.��ȡ�ļ�\n\t2.�����ִ�\n\t3.�˳�\n";
		string op;
		cin >> op;
		string para = "";
		if ( op == "1") {
			// �ʷ����������ļ�Lexinput.txt�е�Դ��ָ�ɵ���,��д���ļ�Lexoutput.txt��
			LEX(para);
			// ��ȡ�ʷ��������ָ�ĵ���
			words = CPPUtil::getToken(output.c_str());
			// �Զ����ɵݹ��½�����Դ��
			CPPUtil::RDAnalysis(g, words);
			// ִ�еݹ��½���������
			executionRD(words);
		}
		else if (op == "2") {
			bool isGcc = false;
			getchar();
			while (1) {
				cout << "������Ҫ�Ƶ���һ���ִ�������$�˳���\n";
				// ��ȡһ���ַ�
				getline(cin, para);
				if (para == "$") {
					break;
				}
				// �����ִ���ʹ����Ҫ��
				int index = para.find('*', 0);
				while (index != -1) {
					para.insert(index,"\\\\");
					index = para.find('*', index+3);
				}
				//para = "\"" + para + "\"";
				//cout << "ת���" << para << endl;
				// �ʷ��������������ִ��ָ�ɵ���,��д���ļ�Lexoutput.txt��
				LEX(para);
				// ��ȡ�ʷ��������ָ�ĵ���
				words = CPPUtil::getToken(output.c_str());
				// �Զ����ɵݹ��½�����Դ��
				CPPUtil::RDAnalysis(g, words);
				// ִ�еݹ��½���������
				if (!isGcc) {// ���ⷵ�ر���
					executionRD(words);
					isGcc = !isGcc;
				}
				else {
					executionRDNoCom(words);
				}
			}
		}
		else {
			break;
		}
	}	
}
void f6(Grammar& grammar) {
	cout << "----------------���FIRSTTERM��------------------" << endl;
	vector<FirstTerm> first = OPGUtil::getFirstTerm(grammar);
	cout << "----------------���LASTTERM��------------------" << endl;
	vector<LastTerm> last = OPGUtil::getLastTerm(grammar);
	cout << "----------------���������ȱ���------------------" << endl;
	map<pair<char, char>, char> table = OPGUtil::getTable(grammar,first,last);
	map<pair<char, char>, char> noTable = OPGUtil::getTableNo(grammar, first, last);
	cout << "������Ⱦ���\n";
	for (char c1 : grammar.terms) {
		cout << "\t" << c1;
	}
	cout << endl;
	for (char c1 : grammar.terms) {
		cout << c1 << "\t";
		for (char c2 : grammar.terms) {
			pair<char, char> p = { c1,c2 };
			if (table.find(p) != table.end() ) {
				cout << table[p] << "\t";
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
	grammar.terms.erase('#');
	cout << "----------------���������ȱ���------------------" << endl;
	static map<pair<char, char>, int> fgTable = OPGUtil::getFGTable(noTable, grammar);
	cout << endl<<  "���Ⱥ���������:" << endl;
	OPGUtil::printFGTable(grammar, fgTable);
}

void f7(GrammarPlus& gp) {
	
}
//void f7(GrammarPlus& gp) {
//	cout << "������ĵ��ʼ��ս����{";
//	vector<string> terms;
//	for (string s : gp.terms) {
//		terms.push_back(s);
//	}
//	cout << "}\n";
//	terms.push_back("$");
//	map<LL1Key, string> LL1Table = PUTIL::getLL1Table(gp.productions, terms);
//	cout << "LL1�﷨���������£�" << endl;
//	for (int i = 0; i < terms.size(); i++) {
//		cout << "\t" << terms[i];
//	}
//	cout << endl;
//	PUTIL::printLL1Table(LL1Table, gp.productions, terms);
//	string juzi;
//	while (1) {
//		getchar();
//		cout << "��������Ƶ��ĵľ��ӣ�(����@�˳�)" << endl;
//		getline(cin, juzi);
//		if (juzi == "@") {
//			break;
//		}
//		PUTIL::topToBottom(juzi, LL1Table, gp.productions, terms);
//	}
//}