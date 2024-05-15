#include "RDUtil.h"


pair<int, bool> RDGUtil::stringToInt(string str) {
	int i = 0;
	int sum = 0;
	bool isDig = true;
	while ( str[i] >= '0' && str[i] <= '9' && i < str.size() ) {
		sum *= 10;
		sum += str[i]-'0';
		i++;
	}
	if (i != str.size()) {
		isDig = false;
	}
	return { sum,isDig };
}
/// <summary>
/// ����һ������ʽ������ֽ�Ϊ�������飬ÿ��Ԫ���Ǹ÷��ŵ�ֵ������
/// </summary>
/// <param name="production"></param>
/// <returns></returns>
vector<SignType> RDGUtil::getSignTypeFromProduction(string production){
	vector<SignType> vst;
	//ÿ������ | ˵������һ������
	production += "|";
	SignType s;
	int i = 0;
	while (i < production.size()) {
		string word = "";
		// ���ս��,ÿ�����һ���ַ�ʱ��ָ��ͣ�ڸ��ַ������һ����ĸ
		if (production[i] >= 'A' && production[i] <= 'Z') {
			word += production[i];
			if (production[i+1] == DYH) {
				word += production[++i];
			}
			s.value = word;
			s.type = NON;
			vst.push_back(s);
		}// �ս���ĵ���
		else if (production[i] >= 'a' && production[i] <= 'z' || production[i] >= '0' && production[i] <= '9') {
			word += production[i];
			while (production[i+1] >= 'a' && production[i+1] <= 'z' || production[i+1] >= '0' && production[i+1] <= '9') {
				word += production[++i];
			}
			s.value = word;
			s.type = TERM;
			vst.push_back(s);
		}// �ս���Ľ��޷�
		else if ( isDelimiter(production[i])) {
			word += production[i];
			s.value = word;
			s.type = TERM;
			vst.push_back(s);
		}/// �ս���������
		else if (isOperator()) {

		}
		i++;
	}
	return vst;
}

bool RDGUtil::isDelimiter(char c)
{
	set<char> delimiter{ DYH, ';', ',', '.', '{', '}', '(', ')', '*', '/', '+', '-'};
	if (delimiter.find(c) != delimiter.end()) {
		return true;
	}
	return false;
}

bool RDGUtil::isOperator()
{
	return false;
}



/// <summary>
	/// ��ӡ�ķ�
	/// </summary>
void RDGrammar::printRDGrammar() {
	cout << "------------------------------------------\n�ķ�����Ԫ�����£�\n";
	cout << "���ս����: {";
	for (set<string>::iterator it = nons.begin(); it != nons.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "�ս����: {";
	for (set<string>::iterator it = terms.begin(); it != terms.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "���пղ���ʽ�ķ��ս����: {";
	for (set<string>::iterator it = null_pro.begin(); it != null_pro.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "��������ս���������ʣ����£�\n{";
	for (set<string>::iterator it = str_terms.begin(); it != str_terms.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "����: \n";
	for (map<string, vector< vector<SignType> > >::iterator it = regulars.begin(); it != regulars.end(); it++) {
		cout << it->first << " ::= ";
		int blankSize = it->first.size() + 5;
		string blank(blankSize, ' ');
		for (vector<vector<SignType> >::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
			for (vector<SignType>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
				cout << it2->value;
				if ( str_terms.find(it2->value ) != str_terms.end() && !RDGUtil::isDelimiter(it2->value[0])) {
					cout << " ";
				}
			}
			vector<vector<SignType> >::iterator itb = it1;
			itb++;
			if (itb != it->second.end()) {
				cout << "|\n" << blank;
			}
		}
		if (null_pro.find(it->first) != null_pro.end()) {
			cout << "|\n" << blank << STRING_NULL << endl;
		}
		else {
			cout << endl;
		}
	}
	cout << "\n���������͵Ĺ��򼯴�ӡ: \n";
	for (map<string, vector< vector<SignType> > >::iterator it = regulars.begin(); it != regulars.end(); it++) {
		cout << it->first << " ::= ";
		int blankSize = it->first.size()+5;
		string blank(blankSize, ' ');
		for (vector<vector<SignType> >::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
			for (vector<SignType>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
				cout << it2->value;
				if (it2->type == NON) {
					cout << "(non) ";
				}
				else {
					cout << "(term) ";
				}
			}
			vector<vector<SignType> >::iterator itb = it1;
			itb++;
			if (itb != it->second.end()) {
				cout << "|\n" << blank;
			}
		}
		if (null_pro.find(it->first) != null_pro.end()) {
			cout << "|\n" << blank << STRING_NULL << endl;
		}
		else {
			cout << endl;
		}
	}
	cout << "------------------------------------------\n";
}

RDGrammar RDGUtil::grammarToRDGrammar(Grammar& g){
	set<string> nons;// ���ս������
	set<string> terms;// �ս������
	set<string> str_terms;// ������ĵ���
	set<string> null_pro;// ���пղ���ʽ�ķ��ս������

	map<string, vector< vector<SignType> > > regulars;// ĳ�����ս���Ĺ���
	for (set<string>::iterator it = g.nons.begin(); it != g.nons.end(); it++) {
		nons.insert(switchString(*it));
	}
	for (set<char>::iterator it = g.terms.begin(); it != g.terms.end(); it++) {
		string t = "";
		t += *it;
		terms.insert(t);
	}
	for (vector<Production>::iterator it = g.productions.begin(); it != g.productions.end(); it++) {
		vector< vector<SignType> > vvst;
		// ��������ת��Ϊ1
		string left = switchString(it->left);
		for (vector<string>::iterator it1 = it->right.begin(); it1 != it->right.end(); it1++) {
			vector<SignType> vst = getSignTypeFromProduction(*it1);
			for (vector<SignType>::iterator itt = vst.begin(); itt != vst.end(); itt++) {
				itt->value = switchString(itt->value);
			}
			vvst.push_back(vst);
			for (vector<SignType>::iterator it2 = vst.begin(); it2 != vst.end(); it2++) {
				if (it2->type == TERM) {
					str_terms.insert(it2->value);
				}
			}
		}
		
		regulars.insert({ left,vvst });
		if (it->have_null == 1) {
			null_pro.insert(left);
		}
	}

	return RDGrammar(g.start, nons, terms, str_terms, null_pro, regulars);
}

string RDGUtil::switchString(string src)
{
	int i = 0;
	while (i < src.size()) {
		if (src[i] == DYH) {
			src[i] = '1';
		}
		i++;
	}
	return src;
}
