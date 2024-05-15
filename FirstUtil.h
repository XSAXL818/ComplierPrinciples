#pragma once
#include<string>
#include<map>
#include<vector>
#include<iostream>
#include<algorithm>
#include"Grammar.h"
#include"FIRST.h"

using namespace std;

/// @brief ���FIRST�Ĺ�����
class FirstUtil {
public:
	static vector<SymbolFirst>::iterator findRecordFirstByLeft(string left, vector<SymbolFirst>& vsf);
	/// @brief �����ս����FIRST�Ͳ���ʽ��FIRST
	/// @param vp �ķ������в���ʽ
	/// @param vrf ����ʽ�̺���Ϣ��
	/// @param vsf ���ս��FIRST
	/// @param vpf ����ʽFIRST
	static void getFirst(vector<Production>& vp, vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf);
	static void printSymbolFirst(vector<SymbolFirst>& vsf);
	static void printGrammarFirst(vector<ProductionFirst>& vpf);
private:
	/// @brief ���ķ���Ϣ��ȡ��RecordFirst����
	/// @param vp �ķ������в���ʽ
	/// @param vr ����ʽ�̺���Ϣ��¼��
	static void generateRecordFirst(vector<Production>& vp, vector<RecordFirst>& vrf);
	/// @brief �ӿ�ʼposλ�ÿ�ʼ���ҵ���һ������ֱ�����FIRST�ķ��ս����������������
	/// @param vrf ����ʽ�̺���Ϣ��
	/// @param pos ��ʼλ��
	/// @return �ҵ����±�����[left,right],left==-1˵��δ�ҵ�
	static pair<int, int> findNoDependent(vector<RecordFirst>& vrf, int pos);
	/// @brief ��vr�в������������ս���������FIRST��ɾ��
	/// @param vr ����ʽ�̺���Ϣ��
	/// @return ����vr�����п������SymbolFirst��ProductionFirst
	static pair< vector<SymbolFirst>, vector<ProductionFirst> > moveNoDependent(vector<RecordFirst>& vrf);
	/// @brief ��vr�г�����vs�е�dependentȥ�������滻��Ӧ�ķ��ս����vr��firstSet��
	/// @param vrf ����ʽ�̺���Ϣ��
	/// @param vsf ���ս����FIRST
	/// @param sig ��Ǳ���¼���з��ս����������vs�е��±꣬δ����Ϊ-1
	static void replaceDependent(vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, map<string, int>& sig);
	static void initMap(vector<Production>& vp, map<string, int>& sig);
	static void printMap(vector<Production>& vp, map<string, int>& sig);
	static void printRecordFirst(vector<RecordFirst>& vrf);
	static int findProductionFirst(vector<ProductionFirst>& vpf,int proID) {
		for (int i = 0; i < vpf.size(); i++) {
			if (vpf[i].proID == proID) {
				return i;
			}
		}
		return -1;

	}
};