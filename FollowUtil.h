#pragma once
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<stack>
#include<string>
#include"Grammar.h"
#include"FOLLOW.h"
#include"FIRST.h"
#include"FirstUtil.h"

using namespace std;

/// @brief ���FOLLOW�Ĺ�����
class FollowUtil {
public:
	/// @brief �����ս���� FOLLOW
	/// @param vp �ķ��Ĳ���ʽ
	/// @param vsf ���ս����FIRST
	/// @param vpf ĳ������ʽ��FIRST
	/// @return 
	static vector<SymbolFollow> getFollow(vector<Production>& vp, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf);
	static void printSymbolFollow(vector<SymbolFollow>& vsfo);
private:
	static void replaceDependent(vector<RecordFollow>& vrfo, vector<SymbolFollow>& vsfo, map<string, int>& sig);
	static void initMap(vector<Production>& vp, map<string, int>& sig);
	/// @brief �ӿ�ʼposλ�ÿ�ʼ���ҵ���һ������ֱ�����FIRST�ķ��ս����������������
		/// @param vrf ����ʽ�̺���Ϣ��
		/// @param pos ��ʼλ��
		/// @return �ҵ����±�����[left,right],left==-1˵��δ�ҵ�
	static pair<int, int> findNoDependent(vector<RecordFollow>& vrfo, int pos);
	/// @brief ��vr�в������������ս���������FIRST��ɾ��
	/// @param vr ����ʽ�̺���Ϣ��
	/// @return ����vr�����п������SymbolFirst��ProductionFirst
	static vector<SymbolFollow> moveNoDependent(vector<RecordFollow>& vrfo);
	/// @brief ͨ��vsf��vp�������RecordFollow��Ϣ��
	/// @param vp �ķ������в���ʽ
	/// @param vsf ���ս����FIRST
	/// @return ����RecordFollow��
	static vector<RecordFollow> getRecordFollow(vector<Production>& vp, vector<SymbolFirst>& vsf);
	/// @brief ɾ���ظ����� ���� ���̺���
	/// @param vrfo 
	static void eraseRepeatFollow(vector<RecordFollow>& vrfo);
	/// @brief ���һ������ʽ left=pro ��RecordFollow�̺���Ϣ
	/// @param vrfo 
	/// @param vsf 
	/// @param left 
	/// @param pro 
	static void getFollowFromProduction(vector<RecordFollow>& vrfo, vector<SymbolFirst>& vsf, string left, string pro);
	/// @brief �ж�str�Ƿ�Ϊ���ս��
	/// @param str 
	/// @return ���ս������true
	static bool isNon(string str);
	/// @brief ������ʽ�ִ��ֽ�Ϊ�ַ�(���ս�����ս��)����
	/// @param pro 
	/// @return 
	static vector<string> getRightsFromPro(string pro);
	static void printRecordFollow(vector<RecordFollow>& vrfo);
	static void printGraph();

	static map< string, int > visisted;
	static map<string, string> parent;
	static vector<string> circle;
	static set<string> vertex;
	static map<string, set<string> > g;

	/// <summary>
	/// �ݹ�˼·���ֽ���ǰ�����Ϊ���ڷ��ʣ������ɵ������һ����㣬
	/// �жϸýڵ��Ƿ����ڱ����ʣ����ڱ�����˵�����ֻ���
	/// �������нڵ��δ���ֻ����򽫸ý����Ϊ�ѷ��ʡ�
	/// ����������ܱ�������·��������Ϊ2������������ʱ
	/// ���ض����н�㣬���в�ѯ���Ĳ�����
	/// </summary>
	/// <param name="ver"></param>
	/// <param name="g"></param>
	/// <returns></returns>
	static bool DFS(string ver, map<string, set<string> >& g) {
		// ����Ϊ���ڷ���
		visisted[ver] = 1;
		// �����ýڵ���Ե���Ľ��
		for (string next : g[ver]) {
			if (visisted[next] == 0) {// �ýڵ�δ����������������������
				// ��·����next�ĸ��ڵ���ver
				parent[next] = ver;
				// ����ý��ı����ҵ��˻�
				if (DFS(next, g)) {
					visisted[ver] = 2;
					return true;
				}
				// Ϊ�ҵ������������һ��ѭ��������������Ƿ�����ҵ���
			}
			else if (visisted[next] == 1) {// �ýڵ����ڱ����ʣ��ҵ�һ����
				//�ҵ�һ��next [-> ... ->] next �Ļ�
				string cur = ver;
				circle.push_back(next);
				// �� ����[]֮��Ľ�����
				while (cur != next) {
					circle.push_back(cur);
					cur = parent[cur];
				}
				//������ʼ���
				circle.push_back(next);
				reverse(circle.begin(), circle.end());
				visisted[ver] = 2;
				return true;
			}
		}
		// ����Ϊ�ѷ���
		visisted[ver] = 2;
		return false;
	};
	static void initDFS(vector<RecordFollow>& vrfo) {
		// ���������
		visisted.clear();
		vertex.clear();
		parent.clear();
		circle.clear();
		g.clear();
		//��ʼ��ͼ�ͽ��
		for (RecordFollow rf : vrfo) {
			if (rf.dependent != "") {// �����̺���ϵ
				g[rf.left].insert(rf.dependent);
				vertex.insert(rf.left);
				vertex.insert(rf.dependent);
			}
		}
		// ��ʼ�����ʱ�
		for (string s : vertex) {
			visisted.insert({ s,0 });
		}
	}
	/// <summary>
	/// �����ҵ������л�
	/// </summary>
	/// <returns></returns>
	static vector<vector<string> > getCircles(vector<RecordFollow>& vrfo);
	static void printCircles(vector<vector<string> >& circles) {
		for (int i = 0; i < circles.size(); i++) {
			cout << i << "\t";
			for (string s : circles[i]) {
				cout << s << " ";
			}
			cout << endl;
		}
	}
	/// <summary>
	/// ����ת���������д洢
	/// </summary>
	/// <param name="circles"></param>
	/// <returns></returns>
	static vector<set<string> > circlesVecToSet( vector<vector<string> >& circles ) {
		vector<set<string> > newCircles;
		for (int i = 0; i < circles.size(); i++) {
			set<string> cir;
			for (string s : circles[i]) {
				cir.insert(s);
			}
			newCircles.push_back(cir);
		}
		return newCircles;
	}
	static vector<int> findSelfDependent(vector<RecordFollow>& vrfo, vector<set<string>>& circlesSet) {
		vector<int> vi;
		for (int i = 0; i < circlesSet.size(); i++ ) {
			bool isSelf = false;// Ĭ��Ϊfalse��������ѭ��δ�ߣ���˵���޸÷��ţ�
			for (int j = 0; j < vrfo.size(); j++) {
				// ��������Ϊ�գ����ǻ��ڷ�
				if (vrfo[j].dependent != "" && circlesSet[i].find(vrfo[j].left) != circlesSet[i].end()) {
					// ���ڷ������������ڻ���
					if (circlesSet[i].find(vrfo[j].dependent) == circlesSet[i].end()) {
						isSelf = false;
						break;
					}
					else {// 
						isSelf = true;
					}
				}
			}
			if (isSelf) {
				vi.push_back(i);
			}
		}
		return vi;
	}
	static set<string> getCircleDependentAndEraseCircle(set<string >& circleSet, vector<SymbolFollow>& vsfo,vector<RecordFollow>& vrfo) {
		set<string> followSet;
		for (int i = vrfo.size() - 1; i >= 0; i--) {
			// �ҵ����ڷ�
			if (circleSet.find(vrfo[i].left) != circleSet.end()) {
				if (vrfo[i].dependent == "") {// ��followSet
					followSet.insert(vrfo[i].followSet.begin(), vrfo[i].followSet.end());
				}
				vrfo.erase(i + vrfo.begin());
			}
		}
		return followSet;
	}
	static void addCircleToSymbolFollowTable(set<string>& circleSet, vector<SymbolFollow>& vsfo, set<string>& followSet) {
		for (string item : circleSet) {
			SymbolFollow sfo(item, followSet);
			vsfo.push_back(sfo);
		}
	}
	static void eraseCircle(vector<RecordFollow>& vrfo,vector<set<string>>& circlesSet,vector<SymbolFollow>& vsfo) {
		// ���һ��ڷ�ֻ�������ڷ��Ļ�
		vector<int> selfDependent = findSelfDependent(vrfo, circlesSet);
		cout << "���ڷ�ֻ�������ڷ�����: ";
		for (int i = 0; i < selfDependent.size(); i++) {
			for (string item : circlesSet[selfDependent[i]]) {
				cout << item << " ";
			}
		}
		cout << endl;
		for (int index : selfDependent) {
			// ��ȡ�û���followSet,����vrfo�е����е�left�ǻ��ڷ���ɾ��
			set<string> newFollowSet = getCircleDependentAndEraseCircle(circlesSet[index], vsfo, vrfo);
			cout << "��ɾ���Ļ� : ";
			for (string item : circlesSet[index]) {
				cout << item << " ";
			}
			cout << endl;
			cout << "��øû�followSet: ";
			for (string item : newFollowSet) {
				cout << item << " ";
			}
			cout << endl;
			// �����ڷ���followSet���뵽SymbolFollow����
			addCircleToSymbolFollowTable(circlesSet[index], vsfo, newFollowSet);
			// ���÷��ű��У��̺��÷��ŵ�followSet����
			for (int i = 0; i < vrfo.size(); i++) {
				if (circlesSet[index].find(vrfo[i].dependent) != circlesSet[index].end()) {
					vrfo[i].dependent = "";
					vrfo[i] .followSet = newFollowSet;
				}
			}
		}
	}
	static int findRecordFollowBySymbolAndDependent( vector<RecordFollow>& vrfo, string symbol, string dependent ) {
		for (int i = 0; i < vrfo.size(); i++) {
			if (vrfo[i].left == symbol && vrfo[i].dependent == dependent) {
				return i;
			}
		}
		return -1;
	}
};
