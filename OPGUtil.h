#pragma once
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<string>
#include"Grammar.h"
#include"GUtil.h"
#include <cassert>

#define RecordLastTerm RecordFirstTerm
#define LastTerm FirstTerm

using namespace std;

/// @brief ��¼һ������ʽ����Ϣ
class RecordFirstTerm {
public:
	int proID;// ����ʽID
	string left;// ����ʽ��
	string dependent;// ���̺��ķ��ս��
	set<string> firstSet;// �������ս��
	RecordFirstTerm() {
		proID = -1;
		firstSet.clear();
		left = "";
		dependent = "";
	}
	RecordFirstTerm(int _proID) {
		proID = _proID;
		left = "";
		dependent = "";
		firstSet.clear();
	}
	
	/// @brief ����һ�������ʽ�ļ�¼
	/// @param _proID 
	/// @param emp 
	RecordFirstTerm(int _proID, string _left) {
		proID = _proID;
		left = _left;
		dependent = "";
		firstSet.clear();
	}
};



/// @brief ���ս����FIRSTֵ
class FirstTerm {
public:
	string left;
	set<string> firstSet;
	FirstTerm() {
		left = "";
		firstSet.clear();
	}
};


class OPGUtil
{
public:
	static map<pair<char, char>, int> getFGTable(map<pair<char, char>, char>& table,Grammar& g) {
		map<pair<char, char>, int> fgTable;
		// Ԥ�����ҵ����е�<,>,=��ϵ������;���Ⱥ������ʼ��ȫΪ1
		map<pair<char, char>, char> lessTable;
		map<pair<char, char>, char> greaterTable;
		map<pair<char, char>, char> equalTable;
		initForFGTable(g,table,fgTable,lessTable,greaterTable,equalTable);
		// ��������
		bool isLoop = true;
		int cnt = 0;
		while ( isLoop ) {
			isLoop = false;
			cout << "����������" << ++cnt << endl;
			printFGTable(g, fgTable);
			// ����С��
			if (getRelation(fgTable, lessTable)) {
				isLoop = true;
			}
			// �������
			if (getRelation(fgTable, greaterTable)) {
				isLoop = true;
			}
			// �������
			if (getRelation(fgTable, equalTable)) {
				isLoop = true;
			}
		}
		return fgTable;
	}
	static vector<FirstTerm> getFirstTerm(Grammar& g) {
		// �̺���Ϣ��
		vector<RecordFirstTerm> vrft;
		vector<FirstTerm> vft;
		map<string, int> sig;
		// �����̺���Ϣ��
		generateRecordFirstTerm(g.productions,vrft);
		initMap(g.productions,sig);
		cout << "FIRSTTERM�̺���¼��\n";
		printRecordFirstTerm(vrft);
		while (!vrft.empty()) {
			// ��һ����ɾ������ֱ�����FIRSTTERM�ķ��ս��
			vector<FirstTerm> newVft = moveNoDependent(vrft);
			cout << "ֱ�����FIRSTTERM+���У�" << newVft.size() << endl;
			for (FirstTerm ft : newVft) {
				cout << ft.left << "\t";
				for (string s : ft.firstSet) {
					cout << s << ",";
				}
				cout << endl;
			}
			int start = vft.size();// ��������FIRST�ķ��ս������������vsf�е�λ��
			for (int i = 0; i < newVft.size(); i++) {
				sig[newVft[i].left] = start + i;
			}
			vft.insert(vft.end(),newVft.begin(), newVft.end());
			// �ڶ������������FIRSTTERM�ķ��ս���滻��vrf��
			replaceDependent(vrft, vft, sig);
		}
		cout << "���ս����FIRSTTERM+Ϊ��\n";
		printFirstTerm(vft);

		return vft;
	}

	static vector<LastTerm> getLastTerm(Grammar& g) {
		// �̺���Ϣ��
		vector<RecordLastTerm> vrft;
		vector<LastTerm> vft;
		map<string, int> sig;
		// �����̺���Ϣ��
		generateRecordLastTerm(g.productions, vrft);
		initMap(g.productions, sig);
		cout << "LASTTERM�̺���¼��\n";
		printRecordFirstTerm(vrft);
		while (!vrft.empty()) {
			// ��һ����ɾ������ֱ�����FIRSTTERM�ķ��ս��
			vector<FirstTerm> newVft = moveNoDependent(vrft);
			cout << "ֱ�����LASTTERM+���У�" << newVft.size() << endl;
			for (FirstTerm ft : newVft) {
				cout << ft.left << "\t";
				for (string s : ft.firstSet) {
					cout << s << ",";
				}
				cout << endl;
			}
			int start = vft.size();// ��������FIRST�ķ��ս������������vsf�е�λ��
			for (int i = 0; i < newVft.size(); i++) {
				sig[newVft[i].left] = start + i;
			}
			vft.insert(vft.end(), newVft.begin(), newVft.end());
			// �ڶ������������FIRSTTERM�ķ��ս���滻��vrf��
			replaceDependent(vrft, vft, sig);
		}
		cout << "���ս����LASTTERM+Ϊ��\n";
		printFirstTerm(vft);

		return vft;
	}
	static map<pair<char,char>, char> getTable(Grammar& g,vector<FirstTerm>& first, vector<LastTerm>& last) {
		map<pair<char, char>, char> table;
		// ����ĳ�����ս���������ʽ
		for (int i = 0; i < g.productions.size(); i++) {
			// ����ÿ������ʽ
			for (string production : g.productions[i].right) {
				
				// ������ʽ�ָ�Ϊ�ս���ͷ��ս��������
				vector<string> rights = PUTIL::getRightsFromPro(production);
				cout << "��ǰ����ʽ�ַ�����Ϊ: ";
				for (string s : rights) {
					cout << s << " ";
				}
				cout << endl;
				// Ѱ�ҹ�ϵ
				for (int j = 0; j < rights.size()-1; j++) {
					//cout << "��" << j << "�ε�����ʼ" << endl;
					// ab ���� aQb
					if( !PUTIL::isNon(rights[j]) ){
						if (!PUTIL::isNon(rights[j + 1])) {
							pair<char, char> p = { rights[j][0],rights[j + 1][0]};
							cout << "���� = ��ϵ: " << rights[j][0] << " " << rights[j + 1][0] << endl;
							table.insert({ p,'=' });
						}
						else if( j != (rights.size() - 2) ){
							if (!PUTIL::isNon(rights[j + 2])) {
								pair<char, char> p = { rights[j][0],rights[j + 2][0] };
								cout << "���� = ��ϵ: " << rights[j][0] << " " << rights[j + 2][0] << endl;
								table.insert({ p,'=' });
							}
							else {
								assert("���ڲ���ʽ��"+right+",������������ķ�!");
							}
						}
					}
					string sym1 = rights[j];
					string sym2 = rights[j + 1];
					// ���ս�����ս��, LAST(Non) > term
					if (PUTIL::isNon(sym1) && !PUTIL::isNon(sym2) ){
						int index = findLeft(last, sym1);
						cout << "���� > ��ϵ: " << sym1 << " " << sym2 << endl;
						cout << "LAST : " << last[index].left << " ";
						for (string s : last[index].firstSet) {
							cout << s << " ";
							pair<char, char> p = { s[0],sym2[0]};
							table.insert({ p, '>' });
						}
						cout << endl;
					}
					// �ս���ͷ��ս��, term < FIRST(Non)
					else if (!PUTIL::isNon(sym1) && PUTIL::isNon(sym2)) {
						int index = findLeft(first, sym2);
						cout << "���� < ��ϵ: " << sym1 << " " << sym2 << endl;
						cout << "FIRST: " << first[index].left << " ";
						for (string s : first[index].firstSet) {
							cout << s[0] << " ";
							pair<char, char> p = { sym1[0],s[0] };
							table.insert({ p, '<' });
						}
						cout << endl;
					}
					//cout << "��" << j << "�ε�������" << endl;
				}
			}
		}
		// ���Ͼ�ĩ�������ڹ�ϵ#E#
		int index1 = findLeft(first, g.productions[0].left);
		int index2 = findLeft(last, g.productions[0].left);
		for (string s : first[index1].firstSet) {
			table.insert({ { '#',s[0]},'<'});
		}
		for (string s : last[index2].firstSet) {
			table.insert({ { s[0],'#'},'>' });
		}
		/*for (char c : g.terms) {
			table.insert({ { c,'#' }, '>' });
			table.insert({ { '#',c }, '<' });
		}*/
		table.insert({ { '#','#'}, '=' });
		g.terms.insert('#');
		return table;
	}
	static map<pair<char, char>, char> getTableNo(Grammar& g, vector<FirstTerm>& first, vector<LastTerm>& last) {
		map<pair<char, char>, char> table;
		// ����ĳ�����ս���������ʽ
		for (int i = 0; i < g.productions.size(); i++) {
			// ����ÿ������ʽ
			for (string production : g.productions[i].right) {

				// ������ʽ�ָ�Ϊ�ս���ͷ��ս��������
				vector<string> rights = PUTIL::getRightsFromPro(production);
				cout << "��ǰ����ʽ�ַ�����Ϊ: ";
				for (string s : rights) {
					cout << s << " ";
				}
				cout << endl;
				// Ѱ�ҹ�ϵ
				for (int j = 0; j < rights.size() - 1; j++) {
					//cout << "��" << j << "�ε�����ʼ" << endl;
					// ab ���� aQb
					if (!PUTIL::isNon(rights[j])) {
						if (!PUTIL::isNon(rights[j + 1])) {
							pair<char, char> p = { rights[j][0],rights[j + 1][0] };
							cout << "���� = ��ϵ: " << rights[j][0] << " " << rights[j + 1][0] << endl;
							table.insert({ p,'=' });
						}
						else if (j != (rights.size() - 2)) {
							if (!PUTIL::isNon(rights[j + 2])) {
								pair<char, char> p = { rights[j][0],rights[j + 2][0] };
								cout << "���� = ��ϵ: " << rights[j][0] << " " << rights[j + 2][0] << endl;
								table.insert({ p,'=' });
							}
							else {
								assert("���ڲ���ʽ��" + right + ",������������ķ�!");
							}
						}
					}
					string sym1 = rights[j];
					string sym2 = rights[j + 1];
					// ���ս�����ս��, LAST(Non) > term
					if (PUTIL::isNon(sym1) && !PUTIL::isNon(sym2)) {
						int index = findLeft(last, sym1);
						cout << "���� > ��ϵ: " << sym1 << " " << sym2 << endl;
						cout << "LAST : " << last[index].left << " ";
						for (string s : last[index].firstSet) {
							cout << s << " ";
							pair<char, char> p = { s[0],sym2[0] };
							table.insert({ p, '>' });
						}
						cout << endl;
					}
					// �ս���ͷ��ս��, term < FIRST(Non)
					else if (!PUTIL::isNon(sym1) && PUTIL::isNon(sym2)) {
						int index = findLeft(first, sym2);
						cout << "���� < ��ϵ: " << sym1 << " " << sym2 << endl;
						cout << "FIRST: " << first[index].left << " ";
						for (string s : first[index].firstSet) {
							cout << s[0] << " ";
							pair<char, char> p = { sym1[0],s[0] };
							table.insert({ p, '<' });
						}
						cout << endl;
					}
				}
			}
		}
		
		g.terms.insert('#');
		return table;
	}
	static void printFGTable(Grammar& g, map<pair<char, char>, int>& fgTable) {
		for (char c : g.terms) {
			cout << "\t" << c;
		}
		cout << endl << "f\t";
		for (char c : g.terms) {
			pair<char, char> p = { 'f',c };
			if (fgTable.find(p) != fgTable.end()) {
				cout << fgTable[p];
			}
			cout << "\t";
		}
		cout << endl << "g\t";
		for (char c : g.terms) {
			pair<char, char> p = { 'g',c };
			if (fgTable.find(p) != fgTable.end()) {
				cout << fgTable[p];
			}
			cout << "\t";
		}
		cout << endl;
	}
private:
	
	static void initForFGTable(Grammar& g, map<pair<char, char>, char>& table, map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& lessTable, map<pair<char, char>, char>& greaterTable, map<pair<char, char>, char>& equalTable) {
		for (pair<pair<char, char>, char> p : table) {
			if (p.second == '<') {
				lessTable.insert(p);
			}
			else if (p.second == '>') {
				greaterTable.insert(p);
			}
			else {
				equalTable.insert(p);
			}
		}
		for (char c : g.terms) {
			fgTable.insert({ {'f',c},1 });
			fgTable.insert({ {'g',c},1 });
		}
	}
	/// <summary>
	/// �����ϵ
	/// </summary>
	/// <param name="fgTable"></param>
	/// <param name="relaTable"></param>
	/// <returns></returns>
	static bool getRelation(map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& relaTable) {
		bool isChange = false;
		// ѭ�����й�ϵ  c1  c2  rela
		for (pair<pair<char, char>, char> p : relaTable) { 
			if (p.second == '<') {// ʵ�� f(c1) < g(c2)
				if (fgTable[{'f', p.first.first}] >= fgTable[{'g', p.first.second}]) {
					fgTable[{'g', p.first.second}] = fgTable[{'f', p.first.first}] + 1;
					isChange = true;
				}
			}
			else if( p.second == '>') {// ʵ�� f(c1) > g(c2)
				if (fgTable[{'f', p.first.first}] <= fgTable[{'g', p.first.second}]) {
					fgTable[{'f', p.first.first}] = fgTable[{'g', p.first.second}] + 1;
					isChange = true;
				}
				
			}
			else {// ʵ�� f(c1) == g(c2)
				if (fgTable[{'f', p.first.first}] != fgTable[{'g', p.first.second}]) {
					int max = fgTable[{'f', p.first.first}] > fgTable[{'g', p.first.second}] ? fgTable[{'f', p.first.first}] : fgTable[{'g', p.first.second}];
					fgTable[{'f', p.first.first}] = fgTable[{'g', p.first.second}] = max;
					isChange = true;
				}
			}
		}
		return isChange;
	}
	
	static int findLeft(vector<FirstTerm>& vft,string left) {
		for (int i = 0; i < vft.size(); i++) {
			if (vft[i].left == left) {
				return i;
			}
		}
		return -1;
	}
	static void generateRecordLastTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft) {
		int proID = 0;
		// ����ĳ�����ս���������ʽ
		for (int i = 0; i < vp.size(); i++) {
			// ��
			string left = vp[i].left;
			// ��������ʽ����ȡ���ս����LASTTERM����ս������ս�����̺���ϵ
			for (int j = 0; j < vp[i].right.size(); j++) {
				RecordFirstTerm rft(proID++, left);
				string right = vp[i].right[j];
				// ������ʽת��Ϊ�ַ�����
				vector<string> rights = PUTIL::getRightsFromPro(right);
				// ��ʼ�±�
				int start = rights.size() - 1;
				// �жϺ������ַ��������һ���ս�����뵽LASTTERM�У�������һ���ַ�ʱ���ս��������̺���ϵ
				if (PUTIL::isNon(rights[start])) {// ���ս��
					rft.dependent = rights[start];
					if (left != rights[start]) {// ȥ�����̺�
						vrft.push_back(rft);
					}
					// �ж��Ƿ��������ַ�
					if (rights.size() > 1) {
						if (!PUTIL::isNon(rights[start - 1])) {// �ս��
							rft = RecordFirstTerm(proID++, left);
							rft.firstSet.insert(rights[start-1]);
							vrft.push_back(rft);
						}
						else {// ���ս��������
							assert("������������ķ�!");
						}
					}
				}
				else {
					rft.firstSet.insert(rights[start]);
					vrft.push_back(rft);
				}
			}
		}
	}
	static void printFirstTerm(vector<FirstTerm>& vft) {
		cout << "symbol\FirstTermSet" << endl;
		for (int i = 0; i < vft.size(); i++) {
			cout << vft[i].left << "\t";
			for (set<string>::iterator it = vft[i].firstSet.begin(); it != vft[i].firstSet.end(); it++) {
				cout << *it << " ";
			}
			cout << endl;
		}
	}
	static void replaceDependent(vector<RecordFirstTerm>& vrft, vector<FirstTerm>& vft, map<string, int>& sig) {
		for (int i = 0; i < vrft.size(); i++) {
			if (vrft[i].dependent != "") {// �ҵ����ܱ��滻��
				int pos = sig[vrft[i].dependent];
				if (pos >= 0) {//�����滻
					vrft[i].dependent = "";
					vrft[i].firstSet.insert(vft[pos].firstSet.begin(), vft[pos].firstSet.end());
				}
			}
		}
	}
	static vector<FirstTerm> moveNoDependent(vector<RecordFirstTerm>& vrft) {
		vector<FirstTerm> vft;
		int left = 0, right = 0;
		int pos = 0;
		pair<int, int> p = findNoDependent(vrft,pos);
		while (p.first < vrft.size()) {
			FirstTerm ft;
			ft.left = vrft[p.first].left;
			for (int i = p.first; i < p.second; i++) {
				ft.firstSet.insert(vrft[i].firstSet.begin(), vrft[i].firstSet.end());
			}
			while (p.second != p.first) {
				vrft.erase(vrft.begin() + p.first);
				p.second--;
			}
			vft.push_back(ft);
			p = findNoDependent(vrft, p.first);
		}
		return vft;
	}
	static pair<int, int> findNoDependent(vector<RecordFirstTerm>& vrft, int pos) {
		int left = pos, right = pos;
		bool is = true;
		while (right < vrft.size()) {
			if (vrft[left].left == vrft[right].left) {
				if (vrft[right].dependent != "") {// ��ǰ���ս�����ɣ�����
					while (vrft[right].left == vrft[left].left && right < vrft.size()) {
						right++;
					}
					left = right;
				}
				else {
					right++;
				}
			}
			else {// �ҵ�һ�����Դ����
				break;
			}
		}
		return make_pair(left, right);

	}
	//getFirst();
	static void initMap(vector<Production>& vp, map<string, int>& sig) {
		sig.clear();
		for (int i = 0; i < vp.size(); i++) {
			sig.insert({ vp[i].left , -1});
		}
	}
	/// <summary>
	/// �����̺���¼��
	/// </summary>
	/// <param name="vp"></param>
	/// <param name="vrft"></param>
	static void generateRecordFirstTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft) {
		int proID = 0;
		// ѭ��ÿ�����ս���������ʽ
		for (int i = 0; i < vp.size(); i++) {
			// ��
			string left = vp[i].left;
			// ѭ��ÿ������ʽ,��ȡ���ս����FIRSTTERM�ͷ��ս������ս����FIRSTTERM�̺���ϵ
			for (int j = 0; j < vp[i].right.size(); j++) {
				RecordFirstTerm rft(proID++,left);
				string right = vp[i].right[j];
				// ������ʽתΪ�ַ�����
				vector<string> rights = PUTIL::getRightsFromPro(right);
				// �ж�ǰ�����ַ���������һ�����ֵ��ս������firstterm��������̺���ϵ�����
				if (PUTIL::isNon(rights[0])) {// ���ս�������̺���ϵ
					rft.dependent = rights[0];
					if (left != rights[0]) {// ȥ�����̺�
						vrft.push_back(rft);
					}
					// �ж��Ƿ��еڶ����ַ������Ƿ�Ϊ�ս�������Ϊ���ս���򱨴�������������ķ�
					if (rights.size() > 1) {
						if (!PUTIL::isNon(rights[1])) {// �ս��
							rft = RecordFirstTerm(proID++, left);
							rft.firstSet.insert(rights[1]);
							vrft.push_back(rft);
						}
						else {// �����������ս������
							assert("������������ķ�!");
						}
					}
				}
				else {// �ս����ֻ�轫�ü���÷��ս����FIRSTTERM�м���
					rft.firstSet.insert(rights[0]);
					vrft.push_back(rft);
				}
			}
		}
	}
	static void printRecordFirstTerm(vector<RecordFirstTerm>& vrft) {
		cout << "proID\tleft\tdependent\tfirstSet" << endl;
		for (int i = 0; i < vrft.size(); i++) {
			cout << vrft[i].proID << "\t" << vrft[i].left << "\t" << vrft[i].dependent << "\t\t";
			for (set<string>::iterator it = vrft[i].firstSet.begin(); it != vrft[i].firstSet.end(); it++) {
				cout << *it << " ";
			}
			cout << endl;
		}
	}
};

