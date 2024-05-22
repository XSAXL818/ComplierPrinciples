#include "OPGUtil.h"


RecordFirstTerm::RecordFirstTerm() {
	proID = -1;
	firstSet.clear();
	left = "";
	dependent = "";
}

RecordFirstTerm::RecordFirstTerm(int _proID) {
	proID = _proID;
	left = "";
	dependent = "";
	firstSet.clear();
}

RecordFirstTerm::RecordFirstTerm(int _proID, string _left) {
	proID = _proID;
	left = _left;
	dependent = "";
	firstSet.clear();
}

FirstTerm::FirstTerm() {
	left = "";
	firstSet.clear();
}

map<pair<char, char>, int> OPGUtil::getFGTable(map<pair<char, char>, char>& table, Grammar& g) {
	map<pair<char, char>, int> fgTable;
	// Ԥ�����ҵ����е�<,>,=��ϵ������;���Ⱥ������ʼ��ȫΪ1
	map<pair<char, char>, char> lessTable;
	map<pair<char, char>, char> greaterTable;
	map<pair<char, char>, char> equalTable;
	initForFGTable(g, table, fgTable, lessTable, greaterTable, equalTable);
	// ��������
	bool isLoop = true;
	int cnt = 0;
	while (isLoop) {
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


vector<FirstTerm> OPGUtil::getFirstTerm(Grammar& g) {
	// �̺���Ϣ��
	vector<RecordFirstTerm> vrft;
	vector<FirstTerm> vft;
	map<string, int> sig;
	// �����̺���Ϣ��
	generateRecordFirstTerm(g.productions, vrft);
	initMap(g.productions, sig);
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
		vft.insert(vft.end(), newVft.begin(), newVft.end());
		// �ڶ������������FIRSTTERM�ķ��ս���滻��vrf��
		replaceDependent(vrft, vft, sig);
	}
	cout << "���ս����FIRSTTERM+Ϊ��\n";
	printFirstTerm(vft);

	return vft;
}


vector<LastTerm> OPGUtil::getLastTerm(Grammar& g) {
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

map<pair<char, char>, char> OPGUtil::getTable(Grammar& g, vector<FirstTerm>& first, vector<LastTerm>& last) {
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
				//cout << "��" << j << "�ε�������" << endl;
			}
		}
	}
	// ���Ͼ�ĩ�������ڹ�ϵ#E#
	int index1 = findLeft(first, g.productions[0].left);
	int index2 = findLeft(last, g.productions[0].left);
	for (string s : first[index1].firstSet) {
		table.insert({ { '#',s[0]},'<' });
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
map<pair<char, char>, char> OPGUtil::getTableNo(Grammar& g, vector<FirstTerm>& first, vector<LastTerm>& last) {
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
void OPGUtil::printFGTable(Grammar& g, map<pair<char, char>, int>& fgTable) {
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
void OPGUtil::initForFGTable(Grammar& g, map<pair<char, char>, char>& table, map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& lessTable, map<pair<char, char>, char>& greaterTable, map<pair<char, char>, char>& equalTable) {
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
bool OPGUtil::getRelation(map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& relaTable) {
	bool isChange = false;
	// ѭ�����й�ϵ  c1  c2  rela
	for (pair<pair<char, char>, char> p : relaTable) {
		if (p.second == '<') {// ʵ�� f(c1) < g(c2)
			if (fgTable[{'f', p.first.first}] >= fgTable[{'g', p.first.second}]) {
				fgTable[{'g', p.first.second}] = fgTable[{'f', p.first.first}] + 1;
				isChange = true;
			}
		}
		else if (p.second == '>') {// ʵ�� f(c1) > g(c2)
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

int OPGUtil::findLeft(vector<FirstTerm>& vft, string left) {
	for (int i = 0; i < vft.size(); i++) {
		if (vft[i].left == left) {
			return i;
		}
	}
	return -1;
}
void OPGUtil::generateRecordLastTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft) {
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
						rft.firstSet.insert(rights[start - 1]);
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
void OPGUtil::printFirstTerm(vector<FirstTerm>& vft) {
	cout << "symbol\tFirstTermSet" << endl;
	for (int i = 0; i < vft.size(); i++) {
		cout << vft[i].left << "\t";
		for (set<string>::iterator it = vft[i].firstSet.begin(); it != vft[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
void OPGUtil::replaceDependent(vector<RecordFirstTerm>& vrft, vector<FirstTerm>& vft, map<string, int>& sig) {
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
vector<FirstTerm> OPGUtil::moveNoDependent(vector<RecordFirstTerm>& vrft) {
	vector<FirstTerm> vft;
	int left = 0, right = 0;
	int pos = 0;
	pair<int, int> p = findNoDependent(vrft, pos);
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
pair<int, int> OPGUtil::findNoDependent(vector<RecordFirstTerm>& vrft, int pos) {
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
void OPGUtil::initMap(vector<Production>& vp, map<string, int>& sig) {
	sig.clear();
	for (int i = 0; i < vp.size(); i++) {
		sig.insert({ vp[i].left , -1 });
	}
}
/// <summary>
/// �����̺���¼��
/// </summary>
/// <param name="vp"></param>
/// <param name="vrft"></param>
void OPGUtil::generateRecordFirstTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft) {
	int proID = 0;
	// ѭ��ÿ�����ս���������ʽ
	for (int i = 0; i < vp.size(); i++) {
		// ��
		string left = vp[i].left;
		// ѭ��ÿ������ʽ,��ȡ���ս����FIRSTTERM�ͷ��ս������ս����FIRSTTERM�̺���ϵ
		for (int j = 0; j < vp[i].right.size(); j++) {
			RecordFirstTerm rft(proID++, left);
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
void OPGUtil::printRecordFirstTerm(vector<RecordFirstTerm>& vrft) {
	cout << "proID\tleft\tdependent\tfirstSet" << endl;
	for (int i = 0; i < vrft.size(); i++) {
		cout << vrft[i].proID << "\t" << vrft[i].left << "\t" << vrft[i].dependent << "\t\t";
		for (set<string>::iterator it = vrft[i].firstSet.begin(); it != vrft[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}

bool OPGUtil::isJuzi(string start, string juzi, map<pair<char, char>, char>& table){
	// �ս��ջ�ͱȽϹ�ϵջ���ȽϹ�ϵջ��i����ʾ�Ĺ�ϵ���ս��ջ��i���͵�i+1�����ŵĹ�ϵ�����i+1�ޣ����Ǵ�ʶ����ӵ��ս��
	vector<char> sym;
	vector<char> op;
	string t;
	// ��ʼ��״̬
	juzi = juzi + "#";
	sym.push_back('#');
	int currentIndex = 0;// ��������ʶ��ķ��ŵ��±�
	int firstTerm = 0;// ջ�е�һ���ս�����±�
	int cnt = 0;
	cout << "����ջ\t\t��ʶ��ľ���\t\tƥ��Ĺ���\n";
	while (1) {
		cout << endl << "ѭ������: " << cnt++ << endl;
		for (int i = 0; i < sym.size(); i++) {
			cout << sym[i];
		}
		cout << "\t\t";
		for (int i = currentIndex; i < juzi.size(); i++) {
			cout << juzi[i];
		}
		cout << "\t\t";
		


		map<pair<char, char>, char>::iterator it = table.find({ sym[firstTerm],juzi[currentIndex] });
		if (it != table.end()) {// ���д��ڹ�ϵ
			char s1 = it->first.first;
			char s2 = it->first.second;
			char r = it->second;
			if (r == '<' || r == '=') {// ������ʶ��ķ��źͱȽϹ�ϵ����ջ��
				sym.push_back(s2);
				op.push_back(r);
				// ����ջ�е�һ���ս�����±�
				string t;
				if (PUTIL::isNon( s2+t)) {// ���Ƿ��ս��
					firstTerm = sym.size() - 2;
				}
				else {
					firstTerm = sym.size() - 1;
				}
				currentIndex++;

				cout << s1 << r << s2;
			}
			else if (it->second == '>') {
				string pattern;
				while (firstTerm < sym.size()) {// ����һ���ս������֮��ķ��ż���
					pattern = pattern + sym.back();
					sym.pop_back();
				}
				firstTerm = sym.size() - 2;
				t = "";
				while (op.back() == '=' || PUTIL::isNon(t+sym.back())) {
					pattern = pattern + sym.back();
					sym.pop_back();
					op.pop_back();
				}
				char newNon = start[0];
				sym.push_back(newNon);
				cout << pattern << "->" << newNon;
				firstTerm = sym.size() - 2;
			}
			else {// �����
				cout << "���ڲ�����<=>��ϵ:" << sym[firstTerm] << "->" << juzi[currentIndex] << endl;
				return false;
			}
		}
		else {
			cout << "�����ڹ�ϵ:" << sym[firstTerm] << "->" << juzi[currentIndex] << endl;
			return false;
			return false;
		}
		if ( currentIndex == juzi.size()-1 && sym.size() == 2 ) {
			cout << endl << "��������" << endl;
			t = "";
			t += sym[1];
			if (t != start) {
				cout << "start: " << start << " " << t << endl;
				return false;
			}
			break;
		}
	}
	
	cout << endl;
	return true;
}
/// <summary>
/// �����������Ⱦ����Ե����Ϸ����Ƿ�Ϊ����
/// </summary>
/// <param name="start"></param>
/// <param name="juzi"></param>
/// <param name="table"></param>
/// <param name="proToLeftTable"></param>
/// <returns></returns>
ERRORTYPE OPGUtil::isSentence(string start, string juzi, map<pair<char, char>, char>& table, map<string, char>& proToLeftTable)
{
	FILE* fp;
//D:\Code\CCode\VS\QtProject\OPG\x64\Debug
	//fopen_s(&fp,"..\\QtProject\\OPG\\x64\\Debug\\toQt.txt", "w");
	fopen_s(&fp,"toQt.txt", "w");
	start = "N";
	// ��ʼ��
	ERRORTYPE error = ISSENTENCE;
	vector<char> symbols;// ����ջ
	vector<char> relation;// ��ϵջ��relation[i]��ʾ����ջsymbols[i] <=> symbol[i+1]
	int firstTerm = 0;// ����ջ�е�һ���ս�����±�
	int current = 0;// ����ʶ��ľ��ӵķ��ŵ��±�
	juzi = juzi + "#";
	symbols.push_back('#');
	int cnt = 0;
	cout << "����\tջ\t��ϵ\t��һ������\t�������벿��\t�����ʶ���\n";
	fprintf(fp, "����\tջ\t��ϵ\t��һ������\t�������벿��\t�����ʶ���\n");
	while (1) {
		fprintf(fp,"%d\t",cnt);
		cout << cnt++ << "\t";
		for (int i = 0; i < symbols.size(); i++) {
			cout << symbols[i];
			fprintf(fp, "%c",symbols[i]);
		}
		cout << "\t";
		fprintf(fp, "\t");
		// ���ҷ���ջ�ĵ�һ���ս���뵱ǰ�ַ��Ĺ�ϵ
		map<pair<char, char>, char>::iterator it = table.find({ symbols[firstTerm],juzi[current] });
		if (it != table.end()) {
			cout << it->second << "\t" << juzi[current] << "\t\t";
			fprintf(fp, "%c\t%c\t\t",it->second, juzi[current]);
			if (current == juzi.size() - 1) {
				fprintf(fp, "$");
			}
			for (int i = current + 1; i < juzi.size(); i++) {
				cout << juzi[i];
				fprintf(fp, "%c",juzi[i]);
			}
			cout << "\t\t";
			fprintf(fp, "\t\t");
			// ���� <=
			if (it->second == '<' || it->second == '=') {
				symbols.push_back(it->first.second);
				relation.push_back(it->second);
				firstTerm = symbols.size() - 1;
				current++;
				fprintf(fp,"$");
			}
			else if (it->second == '>') {
				// �ҵ������ʶ���
				string phase = getLeftmostPrimePhrase(symbols, relation);
				// ���ʶ���ɹ�ԼΪ�ķ��ս���������ջ
				char newNon = 'N';
				pair<string, char> p = stringToPro(proToLeftTable, phase);
				//char newNon = p.second;
				symbols.push_back(newNon);
				cout << phase;
				fprintf(fp, "%s",phase.c_str());
				// �ҵ�����ջ�ĵ�һ���ս��,�����ܳ������������ķ��ս���������һ���������ս��
				firstTerm = symbols.size() - 2;
			}
			else {// ��������ϵ
				error = NONEXISTENTRELATIONSHIP;
				break;
			}
			cout << endl;
			fprintf(fp, "\n");
		}
		else {// �ҵ������ڵĹ�ϵ
			error = NONEXISTENTRELATIONSHIP;
			break;
		}
		// ʶ�����
		if (juzi[current]=='#' && symbols.size() == 2) {// ����ջ��Ϊ #N �� ����ʶ��#
			if (PUTIL::isNon(symbols[1])) {
				break;
			}
		
		}
	}
	fprintf(fp, "%d\t",cnt);
	cout << cnt++ << "\t";
	for (char c : symbols){
		cout << c;
		fprintf(fp, "%c",c);
	}
	cout << "\t\t#\t\t\t\t\n";
	fprintf(fp, "\t$\t#\t\t$\t\t$\n");
	if (start[0] != symbols[1]) {
		error = NOTSTARTCHAR;
	}
	if (error == ISSENTENCE) {
		fprintf(fp,"��һ������!");
	}
	else {
		fprintf(fp, "����һ������!");
	}
	fclose(fp);
	return error;
}




void OPGUtil::printOPGTable(Grammar& g, map<pair<char, char>, char>& table) {
	for (char c1 : g.terms) {
		cout << "\t" << c1;
	}
	cout << endl;
	for (char c1 : g.terms) {
		cout << c1 << "\t";
		for (char c2 : g.terms) {
			pair<char, char> p = { c1,c2 };
			if (table.find(p) != table.end()) {
				cout << table[p] << "\t";
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
}

string OPGUtil::getLeftmostPrimePhrase(vector<char>& symbols, vector<char>& relation) {
	string pattern;
	// ����ջ���ϵջ�У�n1 < n2 = n3 = n4
	// �� n1 < n2 = n3 = n4, ���ֵĵ�һ�� < ������ַ�����
	while (relation.size() > 0) {
		if (PUTIL::isNon(symbols.back())) {// ���ս��ֱ�Ӽ���
			pattern = symbols.back() + pattern;
			symbols.pop_back();
		}
		else {// �ս���жϹ�ϵ
			if (relation.back() == '=') {
				pattern = symbols.back() + pattern;
				symbols.pop_back();
				relation.pop_back();
			}
			else if (relation.back() == '<') {
				pattern = symbols.back() + pattern;
				symbols.pop_back();
				relation.pop_back();
				break;
			}
		}
	}
	while (PUTIL::isNon(symbols.back())) {
		pattern = symbols.back() + pattern;
		symbols.pop_back();
	}
	return pattern;
}

ERRORTYPE OPGUtil::isSentencePlus(Grammar& g, string start, string juzi, map<pair<char, char>, char>& table, map<string, char>& proToLeftTable)
{

	//start = "N";
	// ��ʼ��
	ERRORTYPE error = ISSENTENCE;
	vector<char> symbols;// ����ջ
	vector<char> relation;// ��ϵջ��relation[i]��ʾ����ջsymbols[i] <=> symbol[i+1]
	int firstTerm = 0;// ����ջ�е�һ���ս�����±�
	int current = 0;// ����ʶ��ľ��ӵķ��ŵ��±�
	juzi = juzi + "#";
	symbols.push_back('#');
	int cnt = 0;
	cout << "����\tջ\t��ϵ\t��һ������\t�������벿��\t�����ʶ���\n";
	while (1) {
		cout << cnt++ << "\t";
		for (int i = 0; i < symbols.size(); i++) {
			cout << symbols[i];
		}
		cout << "\t";
		// ���ҷ���ջ�ĵ�һ���ս���뵱ǰ�ַ��Ĺ�ϵ
		map<pair<char, char>, char>::iterator it = table.find({ symbols[firstTerm],juzi[current] });
		if (it != table.end()) {
			cout << it->second << "\t" << juzi[current] << "\t\t";
			for (int i = current + 1; i < juzi.size(); i++) {
				cout << juzi[i];
			}
			cout << "\t\t";

			// ���� <=
			if (it->second == '<' || it->second == '=') {
				symbols.push_back(it->first.second);
				relation.push_back(it->second);
				firstTerm = symbols.size() - 1;
				current++;
			}
			else if (it->second == '>') {
				// �ҵ������ʶ���

				string phase = getLeftmostPrimePhrase(symbols, relation);
				// ���ʶ���ɹ�ԼΪ�ķ��ս���������ջ
				//char newNon = 'N';
				pair<string, char> p = stringToPro(proToLeftTable, phase);
				char newNon = p.second;
				symbols.push_back(newNon);
				cout << phase;
				// �ҵ�����ջ�ĵ�һ���ս��,�����ܳ������������ķ��ս���������һ���������ս��
				firstTerm = symbols.size() - 2;
			}
			else {// ��������ϵ
				error = NONEXISTENTRELATIONSHIP;
				break;
			}
			cout << endl;
		}
		else {// �ҵ������ڵĹ�ϵ
			error = NONEXISTENTRELATIONSHIP;
			break;
		}
		// ʶ�����
		if (juzi[current] == '#' && symbols.size() == 2) {// ����ջ��Ϊ #N �� ����ʶ��#
			break;
		}
	}
	cout << cnt << "\t";
	for (char c : symbols) {
		cout << c;
	}
	cout << "\t\t#\n";
	if (start[0] != symbols[1]) {
		error = NOTSTARTCHAR;
	}

	return error;
}


map<string, char> OPGUtil::getProToLeftTable(Grammar& g) {
	map<string, char> table;
	for (int i = 0; i < g.productions.size(); i++) {
		string left = g.productions[i].left;
		for (int j = 0; j < g.productions[i].right.size(); j++) {
			if (includeTerm(g.productions[i].right[j])) {
				table.insert({ g.productions[i].right[j] ,left[0] });
			}
		}
	}
	return table;
}


void OPGUtil::warShall(map<pair<char, char>, char>& table, Grammar& g) {
	// ��ʼ��2n*2n�ľ���
	vector<vector<bool>> m(g.terms.size() * 2, vector<bool>(g.terms.size() * 2, false));
	map<char, int> termIndex;// ���ս��������ӳ��
	initMatrix(table, g, m, termIndex);
	cout << "��ʼ���ľ���\n";
	printMatrix(m, g.terms);
	/*cout << "�ֶ������ս����˳��ľ���\n";
	vector<char> terms = { '+','*','(',')','i'};
	printMatrix1();*/
	solveWarshall(m, termIndex);
	int n = g.terms.size() * 2;
	for (int i = 0; i < n; i++) {
		m[i][i] = true;
	}
	cout << "����ľ���\n";
	printMatrix(m, g.terms);
	set<char>::iterator it = g.terms.begin();
	bool b = true;
	cout << endl << endl;
	for (char c : g.terms) {
		cout << "\t" << c;
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		if (b && i < n / 2) {
			cout << "f: ";
			b = false;
		}
		else if (!b && i == n / 2) {
			cout << "g: ";
			b = false;
		}
		int cnt = 0;
		for (int j = 0; j < n; j++) {
			if (m[i][j]) {
				cnt++;
			}
		}
		cout << "\t" << cnt;
		it++;
		if (i == (n / 2) - 1) {
			b = true;

			it = g.terms.begin();
			cout << endl;
			cout << "g: ";
		}
	}
	cout << endl;
}
void OPGUtil::solveWarshall(vector<vector<bool>>& m, map<char, int>& termIndex) {
	int n = termIndex.size() * 2;
	for (int col = 0; col < n; col++) {
		for (int row = 0; row < n; row++) {
			if (m[row][col] == true) {
				for (int k = 0; k < n; k++) {
					m[row][k] = m[col][k] + m[row][k];
				}
			}
		}
	}
}

void OPGUtil::printMatrix(vector<vector<bool>>& m, set<char>& terms) {
	for (char c : terms) {
		cout << "\t" << c;
	}
	cout << "\t|";
	for (char c : terms) {
		cout << "\t" << c;
	}
	cout << endl;
	int i = 0, j = 0;
	for (char c1 : terms) {
		cout << c1 << "\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << "|\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << endl;
		i++;
		j = 0;
	}
	for (char c2 : terms) {
		cout << "--------------------";
	}
	cout << endl;
	for (char c1 : terms) {
		cout << c1 << "\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << "|\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << endl;
		i++;
		j = 0;
	}
}

void OPGUtil::initMatrix(map<pair<char, char>, char>& table, Grammar& g, vector<vector<bool>>& m, map<char, int>& termIndex) {
	int index = 0;
	int n = g.terms.size();
	// �ս��������
	cout << "�ս��: ";
	for (char c : g.terms) {
		termIndex.insert({ c,index++ });
		cout << c << " ";
	}
	cout << endl;
	int cnt = 0;
	for (pair<pair<char, char>, char> p : table) {
		//cout << p.first.first << " " << p.first.second << " " << p.second << endl;
		// ��ȡ t1 > t2 ��t1,t2�ھ����е��±�
		int index1 = termIndex[p.first.first];
		int index2 = termIndex[p.first.second];
		if (p.second == '>') {
			m[index1][index2 + n] = true;
			//cout << index1 << " " << index2 + n << endl;
		}
		else if (p.second == '=') {
			m[index1][index2 + n] = true;
			//m[index1+n][index2-n] = true;// ��ֵʱת��
			m[index2 + n][index1] = true;

			//cout << index1 << " " << index2 + n << endl;
			//cout << index2+n << " " << index1 << endl;
		}
		else {
			//m[index1 + n][index2 - n] = true;// ��ֵʱת��
			m[index2 + n][index1] = true;
			//cout << index2 + n << " " << index1 << endl;
		}
	}
}
bool OPGUtil::includeTerm(string pro) {
	for (int i = 0; i < pro.size(); i++) {
		if (pro[i] >= 'a' && pro[i] <= 'z') {
			return true;
		}
	}
	false;
}
pair<string, char> OPGUtil::stringToPro(map<string, char>& table, string regular) {
	for (pair<string, char> p : table) {
		if (p.first.size() == regular.size()) {
			bool isReturn = true;
			for (int i = 0; i < regular.size(); i++) {
				if ((PUTIL::isNon(p.first[i]) && PUTIL::isNon(regular[i])) || p.first[i] == regular[i]) {

				}
				else {
					isReturn = false;
					break;
				}
			}
			if (isReturn) {
				return p;
			}
		}
	}
	return { "",0 };
}