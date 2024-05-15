#pragma once
#include <string>
#include <vector>
#include<algorithm>
#include<set>

using namespace std;

#define CHAR_NULL '@' // ���ַ�����@��ʾ
#define STRING_NULL "@" // ���ִ�
#define EOS 36 //�������Ľ�����־,�����﷨����
#define DYH 39 // ������
constexpr auto CHAR_EOS = '$';
constexpr auto STRING_EOS = "$";



class Production {// ĳ�����ս�������в���ʽ 
public:
	int id;// �����̺��� 
	string left;
	vector<string> right;
	int have_null; // �Ƿ��пղ���ʽ 
	Production();
	bool operator==(const Production& p) const {
		return left == p.left;
	}
	bool operator<(const Production& p) const {
		if (left != p.left) {
			return left < p.left;
		}
		return right.size() < p.right.size();
	}
};
// �ķ�����Ԫ���ʾ��<��ʼ�������򼯣�����ʽ�����ս���������ս����>
class Grammar {
public:
	string start;//��ʼ��
	vector<Production> productions;// P������ʽ
	set<char> terms;//�ս������
	set<string> nons;//���ս������
	
	Grammar() {
		start = "";
		productions.clear();
		terms.clear();
		nons.clear();
	}
	Grammar(string _start, vector<Production> _productions, set<string> _nons, set<char> _terms ) {
		start = _start;
		terms = _terms;
		productions = _productions;
		nons = _nons;
	}
};
// �ķ�plus����Ԫ���ʾ��<��ʼ�������򼯣�����ʽ�����ս���������ս����>
///�Ż������ս��
class GrammarPlus {
public:
	string start;//��ʼ��
	vector<Production> productions;// P������ʽ
	set<string> terms;//�ս������
	set<string> nons;//���ս������

	GrammarPlus() {
		start = "";
		productions.clear();
		terms.clear();
		nons.clear();
	}
	GrammarPlus(string _start, vector<Production> _productions, set<string> _nons, set<string> _terms) {
		start = _start;
		terms = _terms;
		productions = _productions;
		nons = _nons;
	}
};