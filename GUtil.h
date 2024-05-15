#pragma once
#include<string>
#include<vector>
#include<queue>
#include<map>
#include<stack>
#include<iostream>
#include<algorithm>
#include "Grammar.h"
#include "FirstUtil.h"
#include "FollowUtil.h"

using namespace std;

class LL1Key {
public:
	string non;
	char input;

	LL1Key() {
		non = "";
		input = 0;
	}
	LL1Key(string _non, char _input) {
		non = _non;
		input = _input;
	}
	bool operator<(LL1Key key) const {
		if (non == key.non) {
			return input < key.input;
		}
		return non < key.non;
	}
};
class PUTIL {
public:
	// ��ӡ����ʽ
	static void printGrammar(Grammar& grammar);
	// ����ʽд���ļ��� 
	static void fwriteGrammar(vector<Production>& vp,string filePath);
	// ���ļ��ж�ȡ����ʽ 
	static void readGrammar(vector< Grammar >& grammars, string fileName);
	// �ӱ�Ƿ���
	static void labelMethod(vector<Production>& vp);
	// ��ӡ���пղ���ʽ�ķ��ս��Non
	static void printNonHaveNull(vector<Production>& vp);
	/// @brief �������ս����Ԥ���������Ϊterms�е�˳�򣬺�����Ϊvp�е�left����
	/// @param vp 
	/// @param non 
	static map<LL1Key, string> getLL1Table(vector<ProductionFirst>& vpf,vector<Production>& vp, vector<string>& terms);
	static map<LL1Key, string> getLL1TablePlus(GrammarPlus& gp);
	/// <summary>
	/// �ж��Ƿ�Ϊ���ս��
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	static bool isNon(string str);
	/// <summary>
	/// �Զ����·���
	/// </summary>
	/// <param name="src"></param>
	/// <param name="LL1Table"></param>
	/// <param name="vp"></param>
	/// <param name="terms"></param>
	static void topToBottom(string src, map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms);
	static void topToBottomNoProcess(string src, map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms);

	/// <summary>
	/// ������ʽת��Ϊ�ַ�����
	/// </summary>
	/// <param name="pro"></param>
	/// <returns></returns>
	
	/// <summary>
	/// ������ʽ�ַ�����ת��Ϊ����ʽ�ִ�
	/// </summary>
	/// <param name="pro"></param>
	/// <returns></returns>
	static string getProFromRights(vector<string> vs);
	/// <summary>
	/// ������ʽ�ִ�ת��Ϊ����ʽ�ַ�����
	/// </summary>
	/// <param name="pro"></param>
	/// <returns></returns>
	static vector<string> getRightsFromPro(string pro);
	/// <summary>
	/// ��ӡLL1Table��
	/// </summary>
	/// <param name="LL1Table"></param>
	/// <param name="vp"></param>
	/// <param name="terms"></param>
	static void printLL1Table(map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms );
	static vector<string> getTerms(vector<Production>& vp);
	//�����ķ�����ݹ�
	static void EliminateLeftRecursion(vector<Production>& vp);
	static void fixNumber(vector<Production>& vp);
private:
	/// <summary>
	/// ���ķ��Ķ�Ԫ���ʾת����һԪ��ʾ��������ʽ����˳�����򣩣�<left,productions> => <productions>
	/// </summary>
	/// <param name="vp"></param>
	/// <returns></returns>
	static vector<string> initProductions(vector<Production>& vp);
	/// <summary>
	/// ���ķ��еķ��ս��src1,src2����
	/// </summary>
	/// <param name="vp"></param>
	/// <param name="src1"></param>
	/// <param name="src2"></param>
	static void switchNon(vector<Production>& vp,string src1,string src2);
	/// <summary>
	/// ��ʼ��LL1TableԤ�������
	/// </summary>
	/// <param name="LL1Tabl"></param>
	/// <param name="vp"></param>
	/// <param name="productions"></param>
	/// <param name="terms"></param>
	/// <param name="vsf"></param>
	/// <param name="vpf"></param>
	/// <param name="vsfo"></param>
	static map<LL1Key, string> initLL1Table( vector<Production>& vp, vector<string>& productions, vector<string>& terms, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf, vector<SymbolFollow>& vsfo);
	static void initLL1TablePlus(vector< vector<string> >& LL1Tabl, vector<Production>& vp, vector<string>& productions, vector<string>& terms, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf, vector<SymbolFollow>& vsfo);

	/// <summary>
	/// �������ս������������LL1Table���е�λ��
	/// </summary>
	/// <param name="left"></param>
	/// <param name="vp"></param>
	/// <returns></returns>
	static int findLeftIndexInLL1(string left, vector< Production >& vp);
	static int findSymbolFirstByLeft(vector<SymbolFirst>& vsf, string left);
	static int findSymbolFollowByLeft(vector<SymbolFollow>& vsfo, string left);
	static int findProID(string left, string term, vector<ProductionFirst>& vpf);
	// findProduction:����left���� 
	static vector<Production>::iterator findProductionByLeft(vector<Production>& vp, Production& p);
	static vector<Production>::iterator findProductionByLeft(vector<Production>& vp, string left);
	// ȥ��ĳ�����ս���� �ظ����壺E = E|EE 
	static void eraseRepeatLeft(Production& p);
	// �������е�leftȥ�滻right�е�left 
	static bool swapLeftToRright(string& right, string& left, string& leftProduction);
	//  ���Ҳ����ֵķ��ս���ӱ�� ����������� 
	static void addLabelForRight(Production& p, queue<string>& q, map<string, bool>& vis);
	//	�ҳ�vp�������Ҳ�����ֱ���Ƴ�ȫΪ���ս����left
	//	�ҳ� left -> term{term} ������left 	
	static bool findLeftToTerm(vector<Production>& vp, queue<string>& q, map<string, bool>& rvis);
	//	����ֵ�������µı�Ǽ��뷵��true��rvis[left][new]��0��ֵΪ1���� 
	//  left���Ҳ����Ƶ���ȫΪ�ս��������table[left][non]=1�ģ����rvis[non]=true 	
	static bool addLabelForLeft(vector<Production>& vp, string termRight, queue<string>& q, map<string, bool>& rvis, vector< vector<int> >& table);
	// �ҳ�����δ����ǵ�left������ 	
	static vector<string> findLeftNotInVis(vector<Production>& vp, map<string, bool>& vis);
	// ɾ��δ����ǵĲ���ʽ 
	static void eraseLeftNotInVis(vector<Production>& vp, map<string, bool>& vis);
	// ��ʼ��map��� 
	static void initMap(vector<Production>& vp, map<string, bool>& vis);
	// ɾ���ղ���ʽ 
	static void eraserNullProduction(vector<Production>& vp);
	// ɾ���Ҳ����ֲ����ڵķ��ս���Ĳ���ʽ(E=E+P��������Pɾ��E),ɾ���˷���true 
	// ��ɾ���ķ���true 
	static bool eraseNotExsitNon(vector<Production>& vp);
	static void initTable(vector<Production>& vp, vector< vector<int> >& table);
	static void print(vector<Production>& vp, map<string, bool>& vis);
	static void printTable(vector<Production>& vp, vector< vector<int> >& table);
	static void printQueue(queue<string> q);
	static void printStack(stack<string> s);
	//Ԥ��������ʼ��������ս����Z�������ڣ�������������"Z"��ѿ�ʼ������ΪZ�������ս������������
	static void preprocessing(vector<Production>& vp,map<string,int>& m);
	// ��ȥ�����ݹ�,ȷ�����з��ս����:Non(i) ::= Non(i)Term | Non(i+1)Term | Non(i+2)Term | Non(i+3)Term .....
	static void EliminateIndirectLeftRecursion(vector<Production>& vp,int pos, map<string, int>& m);
	// ��ȥֱ����ݹ�,��Non(i) ::= Non(i)Term�����
	static void EliminateDirectLeftRecursion(vector<Production>& vp,int pos);
	/// <summary>
	/// ���ڴ���ֱ����ݹ�����򣬴���󣺲���ʽǰ�����ǵ�ǰ���ս����ͷ��.
	/// </summary>
	/// <param name="vp"></param>
	/// <param name="pos"></param>
	static vector<string>::iterator sortForEliminateDirectLeftRecursion(vector<Production>& vp, int pos);
	/// <summary>
	/// ɾ���޷��ӿ�ʼ���Ƶ����ķ��ս��
	/// </summary>
	static void eraseNonNotDerivation(vector<Production>& vp);
};

