#include "FirstUtil.h"


/// @brief ���FIRST�Ĺ�����

vector<SymbolFirst>::iterator FirstUtil::findRecordFirstByLeft(string left, vector<SymbolFirst>& vsf) {
	vector<SymbolFirst>::iterator it = vsf.begin();
	while (it != vsf.end()) {
		if (it->left == left) {
			break;
		}
		it++;
	}
	return it;
}
/// @brief �����ս����FIRST�Ͳ���ʽ��FIRST
/// @param vp �ķ������в���ʽ
/// @param vrf ����ʽ�̺���Ϣ��
/// @param vsf ���ս��FIRST
/// @param vpf ����ʽFIRST
void FirstUtil::getFirst(vector<Production>& vp, vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf) {
	//��ʼ��
	map<string, int> sig;// ����������FIRST�ķ��ս��
	generateRecordFirst(vp, vrf);// ���ɲ���ʽ�̺���Ϣ��
	// ѭ����������һ�����ҳ�����ֱ�������FIRST�ķ��ս�����ڶ���������һ������Ĵ��뵽Ϊ�����dependent�ġ�
	int cnt = 0;
	initMap(vp, sig);
	cout << "FIRST �̺���¼��\n";
	printRecordFirst(vrf);
	while (!vrf.empty()) {
		/*cout << "��" << cnt++ << "��\n";
		printRecordFirst(vrf);*/
		// ��һ��,ɾ����ֱ�����FIRST�ķ��ս���������뵽vsf��vpf��
		pair< vector<SymbolFirst>, vector<ProductionFirst> > p = moveNoDependent(vrf);
		int start = vsf.size();// ��������FIRST�ķ��ս������������vsf�е�λ��
		for (int i = 0; i < p.first.size(); i++) {
			sig[p.first[i].left] = start + i;
		}
		vsf.insert(vsf.end(), p.first.begin(), p.first.end());
	/*	for (int i = 0; i < p.second.size(); i++) {
			int index = findProductionFirst(vpf, p.second[i].proID);
			if (index != -1) {
				vpf[index].firstSet.insert(p.second[i].firstSet.begin(), p.second[i].firstSet.end());
			}
			else {
				vpf.push_back(p.second[i]);
			}
		}*/
		vpf.insert(vpf.end(), p.second.begin(), p.second.end());
		// �ڶ������������FIRST�ķ��ս���滻��vrf��
		replaceDependent(vrf, vsf, sig);
		// ���������������ѭ���̺��������ǵĵ�
	}
	cout << "���ս����FIRSTΪ��\n";
	printSymbolFirst(vsf);
}
/// <summary>
/// ��ӡSymbolFirst��
/// </summary>
/// <param name="vsf"></param>
void FirstUtil::printSymbolFirst(vector<SymbolFirst>& vsf) {
	cout << "symbol\tFirstSet" << endl;
	for (int i = 0; i < vsf.size(); i++) {
		cout << vsf[i].left << "\t";
		for (set<string>::iterator it = vsf[i].firstSet.begin(); it != vsf[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
/// <summary>
/// ��ӡProductionFirst��
/// </summary>
/// <param name="vpf"></param>
void FirstUtil::printGrammarFirst(vector<ProductionFirst>& vpf) {
	cout << "proID\tleft\tFirstSET" << endl;
	for (int i = 0; i < vpf.size(); i++) {
		cout << vpf[i].proID << "\t" << vpf[i].left << "\t";
		for (set<string>::iterator it = vpf[i].firstSet.begin(); it != vpf[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
/// @brief ���ķ���Ϣ��ȡ��RecordFirst����
/// @param vp �ķ������в���ʽ
/// @param vr ����ʽ�̺���Ϣ��¼��
void FirstUtil::generateRecordFirst(vector<Production>& vp, vector<RecordFirst>& vrf) {
	int proID = 0;// ����ʽID,��vp˳����ʲ���
	for (int i = 0; i < vp.size(); i++) {
		string left = vp[i].left;
		for (int j = 0; j < vp[i].right.size(); j++) {// ѭ��ÿ������ʽ
			RecordFirst rf(proID++);// ��ʼ��һ����¼
			rf.left = left;
			string right = vp[i].right[j];
			// �ҵ���һ�����ս�����ս��
			string t = "";
			t += right[0];
			if (right[0] >= 'A' && right[0] <= 'Z') {// ��ȡ���ս��  E' 
				if (right[1] == DYH) {
					t += right[1];
				}
				rf.dependent = t;
			}
			else {// �ս������FirstSet
				rf.firstSet.insert(t);
			}
			vrf.push_back(rf);
		}
		if (vp[i].have_null == 1) {// �������ʽ�������¼
			vrf.push_back(RecordFirst(proID++, left));
		}
	}
}

/// @brief �ӿ�ʼposλ�ÿ�ʼ���ҵ���һ������ֱ�����FIRST�ķ��ս����������������
/// @param vrf ����ʽ�̺���Ϣ��
/// @param pos ��ʼλ��
/// @return �ҵ����±�����[left,right],left==-1˵��δ�ҵ�
pair<int, int> FirstUtil::findNoDependent(vector<RecordFirst>& vrf, int pos) {
	int left = pos, right = pos;
	bool is = true;
	while (right < vrf.size()) {
		if (vrf[left].left == vrf[right].left) {
			if (vrf[right].dependent != "") {// ��ǰ���ս�����ɣ�����
				while (vrf[right].left == vrf[left].left && right < vrf.size()) {
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
/// @brief ��vr�в������������ս���������FIRST��ɾ��
/// @param vr ����ʽ�̺���Ϣ��
/// @return ����vr�����п������SymbolFirst��ProductionFirst
pair< vector<SymbolFirst>, vector<ProductionFirst> > FirstUtil::moveNoDependent(vector<RecordFirst>& vrf) {
	vector<SymbolFirst> vsf;
	vector<ProductionFirst> vpf;
	int left = 0, right = 0;// [ left, right ) Ϊͬһ�����ս�������в���ʽ
	int pos = 0;
	pair<int, int> p = findNoDependent(vrf, pos);
	while (p.first < vrf.size()) {
		SymbolFirst sf;
		sf.left = vrf[p.first].left;
		for (int i = p.first; i < p.second; i++) {
			ProductionFirst pf;
			pf.left = sf.left;
			pf.proID = vrf[i].proID;
			pf.firstSet.insert(vrf[i].firstSet.begin(), vrf[i].firstSet.end());
			vpf.push_back(pf);
			sf.firstSet.insert(vrf[i].firstSet.begin(), vrf[i].firstSet.end());
		}
		while (p.second != p.first) {
			vrf.erase(vrf.begin() + p.first);
			p.second--;
		}
		vsf.push_back(sf);
		p = findNoDependent(vrf, p.first);
	}
	return 	make_pair(vsf, vpf);
}
/// @brief ��vr�г�����vs�е�dependentȥ�������滻��Ӧ�ķ��ս����vr��firstSet��
/// @param vrf ����ʽ�̺���Ϣ��
/// @param vsf ���ս����FIRST
/// @param sig ��Ǳ���¼���з��ս����������vs�е��±꣬δ����Ϊ-1
void FirstUtil::replaceDependent(vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, map<string, int>& sig) {
	for (int i = 0; i < vrf.size(); i++) {
		if (vrf[i].dependent != "") {// �ҵ����ܱ��滻��
			int pos = sig[vrf[i].dependent];
			if (pos >= 0) {//�����滻
				vrf[i].dependent = "";
				vrf[i].firstSet.insert(vsf[pos].firstSet.begin(), vsf[pos].firstSet.end());
			}
		}
	}
}
void FirstUtil::initMap(vector<Production>& vp, map<string, int>& sig) {
	sig.clear();
	for (int i = 0; i < vp.size(); i++) {
		sig.insert(make_pair(vp[i].left, -1));
	}
}
void FirstUtil::printMap(vector<Production>& vp, map<string, int>& sig) {
	for (int i = 0; i < vp.size(); i++) {
		cout << vp[i].left << "\t" << sig[vp[i].left] << endl;
	}
}
void FirstUtil::printRecordFirst(vector<RecordFirst>& vrf) {
	cout << "proID\tleft\tdependent\tfirstSet" << endl;
	for (int i = 0; i < vrf.size(); i++) {
		cout << vrf[i].proID << "\t" << vrf[i].left << "\t" << vrf[i].dependent << "\t\t";
		for (set<string>::iterator it = vrf[i].firstSet.begin(); it != vrf[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
