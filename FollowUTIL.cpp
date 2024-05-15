#include "FollowUtil.h"


map<string, int> FollowUtil::visisted;
vector<string> FollowUtil::circle;
map<string, string> FollowUtil::parent;
set<string> FollowUtil::vertex;
map<string, set<string> > FollowUtil::g;

void FollowUtil::printGraph() {
	for (pair<string, set<string> > p : g) {
		cout << p.first << " : ";
		for (string s : p.second) {
			cout << s << ",";
		}
		cout << endl;
	}
}


/// @brief ���FOLLOW�Ĺ�����
/// @brief �����ս���� FOLLOW
/// @param vp �ķ��Ĳ���ʽ
/// @param vsf ���ս����FIRST
/// @param vpf ĳ������ʽ��FIRST
/// @return 
vector<SymbolFollow> FollowUtil::getFollow(vector<Production>& vp, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf) {
	// ��ȡRecordFollow��
	vector<RecordFollow> vrfo = getRecordFollow(vp, vsf);
	vector<SymbolFollow> vsfo;
	map<string, int> sig;// ����������FIRST�ķ��ս��
	initMap(vp, sig);
	// // ��ȡ����ֱ���Ƴ�FOLLOW�ķ��ս��,����vrfo��ɾ��
	// ѭ����������һ�����ҳ�����ֱ�������FIRST�ķ��ս�����ڶ���������һ������Ĵ��뵽Ϊ�����dependent�ġ�
	cout << "��ʼ����FOLLOW�̺�������" << endl;
	printRecordFollow(vrfo);
	int cnt = 0;
	// ��0�����ҵ��̺���ϵ�еĻ�,����ʾ���ǵ�FOLLOW��ͬ��������ȥ����Ȼ��������ϲ�
	vector<vector<string> > circles = getCircles(vrfo);
	vector<set<string> > circlesSet = circlesVecToSet(circles);
	sort(vrfo.begin(), vrfo.end());// ���򣬱��ڲ���ͬһ��left������
	cout << "�̺����г��ֵ����еĻ�: " << endl;
	printCircles(circles);
	// ɾ���ظ��ı���
	eraseRepeatFollow(vrfo);
	while (!vrfo.empty()) {
		// ��һ��,ɾ����ֱ�����Follow�ķ��ս���������뵽vsfo
		vector<SymbolFollow> ret = moveNoDependent(vrfo);
		int start = vsfo.size();// ��������FOLLOW�ķ��ս������������vsfo�е�λ��
		for (int i = 0; i < ret.size(); i++) {
			sig[ret[i].left] = start + i;
		}
		vsfo.insert(vsfo.end(), ret.begin(), ret.end());
		// �ڶ������������FIRST�ķ��ս���滻��vrf��
		replaceDependent(vrfo, vsfo, sig);

		// ���������ж��Ƿ���������,��������û���ͬʱ�û�����Ҫ��������Ԫ�ؼ������
		if (ret.empty()) {
			eraseCircle(vrfo,circlesSet,vsfo);
		}
		cout << "��" << cnt++ << "�����Follow" << endl;
		cout << "�̺���\n"; 
		printRecordFollow(vrfo);
		cout << "����FOLLOW��\n";
		printSymbolFollow(vsfo);
	}
	return vsfo;
}
vector<vector<string> > FollowUtil::getCircles(vector<RecordFollow>& vrfo) {
	initDFS(vrfo);
	vector<vector<string> > vvs;
	for (string v : vertex) {
		if (visisted[v] == 0) {//δ���ʣ���Ӹý���ѯ�Ƿ���·���ǻ�
			if (DFS(v, g)) {//���ڻ�
				//vector<vector<string>> vcir{circle};/*
				//set<string> circleSet = *circlesVecToSet(vcir).begin();*/
				// �������·�Ļ�
				/*bool is = false;
				for (int i = 0; i < circle.size() - 3; i++) {
					cout << 1 << endl;
					string head = circle[i];
					string medium = circle[i + 1];
					string tail = circle[i + 2];
					for (int j = 0; j < vrfo.size(); j++) {
						cout << 2 << endl;
						if (circleSet.find(vrfo[j].left)==circleSet.end()) {
							int index1 = findRecordFollowBySymbolAndDependent(vrfo, head, vrfo[j].left);
							int index2 = findRecordFollowBySymbolAndDependent(vrfo, vrfo[j].left, tail);
							circle.insert(circle.begin() + i, vrfo[j].left);
							is = true;
							break;
						}
					}
					if (is) {
						break;
					}
				}*/

				vvs.push_back(circle);
				circle.clear();
			}
		}
	}
	return vvs;
}
/// <summary>
/// ��ӡSymbolFollow��
/// </summary>
/// <param name="vsfo"></param>
void FollowUtil::printSymbolFollow(vector<SymbolFollow>& vsfo) {
	cout << "left\tfollowSet" << endl;
	for (int i = 0; i < vsfo.size(); i++) {
		cout << vsfo[i].left << "\t";
		for (set<string>::iterator it = vsfo[i].followSet.begin(); it != vsfo[i].followSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
/// <summary>
/// ���Ѿ����Follow�ķ��ս�������浽δ����ķ��ս����Dependent��
/// </summary>
/// <param name="vrfo">���ս�����̺���Ϣ��</param>
/// <param name="vsfo">�����Follow�ı�</param>
/// <param name="sig">��Ǳ�sig[���ս��]>=0����÷��ս�����������SymbolFollow���±�Ϊsig[Non]</param>
void FollowUtil::replaceDependent(vector<RecordFollow>& vrfo, vector<SymbolFollow>& vsfo, map<string, int>& sig) {
	for (int i = 0; i < vrfo.size(); i++) {
		if (vrfo[i].dependent != "") {// �ҵ����ܱ��滻��
			int pos = sig[vrfo[i].dependent];
			if (pos >= 0) {//�����滻
				vrfo[i].dependent = "";
				vrfo[i].followSet.insert(vsfo[pos].followSet.begin(), vsfo[pos].followSet.end());
			}
		}
	}
}
/// <summary>
/// ��ʼ����Ǳ�
/// </summary>
/// <param name="vp"></param>
/// <param name="sig"></param>
void FollowUtil::initMap(vector<Production>& vp, map<string, int>& sig) {
	sig.clear();
	for (int i = 0; i < vp.size(); i++) {
		sig.insert(make_pair(vp[i].left, -1));
	}
}
/// @brief �ӿ�ʼposλ�ÿ�ʼ���ҵ���һ������ֱ�����FIRST�ķ��ս����������������
	/// @param vrf ����ʽ�̺���Ϣ��
	/// @param pos ��ʼλ��
	/// @return �ҵ����±�����[left,right],left==-1˵��δ�ҵ�
pair<int, int> FollowUtil::findNoDependent(vector<RecordFollow>& vrfo, int pos) {
	int left = pos, right = pos;
	bool is = true;
	while (right < vrfo.size()) {
		if (vrfo[left].left == vrfo[right].left) {
			if (vrfo[right].dependent != "") {// ��ǰ���ս�����ɣ�����
				while (right < vrfo.size()) {
					if (vrfo[right].left != vrfo[left].left) {
						break;
					}
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
vector<SymbolFollow> FollowUtil::moveNoDependent(vector<RecordFollow>& vrfo) {
	vector<SymbolFollow> vsfo;
	int left = 0, right = 0;// [ left, right ) Ϊͬһ�����ս�������в���ʽ
	bool is = true;// ���ڱ�ǵ�ǰ���ս���Ƿ����ֱ�����
	int pos = 0;
	pair<int, int> p = findNoDependent(vrfo, pos);
	while (p.first < vrfo.size()) {
		SymbolFollow sfo;
		sfo.left = vrfo[p.first].left;
		for (int i = p.first; i < p.second; i++) {
			sfo.followSet.insert(vrfo[i].followSet.begin(), vrfo[i].followSet.end());
		}
		while (p.second != p.first) {
			vrfo.erase(vrfo.begin() + p.first);
			p.second--;
		}
		vsfo.push_back(sfo);
		p = findNoDependent(vrfo, p.first);
	}
	return 	vsfo;
}
/// @brief ͨ��vsf��vp�������RecordFollow��Ϣ��
/// @param vp �ķ������в���ʽ
/// @param vsf ���ս����FIRST
/// @return ����RecordFollow��
vector<RecordFollow> FollowUtil::getRecordFollow(vector<Production>& vp, vector<SymbolFirst>& vsf) {
	//FirstUtil::printSymbolFirst(vsf);
	vector<RecordFollow> vrfo;
	for (int i = 0; i < vp.size(); i++) {
		for (vector<string>::iterator sit = vp[i].right.begin(); sit != vp[i].right.end(); sit++) {// �������в���ʽ
			getFollowFromProduction(vrfo, vsf, vp[i].left, *sit);
		}
	}
	// Ϊ������������Ԥ����ɾ�������ظ������̺������������
	eraseRepeatFollow(vrfo);
	RecordFollow rf(vp[0].left);// Ϊ��ʼ�����ַ�������EOF��־
	string end = "";
	end += EOS;
	rf.followSet.insert(end);
	vrfo.push_back(rf);
	return vrfo;
}
/// @brief ɾ���ظ����� ���� ���̺���
/// @param vrfo 
void FollowUtil::eraseRepeatFollow(vector<RecordFollow>& vrfo) {
	sort(vrfo.begin(), vrfo.end());
	for (int i = 0; i < vrfo.size(); i++) {
		if (vrfo[i].left == vrfo[i].dependent) {// ɾ������
			vrfo.erase(vrfo.begin() + i);
			i--;
		}
		else if (i != vrfo.size() - 1) {
			if (vrfo[i] == vrfo[i + 1]) {
				vrfo.erase(vrfo.begin() + i + 1);
				i--;
			}
		}
	}
}
/// @brief ���һ������ʽ left=pro ��RecordFollow�̺���Ϣ
/// @param vrfo 
/// @param vsf 
/// @param left 
/// @param pro 
void FollowUtil::getFollowFromProduction(vector<RecordFollow>& vrfo, vector<SymbolFirst>& vsf, string left, string pro) {
	// ������ʽתΪ�ս���ͷ��ս��������
	vector<string> rights = getRightsFromPro(pro);
	int i = rights.size() - 2;// ѭ���� size-2��ʼ
	int j = i + 1;// ���һ���ַ�
	bool allNull = true;// ��¼ Yi+1 �� Yn-1 ȫ�������Ƴ������ʽ,���ڽ���1
	// ���ս�� Y �пղ���ʽ��ζ�� FIRST[Y]�������� ��@
	// X -> Y0 Y2 Y3 ... Yn-1:��X�Ƶ�ΪY0Y1Y2...Yn-1
	// ����1-1�� Yn�Ƿ��ս�� �� FOLLOW[Yn-1] += FOLLOW[X]
	// ���ͣ���XZ����ʱ����Ȼ���Ƶ�ΪY0Y1Y2...Yn-1 Z,������ Yn-1Q������Yn-1��FOLLOW����X��
	// ����1-2����Yi( 0 <= i < n ),���Yi+1 �� Yn ���������ʽ����FOLLOW[Yi] += FOLLOW[X]
	// ���ͣ����ڽ���1��֪����Yi+1��Yn����Ϊ�����ʽ���� X-> Y1 Y2 Y3 ... Yi
	// ����2-1�� ����Yi( 0 <= i < n-1 ),���Yi�Ƿ��ս������FOLLOW[Yi] += FIRST[Yi+1]-{ ��@ }
	// ���ͣ�Yi��FOLLOW�϶��ǰ�����һ���ַ�FIRST�����Yi+1���ս������FIRST[Yi+1] = { Yi+1 }
	// ����2-2����Yi( 0 <= i < i+1 < j < n ),��� Yi+1 �� Yj ȫΪ���ս����ȫ�������ʽ���� FOLLOW[Yi] += FIRST[j+1]-{ ��@ }
	// С�᣺
	// E = + T E'
	//      ����1��Yi [ Yi+1 ... Yn-1 ] ,���[]����Ԫ�أ���ȫΪ���ս���Һ������ʽ,   ��FOLLOW[Yi] += FOLLOW[X]
	//      ����2��Yi [ Yi+1 ... Yj-1 ] Yj,���[]��Ԫ�أ�����ȫΪ���ս���Һ������ʽ����FOLLOW[Yi] += FIRST[Yj]-{ ��@ }
	// ���н���2��Yj�����Ƿ��ս��
	// �����Ա���Yiʱ�����ж��Ƿ�Ϊ���ս�������ս�����ж�ʱ�Ƿ��������ʽ,�޲���ʽ�����1ʧЧ��2�����жϰ����ս����1ʧЧ��2�����ж�����
	if (isNon(rights[j])) {// ���һ��Ϊ���ս��������1�������������1-1
		RecordFollow rf(rights[j]);
		rf.dependent = left;
		vrfo.push_back(rf);
	}
	else {
		allNull = false;
	}
	// i=size-2
	while (i >= 0) {// ���������ַ�
		if (isNon(rights[i])) {// ���ս��
			// �������2
			RecordFollow rf(rights[i]);
			if (isNon(rights[j])) { // FOLLOW[Yi] += FIRST[Yj]-{ ��@ }
				vector<SymbolFirst>::iterator it = FirstUtil::findRecordFirstByLeft(rights[j], vsf);
				rf.followSet.insert(it->firstSet.begin(), it->firstSet.end());
				rf.followSet.erase("@");
			}
			else { // �ս����FIRST�����Լ�
				rf.followSet.insert(rights[j]);
			}
			vrfo.push_back(rf);
			// �жϵ�ǰ���ս���Ƿ��������ʽ
			vector<SymbolFirst>::iterator itt = FirstUtil::findRecordFirstByLeft(rights[i], vsf);
			if (itt->firstSet.find("@") == itt->firstSet.end()) {// �������ʽ
				j = i;// ����2 ����ı�
			}
			// �жϽ���1������,����ǰ���ս����� ���з��ս���Ƿ��������ʽ
			if (allNull) {
				itt = FirstUtil::findRecordFirstByLeft(rights[i + 1], vsf);
				if (itt->firstSet.find("@") == itt->firstSet.end()) {// �������ʽ
					allNull = false;
				}
			}
			// ����1�Ĵ���
			if (allNull) {// ��ǰ�ַ�������з��ս���������ʽ
				rf = RecordFollow(rights[i]);
				rf.dependent = left;
				vrfo.push_back(rf);
			}
		}
		else {
			allNull = false;// ����1ʧЧ
			j = i; // ����2 ������ �ַ�
		}
		i--;
	}
}
/// @brief �ж�str�Ƿ�Ϊ���ս��
/// @param str 
/// @return ���ս������true
bool FollowUtil::isNon(string str) {
	if (str[0] >= 'A' && str[0] <= 'Z') {
		return true;
	}
	return false;
}
/// @brief ������ʽ�ִ��ֽ�Ϊ�ַ�(���ս�����ս��)����
/// @param pro 
/// @return 
vector<string> FollowUtil::getRightsFromPro(string pro) {
	vector<string> rights;
	string t;
	for (int i = 0; i < pro.size(); i++) {
		t = "";
		t += pro[i];
		if (pro[i] >= 'A' && pro[i] <= 'Z') {// �ж��Ƿ�Ϊ E'
			if (i != pro.size() - 1) {// ���ַ��������һ���ַ�
				if (pro[i + 1] == DYH) {
					t += pro[i + 1];
					i++;
				}
			}
		}
		rights.push_back(t);
	}
	return rights;
}
/// <summary>
/// ��ӡRecordFollow��
/// </summary>
/// <param name="vrfo"></param>
void FollowUtil::printRecordFollow(vector<RecordFollow>& vrfo) {
	cout << "left\tdependent\tfollowSet" << endl;
	for (int i = 0; i < vrfo.size(); i++) {
		cout << vrfo[i].left << "\t" << vrfo[i].dependent << "\t\t";
		for (set<string>::iterator it = vrfo[i].followSet.begin(); it != vrfo[i].followSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}

