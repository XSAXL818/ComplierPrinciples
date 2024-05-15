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

/// @brief 记录一个产生式的信息
class RecordFirstTerm {
public:
	int proID;// 产生式ID
	string left;// 产生式左部
	string dependent;// 被蕴含的非终结符
	set<string> firstSet;// 包含的终结符
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
	
	/// @brief 创建一个虚产生式的记录
	/// @param _proID 
	/// @param emp 
	RecordFirstTerm(int _proID, string _left) {
		proID = _proID;
		left = _left;
		dependent = "";
		firstSet.clear();
	}
};



/// @brief 非终结符的FIRST值
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
		// 预处理：找到所有的<,>,=关系并保存;优先函数表初始化全为1
		map<pair<char, char>, char> lessTable;
		map<pair<char, char>, char> greaterTable;
		map<pair<char, char>, char> equalTable;
		initForFGTable(g,table,fgTable,lessTable,greaterTable,equalTable);
		// 迭代处理
		bool isLoop = true;
		int cnt = 0;
		while ( isLoop ) {
			isLoop = false;
			cout << "迭代次数：" << ++cnt << endl;
			printFGTable(g, fgTable);
			// 处理小于
			if (getRelation(fgTable, lessTable)) {
				isLoop = true;
			}
			// 处理大于
			if (getRelation(fgTable, greaterTable)) {
				isLoop = true;
			}
			// 处理等于
			if (getRelation(fgTable, equalTable)) {
				isLoop = true;
			}
		}
		return fgTable;
	}
	static vector<FirstTerm> getFirstTerm(Grammar& g) {
		// 蕴含信息表
		vector<RecordFirstTerm> vrft;
		vector<FirstTerm> vft;
		map<string, int> sig;
		// 生成蕴含信息表
		generateRecordFirstTerm(g.productions,vrft);
		initMap(g.productions,sig);
		cout << "FIRSTTERM蕴含记录表\n";
		printRecordFirstTerm(vrft);
		while (!vrft.empty()) {
			// 第一步，删除可以直接求出FIRSTTERM的非终结符
			vector<FirstTerm> newVft = moveNoDependent(vrft);
			cout << "直接求出FIRSTTERM+的有：" << newVft.size() << endl;
			for (FirstTerm ft : newVft) {
				cout << ft.left << "\t";
				for (string s : ft.firstSet) {
					cout << s << ",";
				}
				cout << endl;
			}
			int start = vft.size();// 标记已求出FIRST的非终结符，及其所在vsf中的位置
			for (int i = 0; i < newVft.size(); i++) {
				sig[newVft[i].left] = start + i;
			}
			vft.insert(vft.end(),newVft.begin(), newVft.end());
			// 第二步，将已求出FIRSTTERM的非终结符替换到vrf中
			replaceDependent(vrft, vft, sig);
		}
		cout << "非终结符的FIRSTTERM+为：\n";
		printFirstTerm(vft);

		return vft;
	}

	static vector<LastTerm> getLastTerm(Grammar& g) {
		// 蕴含信息表
		vector<RecordLastTerm> vrft;
		vector<LastTerm> vft;
		map<string, int> sig;
		// 生成蕴含信息表
		generateRecordLastTerm(g.productions, vrft);
		initMap(g.productions, sig);
		cout << "LASTTERM蕴含记录表\n";
		printRecordFirstTerm(vrft);
		while (!vrft.empty()) {
			// 第一步，删除可以直接求出FIRSTTERM的非终结符
			vector<FirstTerm> newVft = moveNoDependent(vrft);
			cout << "直接求出LASTTERM+的有：" << newVft.size() << endl;
			for (FirstTerm ft : newVft) {
				cout << ft.left << "\t";
				for (string s : ft.firstSet) {
					cout << s << ",";
				}
				cout << endl;
			}
			int start = vft.size();// 标记已求出FIRST的非终结符，及其所在vsf中的位置
			for (int i = 0; i < newVft.size(); i++) {
				sig[newVft[i].left] = start + i;
			}
			vft.insert(vft.end(), newVft.begin(), newVft.end());
			// 第二步，将已求出FIRSTTERM的非终结符替换到vrf中
			replaceDependent(vrft, vft, sig);
		}
		cout << "非终结符的LASTTERM+为：\n";
		printFirstTerm(vft);

		return vft;
	}
	static map<pair<char,char>, char> getTable(Grammar& g,vector<FirstTerm>& first, vector<LastTerm>& last) {
		map<pair<char, char>, char> table;
		// 遍历某个非终结符及其产生式
		for (int i = 0; i < g.productions.size(); i++) {
			// 遍历每个产生式
			for (string production : g.productions[i].right) {
				
				// 将产生式分割为终结符和非终结符的数组
				vector<string> rights = PUTIL::getRightsFromPro(production);
				cout << "当前产生式字符数组为: ";
				for (string s : rights) {
					cout << s << " ";
				}
				cout << endl;
				// 寻找关系
				for (int j = 0; j < rights.size()-1; j++) {
					//cout << "第" << j << "次迭代开始" << endl;
					// ab 或者 aQb
					if( !PUTIL::isNon(rights[j]) ){
						if (!PUTIL::isNon(rights[j + 1])) {
							pair<char, char> p = { rights[j][0],rights[j + 1][0]};
							cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 1][0] << endl;
							table.insert({ p,'=' });
						}
						else if( j != (rights.size() - 2) ){
							if (!PUTIL::isNon(rights[j + 2])) {
								pair<char, char> p = { rights[j][0],rights[j + 2][0] };
								cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 2][0] << endl;
								table.insert({ p,'=' });
							}
							else {
								assert("存在产生式："+right+",不是算符优先文法!");
							}
						}
					}
					string sym1 = rights[j];
					string sym2 = rights[j + 1];
					// 非终结符和终结符, LAST(Non) > term
					if (PUTIL::isNon(sym1) && !PUTIL::isNon(sym2) ){
						int index = findLeft(last, sym1);
						cout << "存在 > 关系: " << sym1 << " " << sym2 << endl;
						cout << "LAST : " << last[index].left << " ";
						for (string s : last[index].firstSet) {
							cout << s << " ";
							pair<char, char> p = { s[0],sym2[0]};
							table.insert({ p, '>' });
						}
						cout << endl;
					}
					// 终结符和非终结符, term < FIRST(Non)
					else if (!PUTIL::isNon(sym1) && PUTIL::isNon(sym2)) {
						int index = findLeft(first, sym2);
						cout << "存在 < 关系: " << sym1 << " " << sym2 << endl;
						cout << "FIRST: " << first[index].left << " ";
						for (string s : first[index].firstSet) {
							cout << s[0] << " ";
							pair<char, char> p = { sym1[0],s[0] };
							table.insert({ p, '<' });
						}
						cout << endl;
					}
					//cout << "第" << j << "次迭代结束" << endl;
				}
			}
		}
		// 加上句末符，存在关系#E#
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
		// 遍历某个非终结符及其产生式
		for (int i = 0; i < g.productions.size(); i++) {
			// 遍历每个产生式
			for (string production : g.productions[i].right) {

				// 将产生式分割为终结符和非终结符的数组
				vector<string> rights = PUTIL::getRightsFromPro(production);
				cout << "当前产生式字符数组为: ";
				for (string s : rights) {
					cout << s << " ";
				}
				cout << endl;
				// 寻找关系
				for (int j = 0; j < rights.size() - 1; j++) {
					//cout << "第" << j << "次迭代开始" << endl;
					// ab 或者 aQb
					if (!PUTIL::isNon(rights[j])) {
						if (!PUTIL::isNon(rights[j + 1])) {
							pair<char, char> p = { rights[j][0],rights[j + 1][0] };
							cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 1][0] << endl;
							table.insert({ p,'=' });
						}
						else if (j != (rights.size() - 2)) {
							if (!PUTIL::isNon(rights[j + 2])) {
								pair<char, char> p = { rights[j][0],rights[j + 2][0] };
								cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 2][0] << endl;
								table.insert({ p,'=' });
							}
							else {
								assert("存在产生式：" + right + ",不是算符优先文法!");
							}
						}
					}
					string sym1 = rights[j];
					string sym2 = rights[j + 1];
					// 非终结符和终结符, LAST(Non) > term
					if (PUTIL::isNon(sym1) && !PUTIL::isNon(sym2)) {
						int index = findLeft(last, sym1);
						cout << "存在 > 关系: " << sym1 << " " << sym2 << endl;
						cout << "LAST : " << last[index].left << " ";
						for (string s : last[index].firstSet) {
							cout << s << " ";
							pair<char, char> p = { s[0],sym2[0] };
							table.insert({ p, '>' });
						}
						cout << endl;
					}
					// 终结符和非终结符, term < FIRST(Non)
					else if (!PUTIL::isNon(sym1) && PUTIL::isNon(sym2)) {
						int index = findLeft(first, sym2);
						cout << "存在 < 关系: " << sym1 << " " << sym2 << endl;
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
	/// 处理关系
	/// </summary>
	/// <param name="fgTable"></param>
	/// <param name="relaTable"></param>
	/// <returns></returns>
	static bool getRelation(map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& relaTable) {
		bool isChange = false;
		// 循环所有关系  c1  c2  rela
		for (pair<pair<char, char>, char> p : relaTable) { 
			if (p.second == '<') {// 实现 f(c1) < g(c2)
				if (fgTable[{'f', p.first.first}] >= fgTable[{'g', p.first.second}]) {
					fgTable[{'g', p.first.second}] = fgTable[{'f', p.first.first}] + 1;
					isChange = true;
				}
			}
			else if( p.second == '>') {// 实现 f(c1) > g(c2)
				if (fgTable[{'f', p.first.first}] <= fgTable[{'g', p.first.second}]) {
					fgTable[{'f', p.first.first}] = fgTable[{'g', p.first.second}] + 1;
					isChange = true;
				}
				
			}
			else {// 实现 f(c1) == g(c2)
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
		// 遍历某个非终结符及其产生式
		for (int i = 0; i < vp.size(); i++) {
			// 左部
			string left = vp[i].left;
			// 遍历产生式，获取非终结符的LASTTERM与非终结符与非终结符的蕴含关系
			for (int j = 0; j < vp[i].right.size(); j++) {
				RecordFirstTerm rft(proID++, left);
				string right = vp[i].right[j];
				// 将产生式转换为字符数组
				vector<string> rights = PUTIL::getRightsFromPro(right);
				// 开始下标
				int start = rights.size() - 1;
				// 判断后两个字符，将最后一个终结符加入到LASTTERM中，如果最后一个字符时非终结符则加入蕴含关系
				if (PUTIL::isNon(rights[start])) {// 非终结符
					rft.dependent = rights[start];
					if (left != rights[start]) {// 去除自蕴含
						vrft.push_back(rft);
					}
					// 判断是否有两个字符
					if (rights.size() > 1) {
						if (!PUTIL::isNon(rights[start - 1])) {// 终结符
							rft = RecordFirstTerm(proID++, left);
							rft.firstSet.insert(rights[start-1]);
							vrft.push_back(rft);
						}
						else {// 非终结符，报错
							assert("不是算符优先文法!");
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
			if (vrft[i].dependent != "") {// 找到可能被替换的
				int pos = sig[vrft[i].dependent];
				if (pos >= 0) {//可以替换
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
				if (vrft[right].dependent != "") {// 当前非终结符不可，跳过
					while (vrft[right].left == vrft[left].left && right < vrft.size()) {
						right++;
					}
					left = right;
				}
				else {
					right++;
				}
			}
			else {// 找到一个可以处理的
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
	/// 生成蕴含记录表
	/// </summary>
	/// <param name="vp"></param>
	/// <param name="vrft"></param>
	static void generateRecordFirstTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft) {
		int proID = 0;
		// 循环每个非终结符及其产生式
		for (int i = 0; i < vp.size(); i++) {
			// 左部
			string left = vp[i].left;
			// 循环每个产生式,获取非终结符的FIRSTTERM和非终结符与非终结符的FIRSTTERM蕴含关系
			for (int j = 0; j < vp[i].right.size(); j++) {
				RecordFirstTerm rft(proID++,left);
				string right = vp[i].right[j];
				// 将产生式转为字符数组
				vector<string> rights = PUTIL::getRightsFromPro(right);
				// 判断前两个字符，并将第一个出现的终结符加入firstterm，如果有蕴含关系则加入
				if (PUTIL::isNon(rights[0])) {// 非终结符，有蕴含关系
					rft.dependent = rights[0];
					if (left != rights[0]) {// 去除自蕴含
						vrft.push_back(rft);
					}
					// 判断是否有第二个字符，且是否为终结符，如果为非终结符则报错，不是算符优先文法
					if (rights.size() > 1) {
						if (!PUTIL::isNon(rights[1])) {// 终结符
							rft = RecordFirstTerm(proID++, left);
							rft.firstSet.insert(rights[1]);
							vrft.push_back(rft);
						}
						else {// 出现两个非终结符相连
							assert("不是算符优先文法!");
						}
					}
				}
				else {// 终结符，只需将该加入该非终结符的FIRSTTERM中即可
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

