#include "FAUtil.h"

void FAUtil::readDFA( vector<DFA>& dfas, string filePath ) {
	char line[200];
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "r");
	if (fp == 0) {
		cout << "�ļ���ʧ��!" << endl;
		return;
	}
	fgets(line, sizeof(line), fp);// ��ȡ��һ�е�˵���ı�
	while (fgets(line, sizeof(line), fp)) {
		set<string> statusSet = set<string>();
		set<char> inputLetter = set<char>();
		string start = "";
		map<DFAKey, string> mapTable = map<DFAKey, string>();
		set<string> finalSet = set<string>();

		string str = line;
		if (str.find_last_of('\n') != -1) {// ȥ�����з� 
			str.erase(--str.end());
		}
		// ��ȡDFA�ı�
		if (allNum(str)) {//ȫ������
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				string ver1 = "";
				char edge = str[2];
				string ver2 = "";
				ver1 += str[0];
				//edge += str[2]
				ver2 += str[4];
				// ״̬����
				statusSet.insert(ver1);
				statusSet.insert(ver2);
				// ӳ�伯��
				DFAKey in(ver1, edge);
				mapTable.insert(make_pair(in, ver2));
				// �����ַ�����
				inputLetter.insert(edge);
			}
		}
		//��ȡ��ʼ״̬
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// ȥ�����з� 
			str.erase(--str.end());
		}
		if (allNum(str)) {//ȫ������
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// ȥ�����з� 
					str.erase(--str.end());
				}
				start = str;
			}
		}
		//��ȡ��ֹ״̬
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// ȥ�����з� 
			str.erase(--str.end());
		}
		if (allNum(str)) {//ȫ������
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// ȥ�����з� 
					str.erase(--str.end());
				}
				finalSet.insert(str);
			}
		}
		DFA dfa( statusSet, inputLetter, mapTable, start, finalSet );
		dfas.push_back(dfa);
	}
	fclose(fp);
	cout << filePath << "�ļ���ȡ����" << endl;
}


void FAUtil::printMatDFA(DFA& dfa) {
	for (set<char>::iterator it = dfa.inputLetter.begin(); it != dfa.inputLetter.end(); it++) {
		cout << "\t|" << *it;
	}
	cout << endl;
	for (set<string>::iterator it1 = dfa.statusSet.begin(); it1 != dfa.statusSet.end(); it1++) {
		cout << "________________________________________________________________" << endl;
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = dfa.inputLetter.begin(); it2 != dfa.inputLetter.end(); it2++) {
			DFAKey in(*it1, *it2);
			if (dfa.mapTable.find( in )!= dfa.mapTable.end()) {
				cout << dfa.mapTable[in];
			}
			else {
				cout << "*";
			}
			cout << "\t|";
		}
		cout << endl;
	}
}

void DFA::printDFA() {
	cout << "״̬����{";
	for (set<string>::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "״̬ת�ƾ����ӡ��" << endl;
	FAUtil::printMatDFA(*this);
	cout << "ӳ�伯(" << mapTable.size() <<  "):" << endl;
	for (map<DFAKey, string>::iterator it = this->mapTable.begin(); it != this->mapTable.end(); it++) {
		cout << it->first.ver << " " << it->first.edge << " " << it->second << endl;
	}
	cout << "�������ַ�(" << inputLetter.size() << "): {";
	for (set<char>::iterator it = this->inputLetter.begin(); it != this->inputLetter.end(); it++) {
		cout << *it << ",";
	}
	cout <<"}" << endl;
	cout << "��ʼ״̬Ϊ��" << this->start << endl;
	cout << "��ֹ״̬Ϊ(" << finalSet.size() <<  ")��{";
	for (set<string>::iterator it = this->finalSet.begin(); it != this->finalSet.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
};

void DFA::printMatDFA() {
	for (set<char>::iterator it = this->inputLetter.begin(); it != this->inputLetter.end(); it++) {
		cout << "\t|" << *it;
	}
	cout << endl;
	for (set<string>::iterator it1 = this->statusSet.begin(); it1 != this->statusSet.end(); it1++) {
		cout << "________________________________________________________________" << endl;
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = this->inputLetter.begin(); it2 != this->inputLetter.end(); it2++) {
			DFAKey in(*it1, *it2);
			if (this->mapTable.find(in) != this->mapTable.end()) {
				cout << this->mapTable[in];
			}
			else {
				cout << "*";
			}
			cout << "\t|";
		}
		cout << endl;
	}

}

void FAUtil::printDFAs(vector<DFA>& dfas) {
	for (int i = 0; i < dfas.size(); i++) {
		cout << "��" << i << "��DFA: " << endl;
		dfas[i].printDFA();
	}
	cout << endl;
}

bool FAUtil::allNum(string str) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] < '0' || str[i]>'9') {
			return false;
		}
	}
	return true;
}
int FAUtil::stringToNum(string str) {
	int num = 0;
	for (int i = 0; i < str.size(); i++) {
		num *= 10;
		num += str[i] - '0';
	}
	return num;
}

void FAUtil::DFADerivedSentence(DFA& dfa,string str) {
	string head = dfa.start;
	DFAKey in;
	cout << "M(" << head << "," << str << ") = " << endl;
	while (str.size() != 0) {// ѭ������ÿһ���ַ�
		char inputChar = str[0];
		in = DFAKey(head, inputChar);
		str.erase(0, 1);
		if (str.size() != 0) {
			cout << "\t=M(" << "M(" << in.ver << "," << in.edge << ")," << str << ")" << endl;
		}
		if ( dfa.mapTable.find(in) == dfa.mapTable.end() ) {
			cout << "δƥ�䵽ӳ�䣡�ⲻ��һ������" << endl;
			return;
		}
		else {
			head = dfa.mapTable[in];// ��ȡ M(A,a) = B
		}
	}
	cout << "\t=M(" << in.ver << "," << in.edge << ")" << endl << "\t=";
	if (dfa.finalSet.find(head) != dfa.finalSet.end()) {
		cout << head << endl;
		cout << "����һ�����ӣ�\n\n";
	}
	else {
		cout << head << endl << "\t�Ƶ����������ֹ̬������һ�����ӣ�" << endl;
	}
}


void FAUtil::readNFA(vector<NFA>& nfas, string filePath) {
	char line[200];
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "r");
	if (fp == 0) {
		cout << "�ļ���ʧ��!" << endl;
		return;
	}
	fgets(line, sizeof(line), fp);// ��ȡ��һ�е�˵���ı�
	while (fgets(line, sizeof(line), fp)) {
		set<string> statusSet;//״̬����
		set<char> inputLetter;//�����ַ�����
		map< DFAKey, set<string> > mapTable;//ӳ�伯��
		set<string> start;//��ʼ״̬����
		set<string> finalSet;//��ֹ״̬����

		string str = line;
		if (str.find_last_of('\n') != -1) {// ȥ�����з� 
			str.erase(--str.end());
		}
		// ��ȡDFA�ı�
		if (allNum(str)) {//ȫ������
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				string ver1 = "";
				char edge = str[2];
				string ver2 = "";
				ver1 += str[0];
				//edge += str[2]
				ver2 += str[4];
				// ״̬����
				statusSet.insert(ver1);
				statusSet.insert(ver2);
				// ӳ�伯��
				DFAKey in(ver1, edge);
				if (mapTable.find(in) != mapTable.end() ) {
					mapTable[in].insert(ver2);
				}
				else {
					mapTable.insert(make_pair(in, set<string>{ver2}));
				}
				// �����ַ�����
				inputLetter.insert(edge);
			}
		}
		//��ȡ��ʼ״̬
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// ȥ�����з� 
			str.erase(--str.end());
		}
		if (allNum(str)) {//ȫ������
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// ȥ�����з� 
					str.erase(--str.end());
				}
				start.insert(str);
			}
		}
		//��ȡ��ֹ״̬
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// ȥ�����з� 
			str.erase(--str.end());
		}
		if (allNum(str)) {//ȫ������
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// ȥ�����з� 
					str.erase(--str.end());
				}
				finalSet.insert(str);
			}
		}
		NFA nfa(statusSet, inputLetter, mapTable, start, finalSet);
		nfas.push_back(nfa);
	}
	fclose(fp);
	cout << filePath << "�ļ���ȡ����" << endl;
}

void FAUtil::printMatNFA(NFA& nfa) {
	for (set<char>::iterator it = nfa.inputLetter.begin(); it != nfa.inputLetter.end(); it++) {
		cout << "\t|\t" << *it;
	}
	cout << endl;
	for (set<string>::iterator it1 = nfa.statusSet.begin(); it1 != nfa.statusSet.end(); it1++) {
		cout << "________________________________________________________________" << endl;
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = nfa.inputLetter.begin(); it2 != nfa.inputLetter.end(); it2++) {
			DFAKey in(*it1, *it2);
			if (nfa.mapTable.find(in) != nfa.mapTable.end()) {
				for (set<string>::iterator sit = nfa.mapTable[in].begin(); sit != nfa.mapTable[in].end(); sit++) {
					cout << *sit << ",";
				}
			}
			else {
				cout << "*";
			}
			cout << "\t\t|";
		}
		cout << endl;
	}
}
void FAUtil::printNFAs(vector<NFA>& nfas) {
	for (int i = 0; i < nfas.size(); i++) {
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<��" << i << "��NFA: >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		nfas[i].printNFA();
	}
	cout << endl;
}

void NFA::printNFA() {
	cout << "״̬ת�ƾ����ӡ��" << endl;
	FAUtil::printMatNFA(*this);
	cout << "ӳ�伯(" << mapTable.size() << "):" << endl;
	for (map<DFAKey, set<string> >::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		cout << it->first.ver << " " << it->first.edge << " {";
		for (set<string>::iterator itt = it->second.begin(); itt != it->second.end(); itt++) {
			cout << *itt << ",";
		}
		cout << "}" << endl;
	}
	cout << "�������ַ�(" << inputLetter.size() << "): {";
	for (set<char>::iterator it = inputLetter.begin(); it != inputLetter.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
	cout << "��ʼ״̬Ϊ(" << start.size() << ")��{";
	for (set<string>::iterator it = start.begin(); it != start.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
	cout << "��ֹ״̬Ϊ(" << finalSet.size() << ")��{";
	for (set<string>::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
}

/// <summary>
/// ��ʼ��@_closure��¼�̺���
/// ����<A,@>=B��A_closure += B_closure + { A } ,������<A,@>��ӳ�䣬��A_closure={ A }
/// </summary>
/// <param name="vrc"></param>
void NFA::initRecordClosure(vector<RecordClosure>& vrc) {
	for (set<string>::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		DFAKey key(*it, CHAR_NULL);// <ver,@>
		set<string> dependent = set<string>{};// �̺��ķ���
		set<string> closure = set<string>{};// �հ�
		if (mapTable.find(key) != mapTable.end()) {// ����<ver,@>��ӳ�䣬����ӳ��
			for (set<string>::iterator sit = mapTable[key].begin(); sit != mapTable[key].end(); sit++) {
				dependent.insert(*sit);
			}
		}
		closure.insert(*it);//���������հ�
		vrc.push_back(RecordClosure(*it, dependent, closure));

	}
}
void NFA::printRecordClosure(vector<RecordClosure>& vrc) {
	cout << "״̬��@_closure���£�" << endl;
	cout << "symbol\tdependent\tclosure\n";
	for (int i = 0; i < vrc.size(); i++) {
		cout << vrc[i].symbol << "\t|";
		for (set<string>::iterator it = vrc[i].dependent.begin(); it != vrc[i].dependent.end(); it++) {
			cout << *it << " ";
		}
		if (vrc[i].dependent.size() == 0) {
			cout << "*";
		}
		cout << "\t\t|";
		for (set<string>::iterator it = vrc[i].closure.begin(); it != vrc[i].closure.end(); it++) {
			cout << *it << " ";
		}
		cout << "\t\t|" << endl;
	}
}
void NFA::initNullClosure(vector<RecordClosure>& vrc) {
	while (vrc.size()) {
		eraseNullDependent(vrc);
		replaceDependent(vrc);
	}
}


void NFA::initClosure() {
	//@_closure�̺���
	vector<RecordClosure> vrc;
	// ��ʼ��@_closure�̺���
	initRecordClosure(vrc);
	cout << "@_closure�̺������£�" << endl;
	printRecordClosure(vrc);
	// ��ʼ��@_closure
	cout << "@_closure�����£�" << endl;
	initNullClosure(vrc);
	printNullClosure();
	inputLetter.erase(CHAR_NULL);
	//��ʼ��letter_closure�̺���:��״̬A�� a_closure = <A,a>������ӳ���״̬��@_closure
	initLetterClosure();
	cout << "letter_closure�����£�" << endl;
	printLetterClosure();
}

DFA FAUtil::NFAToDFA(NFA& nfa) {
	NFA old = nfa;
	// δ��ʼ��״̬�ıհ�
	cout << "<<<<<<<<<<<<<<<<<<<<<<<�հ�����" << endl<<endl;
	if (nfa.nullClosure.empty()||nfa.letterClosure.empty()) {
		nfa.initClosure();
		// �������ÿ�ʼ�����ϣ�����ԭ���ϵ�@_closure�Ĳ�
		set<string> newStart;
		for (set<string>::iterator it = nfa.start.begin(); it != nfa.start.end(); it++) {
			newStart.insert(nfa.nullClosure[*it].begin(), nfa.nullClosure[*it].end());
		}
		nfa.start = newStart;
		cout << "��ʼ���ıհ�Ϊ��{";
		for (set<string>::iterator it = nfa.start.begin(); it != nfa.start.end(); it++) {
			cout << *it << ",";
		}
		cout << "}" << endl;
	}
	string file = "NFAOutput.txt";
	cout << "NFA�����" << file << "��" << endl;
	nfa.fwriteNFA(file);
	cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<NFA��ȷ����<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n" << endl;
	//	NFA�ȼ۹���DFA,ת��Ϊ�м�����ʾ������ת��
	NFAtoDFA ntd = equivalentReplacement( nfa );
	// ��ӡ�м�״̬���
	cout << "NFAת��ΪDFA���м�״̬���£�" << endl;
	ntd.printNFAtoDFA();
	DFA dfa = ntd.toDFA();
	cout << "\n<<<<<<<<<<<<<<<<<<���±�ź�<<<<<<<<<<<<<<<<<<<<<<" << endl;
	dfa.printDFA();
	file = "NoSimplificationDFA.txt";
	cout << "δ�����ļ�������"<<file<< "��"<<endl;
	dfa.fwriteDFA(file.c_str());
	// DFA�Ļ���
	cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<DFA�Ļ���<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	dfa.simplification();
	nfa = old;
	return dfa;
}

void DFA::simplification() {
	// ��һ�λ���Ϊ��ֹ״̬��ͷ���ֹ״̬��
	set<string> s1;//����ֹ״̬��
	set_difference(statusSet.begin(), statusSet.end(), finalSet.begin(), finalSet.end(), inserter(s1, s1.begin()));
	// ��ʼ��ջ�����ڱ����ѻ������map,ջ���Ա��ڷ������״̬����
	stack< set<string> > st;
	map< set<string>, int > m;//���ڱ��
	set< set<string> > allGroup;//����ÿ���׶ε����з���
	st.push(finalSet);
	st.push(s1);
	allGroup.insert(finalSet);
	allGroup.insert(s1);
	int id = 0;
	int loopTimes = 0;
	cout << "��ʼδ���ֵ���Ϊ��" << endl;
	FAUtil::printGroupStack(st);

	while (!st.empty()) {
		cout << "***************************��" << loopTimes++ << "�ε���*******************************" << endl;
		// ȡ��һ������
		set<string> group = st.top();
		st.pop();
		cout << "���л��ֵ���Ϊ: ";
		for (set<string>::iterator sit = group.begin(); sit != group.end(); sit++) {
			cout << *sit;
		}
		cout << endl;
		//����ֻ��һ��Ԫ��,�ѻ��ֺ�
		if (group.size() == 1) {
			cout << "����ֻ��һ��Ԫ�أ����ɻ���" << endl;
			allGroup.insert(group);
			m.insert(make_pair(group, id++));
		}
		else {
			// �������飬��ǰÿ�λ���2�飬���ڿ��滻������ʵ��ÿ�λ���N��
			vector< set<string> > groups = divideTwoGroup(group, allGroup);
			if (groups.size() == 1) {// δ����,����ǰ�鲻�ɻ���
				cout << "��ǰ�鲻�ɻ���" << endl;
				m.insert(make_pair(group, id++));// ֱ�Ӽ���
			}
			else {// �ɻ���,���¼���ջ�У��ȴ��´λ���
				cout << "��ǰ��ɻ��֣��»��ֵ���Ϊ���£���СΪ" << groups.size() << endl;
				// ���з��鼯�ϣ�ɾ����ǰ���飬�����·���
				allGroup.erase(group);
				for (vector< set<string> >::iterator it = groups.begin(); it != groups.end(); it++) {
					st.push(*it);
					allGroup.insert(*it);
				}
				for (vector< set<string> >::iterator it = groups.begin(); it != groups.end(); it++) {
					for (set<string>::iterator sit = it->begin(); sit != it->end(); sit++) {
						cout << *sit;
					}
					cout << " ";
				}
				cout << endl;
			}
		}
		
		 cout << "δ���ֵ���: " << endl;
		 FAUtil::printGroupStack(st);
		 cout << "�ѻ��ֵ���:" << endl;
		 for (map< set<string>, int >::iterator it = m.begin(); it != m.end(); it++) {
			 cout << "{";
			 for (set<string>::iterator sit = it->first.begin(); sit != it->first.end(); sit++) {
				 cout << *sit << ",";
			 }
			 cout <<"}" << endl;
		 }
	}
	id = 0;
	cout << "���ֺ�����з���:" << endl;
	for (set< set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
		m[*it] = id++;
	}
	cout << "״̬\t\tID" << endl;
	for (map< set<string>, int >::iterator it = m.begin(); it != m.end(); it++) {
		cout << "{";
		for (set<string>::iterator sit = it->first.begin(); sit != it->first.end(); sit++) {
			cout << *sit << ",";
		}
		cout << "}\t\t" << it->second << endl;
	}
	// ���±��,����DFA
	map<DFAKey, string> newMapTable;
	// �����ַ�
	for (set<char>::iterator it1 = inputLetter.begin(); it1 != inputLetter.end(); it1++) {
		// ���������ɵ�״̬��
		for (set< set<string> >::iterator it2 = allGroup.begin(); it2 != allGroup.end(); it2++) {
			set<string> newStatusSet;
			// ��ȡ��״̬����ת����״̬�ļ���
			for (set<string>::iterator it3 = it2->begin(); it3 != it2->end(); it3++) {
				map<DFAKey, string>::iterator ret = mapTable.find({ *it3,*it1 });
				if (ret != mapTable.end()) {
					newStatusSet.insert(ret->second);
				}
			}
			if (!newStatusSet.empty()) {// ��ȡ��״̬���ϲ�Ϊ�գ����п����ս���������ַ����򲻿���
				DFAKey key( to_string(m[*it2]), *it1);
				string newState;
				for (set<set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
					if (FAUtil::isInclude(*it, newStatusSet)) {
						newState = to_string(m[*it]);
					}
				}
				newMapTable.insert({ key, newState });
			}
		}
	}
	this->mapTable = newMapTable;
	//���»�ȡ��ʼ��,�ս�����ϣ�״̬����
	bool isFind = false;
	set<string> newFinalSet;
	set<string> newStatusSet;
	for (set<set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
		if (!isFind) {//��ʼ��ֻ��һ��
			if (FAUtil::isInclude(*it, set<string>{start})) {
				start = to_string(m[*it]);
			}
		}
		for (set<string>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
			if (finalSet.find(*it2 )!= finalSet.end()) {//��״̬���ϰ�����ֹ״̬
				newFinalSet.insert(to_string(m[*it]));
			}
		}
		newStatusSet.insert(to_string(m[*it]));
	}
	finalSet = newFinalSet;
	statusSet = newStatusSet;
	cout << "***************************�򻯺��DFA���£�**********************" << endl;
	printDFA();
}
void DFA::fwriteDFA(string filePath) {
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "w");
	if (fp == 0) {
		cout << "�ļ���ʧ��!" << endl;
		return;
	}
	fprintf(fp, "%d\n", (int)mapTable.size());
	for (map<DFAKey, string>::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		fprintf(fp, "%s %c %s\n",it->first.ver.c_str(), it->first.edge, it->second.c_str());
	}
	fprintf(fp, "%s\n", start.c_str());
	fprintf(fp, "%d\n", (int)finalSet.size());
	for (set<string>::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		fprintf(fp, "%s\n", it->c_str());
	}
	fclose(fp);
}
/// <summary>
/// �ж�s1�Ƿ����s2,����s2-s1�Ĳ�Ƿ�Ϊ��
/// </summary>
/// <param name="s1"></param>
/// <param name="s2"></param>
/// <returns></returns>
bool FAUtil::isInclude(set<string> s1, set<string> s2) {
	set<string> s3;
	set_difference(s2.begin(), s2.end(), s1.begin(), s1.end(), inserter(s3, s3.begin()));
	if (s3.size() == 0) {
		return true;
	}
	return false;

}
void FAUtil::printGroupStack(stack<set<string>> st){
	while (!st.empty()) {
		set<string> top = st.top();
		st.pop();
		cout << "{";
		for (set<string>::iterator it = top.begin(); it != top.end(); it++) {
			cout << *it << ",";
		}
		cout << "}" << endl;
	}
}

/// <summary>
/// ÿ�λ������飬ԭ���Ե�ǰ��Group1������״̬S1,S2������ <S1,a>=T1, <S2,a>=T2,��T1,T2״̬���ڲ�ͬ������
/// ��˵��S1,S2���ȼۣ������Խ����ֳ����飨��S1�ȼۣ���S1���ȼۣ�
/// ���T1������Group2��,�������滮��
/// ��1����S1�ȼۣ���Group1�е�����״̬����a��ת��Ϊ��״̬��Group2��
/// ��2����S1���ȼۣ���
/// </summary>
/// <param name="group"></param>
/// <returns>size==1,�򲻿ɷ֣�����ɷ�</returns>
vector< set<string> > DFA::divideTwoGroup(set<string> group, set< set<string> >& allGroup) {
	vector< set<string> > groups;// ���ص���
	bool isDiv = false;//�ʼĬ�ϲ��ɷ֣��������״̬���ϲ������κ��ַ�ʱ��״̬���ɷ�
	char c;
	set<string> divSet;// ����S1����
	for (set<char>::iterator cit = inputLetter.begin(); cit != inputLetter.end(); cit++) {
		// ��ȡ��ǰ������ĳ���ַ���ת����״̬�ļ���
		set<string> sslc = getDFAStatusSetLetterClosure(group, *cit);
		if (sslc.size() == 0) {// �����ڲ������ܽ����ַ���״̬,˵�����ɷ�
			isDiv = false;
			continue;
		}
		// �ж��Ƿ�ɷ֣�Ĭ�Ͽɷ֣�ֱ������һ�����е�״̬��������ǰ ת���õ���״̬�� ʱ�Ų��ɷ�
		isDiv = true;
		c = *cit;
		for (set< set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
			if (FAUtil::isInclude(*it, sslc)) {// ����һ�����飬������ǰ״̬,����ǰ״̬���ɷ�
				isDiv = false;
				break;
			}
		}
		cout << "�����ַ� " << c << " ��ת����״̬����Ϊ: {";
		for (set<string>::iterator sit = sslc.begin(); sit != sslc.end(); sit++) {
			cout << *sit << ",";
		}
		cout << "}" << endl;

		if (isDiv) {// ��ǰ�ַ�ת����״̬���Ͽɷ�
			string state = *sslc.begin();//Ĭ�ϵ�һ��״̬��S1����stateΪT1
			set<string> stateSet{ state };
			for (set< set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {// �ҵ�T1���ڵ���
				if (FAUtil::isInclude(*it, stateSet)) {
					divSet = *it;
					break;
				}
			}
			break;
		}
	}
	if (isDiv) {//�ɷ�
		set<string> newGroup1;// ���ֳ���ǰ�������״̬,��S1�ȼ۵�
		for (set<string>::iterator it = group.begin(); it != group.end(); it++) {
			if (divSet.find(mapTable[{*it, c}]) != divSet.end()) {//S1�ȼ�
				newGroup1.insert(*it);
			}
		}



		set<string> newGroup2;//��S1���ȼ�
		set_difference(group.begin(), group.end(), newGroup1.begin(), newGroup1.end(), inserter(newGroup2, newGroup2.begin()) );
		return vector< set<string> >{ newGroup1, newGroup2 };
	}
	else {
		return vector< set<string> >{group};
	}
}

set<string> DFA::getDFAStatusSetLetterClosure(set<string>& statusSet, char letter) {
	set<string> newStatusSet;
	DFAKey key;
	key.edge = letter;
	for (set<string>::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		key.ver = *it;
		map<DFAKey, string>::iterator ret = mapTable.find(key);
		if (ret != mapTable.end()) {
			newStatusSet.insert(ret->second);
		}
	}
	return newStatusSet;
}
DFA NFAtoDFA::toDFA( ) {
	DFA dfa;
	dfa.inputLetter = inputLetter;
	//��ʼ
	dfa.start = statusID[start];
	// ״̬
	for (set< NFASTATUS >::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		dfa.statusSet.insert(statusID[*it]);
	}
	//ӳ��
	for (map<NFAtoDFAKey, NFASTATUS>::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		DFAKey key(statusID[it->first.NFAStatus], it->first.input);
		dfa.mapTable.insert(make_pair(key, statusID[it->second]));
	}
	//��ֹ״̬
	for (set< NFASTATUS >::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		dfa.finalSet.insert(statusID[*it]);
	}
	return dfa;
}

NFAtoDFA FAUtil::equivalentReplacement(NFA& nfa) {
	// ��ʼ������,���ʱ�Ǳ�
	queue<NFASTATUS> q;
	set<NFASTATUS> vis;
	// ��ʼ��NFAtoDFA�Ŀ�ʼ״̬����״̬��
	int cnt = 0;
	NFAtoDFA ntd;
	ntd.start = nfa.start;
	ntd.statusSet.insert(nfa.start);
	ntd.statusID[nfa.start] = to_string(cnt++);
	ntd.inputLetter = nfa.inputLetter;
	ntd.inputLetter.erase(CHAR_NULL);
	ntd.stateID.push_back(nfa.start);
	q.push(nfa.start);
	vis.insert(nfa.start);
	// Ѱ���µ�״̬
	while (!q.empty()) {
		// ȡ��ĳ��״̬����
		NFASTATUS vertex = q.front();
		q.pop();
		// ������ǰ״̬�������� ���������ַ���< ״̬����, �����ַ� >��ӳ��,���õ�һ������
		// ���������ַ�
		for (set<char>::iterator cit = nfa.inputLetter.begin(); cit != nfa.inputLetter.end(); cit++) {
			set<string> newStatusSet = set<string>();
			// ��״̬������״̬ȡ������ȡ���е� <״̬, �����ַ�>��ӳ��,���õ�һ��ӳ���ϵ
			for (set<string>::iterator sit = vertex.begin(); sit != vertex.end(); sit++) {
				DFAKey key(*sit, *cit);
				newStatusSet.insert(nfa.letterClosure[key].begin(), nfa.letterClosure[key].end());
			}
			if (newStatusSet.size() == 0) {// ��״̬�����ַ��޷������µ�״̬����
				continue;
			}
			NFAtoDFAKey ntdkey(vertex, *cit);
			// �µ�<״̬����,�����ַ�>
			ntd.mapTable.insert(make_pair(ntdkey, newStatusSet));
			if (vis.find(newStatusSet) == vis.end()) {// ��״̬
				bool isFinal = false;
				// �ж��Ƿ����ֹ״̬
				for (set<string>::iterator fit = nfa.finalSet.begin(); fit != nfa.finalSet.end(); fit++) {
					if (newStatusSet.find(*fit) != newStatusSet.end()) {// ��״̬Ϊ��ֹ״̬
						isFinal = true;
						break;
					}
				}
				if (isFinal) {// ������ֹ״̬����
					ntd.finalSet.insert(newStatusSet);
				}
				//�������
				q.push(newStatusSet);
				//������ʱ��
				vis.insert(newStatusSet);
				//��״̬������
				ntd.statusID[newStatusSet] = to_string(cnt++);
				ntd.stateID.push_back(newStatusSet);
				//���뵽״̬�ļ���
				ntd.statusSet.insert(newStatusSet);
			}
		}
	}
	for (set<NFASTATUS>::iterator it = ntd.statusSet.begin(); it != ntd.statusSet.end(); it++) {
		bool isFinal = false;
		for (set<string>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
			for (set<string>::iterator it3 = nfa.finalSet.begin(); it3 != nfa.finalSet.end(); it3++) {
				if (*it2 == *it3) {
					isFinal = true;
					break;
				}
			}
			if (isFinal) {
				break;
			}
		}
		if (isFinal) {
			ntd.finalSet.insert(*it);
		}
	}
	return ntd;
}

void NFAtoDFA::renumber() {
	
}

void NFA::eraseNullDependent(vector<RecordClosure>&vrc) {
	for (int i = 0; i < vrc.size(); i++) {
		if (vrc[i].dependent.size() == 0) {//��ֱ������ձհ�
			nullClosure.insert(make_pair(vrc[i].symbol, vrc[i].closure));
			vrc.erase(vrc.begin() + i);
			i--;
		}
	}
};
void NFA::replaceDependent(vector<RecordClosure>& vrc) {
	for (int i = 0; i < vrc.size(); i++) {
		set<string> st = vrc[i].dependent;
		for (set<string>::iterator it = vrc[i].dependent.begin(); it != vrc[i].dependent.end(); it++) {
			map<string, set<string>>::iterator ret = nullClosure.find(*it);
			if (ret != nullClosure.end()) {//�滻
				vrc[i].closure.insert(ret->second.begin(), ret->second.end());
				st.erase(*it);
			}
		}
		vrc[i].dependent = st;
	}
}
void NFA::printNullClosure() {
	cout << "symbol\t@_closure\n";
	cout << "_______________________________________" << endl;
	for (map<string, set<string> >::iterator it = nullClosure.begin(); it != nullClosure.end(); it++) {
		cout << it->first << "\t|";
		for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); sit++) {
			cout << *sit << ",";
		}
		cout <<endl << "_______________________________________" << endl;
	}
};

void NFA::printLetterClosure() {
	for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
		cout << "\t|" << *it2;
	}
	cout << endl << "____________________________________" << endl;
	for (set<string>::iterator it1 = statusSet.begin(); it1 != statusSet.end(); it1++) {
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
			if (*it2 == CHAR_NULL) {
				continue;
			}
			map<DFAKey, set<string> >::iterator ret = letterClosure.find(DFAKey(*it1, *it2));
			if (ret != letterClosure.end()) {
				for (set<string>::iterator sit = ret->second.begin(); sit != ret->second.end(); sit++) {
					cout << *sit << ",";
				}
			}
			else {
				cout << "*";
			}
			cout << "\t|";
		}
		cout << endl << "____________________________________" << endl;
	}
}

void NFA::initLetterClosure() {
	int cnt = 0;
	for (map< DFAKey, set<string> >::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		letterClosure.insert(make_pair(it->first, set<string>()));
		for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); sit++) {
			letterClosure[it->first].insert(nullClosure[*sit].begin(), nullClosure[*sit].end());
		}
	}
}
void NFAtoDFA::printMapTable() {
	for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
		cout << "\t" << *it2 << "\t";
	}
	cout << endl << "___________________________________" << endl;
	for (vector<NFASTATUS>::iterator it1 = stateID.begin(); it1 != stateID.end(); it1++) {
		printNFAStatus(*it1);
		cout << "\t|";
		for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
			NFAtoDFAKey key(*it1, *it2);
			map<NFAtoDFAKey, NFASTATUS>::iterator ret = mapTable.find(key);
			if (ret != mapTable.end()) {
				printNFAStatus(ret->second);
			}
			else {
				cout << "*";
			}
			cout << "\t\t|";
		}
		cout << endl;
	}
}
void NFAtoDFA::printNFAtoDFA() {
	cout << "״̬����(" << statusSet.size()<< "): ";
	printStatusSet();

	for (set<NFASTATUS>::iterator sit = statusSet.begin(); sit != statusSet.end(); sit++) {
		if (*sit == NFASTATUS()) {
			cout << "���ڿ�״̬" << endl;
			break;
		}
	}

	cout << "��ʼ״̬(" << start.size() << "): ";
	printStart();
	cout << "��ֹ״̬����(" << finalSet.size()<< ")��";
	printFinalSet();
	cout << "ӳ�伯�ϣ�" << endl;
	printMapTable();
}
void NFAtoDFA::printStatusSet() {
	cout << "{";
	for (set<NFASTATUS>::iterator sit = statusSet.begin(); sit != statusSet.end(); sit++) {
		printNFAStatus(*sit);
		cout << ",";
	}
	cout << "}" << endl;
}
void NFA::fwriteNFA(string filePath){
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "w");
	if (fp == 0) {
		cout << "�ļ���ʧ��!" << endl;
		return;
	}
	int size = 0;
	for (map< DFAKey, set<string> >::iterator it1 = mapTable.begin(); it1 != mapTable.end(); it1++) {
		size += it1->second.size();
	}
	fprintf(fp, "%d\n", size);
	for (map< DFAKey, set<string> >::iterator it1 = mapTable.begin(); it1 != mapTable.end(); it1++) {
		for (set<string>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			fprintf(fp, "%s %c %s\n", it1->first.ver.c_str(), it1->first.edge, it2->c_str());
		}
	}
	fprintf(fp, "%d\n", (int)start.size());
	for (set<string>::iterator it = start.begin(); it != start.end(); it++) {
		fprintf(fp, "%s\n", it->c_str());
	}
	fprintf(fp, "%d\n", (int)finalSet.size());
	for (set<string>::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		fprintf(fp, "%s\n", it->c_str());
	}
	fclose(fp);
}
void NFAtoDFA::printStart() {
	cout << "{";
	for (NFASTATUS::iterator sit = start.begin(); sit != start.end(); sit++) {
		cout << *sit << ",";
	}
	cout << "}" << endl;
}
void NFA::eraseNullClosure() {
	for (set<string>::iterator it1 = statusSet.begin(); it1 != statusSet.end(); it1++) {
		bool isDel = true;
		for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
			DFAKey key(*it1, *it2);
			if (letterClosure.find(key) != letterClosure.end()) {
				isDel = false;
				break;
			}
		}
		if (isDel) {

		}
	}
}
void NFAtoDFA::printFinalSet() {
	cout << "{ ";
	for (set<NFASTATUS>::iterator fit = finalSet.begin(); fit != finalSet.end(); fit++) {
		printNFAStatus(*fit) ;
		cout << ",";
	}
	cout << "}" << endl;
}
void NFAtoDFA::printNFAStatus( const NFASTATUS& sta) {
	//״̬
	for (set<string>::iterator it = sta.begin(); it != sta.end(); it++) {
		cout << *it;
	}
}

NFA::NFA(set<string> _statusSet, set<char> _inputLetter, map< DFAKey, set<string> > _mapTable, set<string> _start, set<string> _finalSet) {
	statusSet = _statusSet;
	inputLetter = _inputLetter;
	mapTable = _mapTable;
	start = _start;
	finalSet = _finalSet;
	nullClosure.clear();
	letterClosure.clear();
}
DFA::DFA(set<string> _statusSet, set<char> _inputLetter, map<DFAKey, string> _mapTable, string _start, set<string> _finalSet) {
	statusSet = _statusSet;
	inputLetter = _inputLetter;
	mapTable.insert(_mapTable.begin(), _mapTable.end());
	start = _start;
	finalSet = _finalSet;
}

bool DFAKey::operator<(DFAKey in) const {
	if (ver == in.ver) {
		return edge < in.edge;
	}
	return ver < in.ver;
}
bool RecordClosure::operator<(RecordClosure rc) const {
	if (symbol != rc.symbol) {
		return symbol < rc.symbol;
	}
	else if (dependent != rc.dependent) {
		return dependent < rc.dependent;
	}
	return closure < rc.closure;
}
RecordClosure::RecordClosure(string _symbol, set<string> _dependent, set<string> _closure) {
	symbol = _symbol;
	dependent = _dependent;
	closure = _closure;
}



