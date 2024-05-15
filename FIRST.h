#pragma once
#include<string>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;

/// @brief ��¼һ������ʽ����Ϣ
class RecordFirst {
public:
	int proID;// ����ʽID
	string left;// ����ʽ��
	string dependent;// ���̺��ķ��ս��
	set<string> firstSet;// �������ս��
	RecordFirst();
	RecordFirst(int _proID);
	/// @brief ����һ�������ʽ�ļ�¼
	/// @param _proID 
	/// @param emp 
	RecordFirst(int _proID, string _left);
};

/// @brief ���ս����FIRSTֵ
class SymbolFirst {
public:
	string left;
	set<string> firstSet;
	SymbolFirst();
};


/// @brief һ������ʽ��FIRST
class ProductionFirst {
public:
	int proID;
	string left;
	set<string> firstSet;
	ProductionFirst();
};