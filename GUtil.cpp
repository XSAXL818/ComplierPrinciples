#include "GUtil.h"


void PUTIL::printGrammar(Grammar& grammar) {
	cout << "���򼯣�" << endl;
	for (int i = 0; i < grammar.productions.size(); i++) {
		cout << grammar.productions[i].id << " " << grammar.productions[i].left << "::=";
		for (vector<string>::iterator it = grammar.productions[i].right.begin(); it != grammar.productions[i].right.end(); it++) {
			cout << *it;
			if (it + 1 != grammar.productions[i].right.end()) {
				cout << "|";
			}
		}
		if (grammar.productions[i].have_null == 1) {
			if (grammar.productions[i].right.size() >= 1) {
				cout << "|";
			}
			cout << STRING_NULL;
		}
		cout << endl;
	}
	cout << "��ʼ����{" << grammar.start << ",}" << endl;
	cout << "���ս�����ϣ�{";
	for (set<string>::iterator it = grammar.nons.begin(); it != grammar.nons.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "�ս�����ϣ�{";
	for (set<char>::iterator it = grammar.terms.begin(); it != grammar.terms.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
}
// ����ʽд���ļ��� 
void PUTIL::fwriteGrammar(vector<Production>& vp,string filePath) {
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filePath.c_str(), "a");
	if (fp == 0) {
		cout << "�ļ���ʧ��!" << endl;
		return;
	}
	for (int i = 0; i < vp.size(); i++) {
		fprintf(fp, "%s::=", vp[i].left.c_str());
		for (vector<string>::iterator it = vp[i].right.begin(); it != vp[i].right.end(); it++) {
			fprintf(fp, "%s", it->c_str());
			if (it + 1 != vp[i].right.end()) {
				fprintf(fp, "|");
			}
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	fclose(fp);
}
// ���ļ��ж�ȡ����ʽ 
//void PUTIL::readGrammar( vector< Grammar >& grammars, string fileName ){
//
//	char line[200];
//	FILE* fp;
//	errno_t err = fopen_s( &fp, fileName.c_str(), "r" );
//	int row = -1;
//	//vector<Production> vp;
//	Grammar g;
//	string start;//��ʼ��
//	vector<Production> productions;// P������ʽ
//	set<char> terms;//�ս������
//	set<string> nons;//���ս������
//	if (fp == 0) {
//		cout << "�ļ���ʧ��!" << endl;
//		return;
//	}
//	while( fgets( line, sizeof(line),fp ) ){// ÿ�ζ�ȡһ�� 
//		Production pro;// ĳ���ս���Ĳ���ʽ 
//		row++;
//		pro.id = row;
//		string str = line;
//		if( str.find_last_of('\n') != -1 )	{// ȥ�����з� 
//			str.erase( --str.end() );
//		}
//		string left = "";
//		int pos = str.find( "::=" );
//		// ��ȡ���,�ɻ�ȡE' 
//		if( pos == -1 ){ // ˵�����µ��ķ�
//			if( !productions.empty() ) {
//				g = Grammar( productions[0].left, productions, nons, terms);
//				start = "";
//				productions = vector<Production>();
//				terms = set<char>();
//				nons = set<string>();
//				grammars.push_back(g);
//				row = -1; 
//			}
//			continue;
//		} else {
//			for( int i = 0; i < pos; i++ ){
//				left += str[i];
//			}
//		}
//		pro.left = left;	
//		nons.insert(left);
//		// ��ȡ�ұ� 
//		string right = "";
//		for( int i = pos+3; i < str.size(); i++ ){
//			if( str[i] == '|' ){
//				pro.right.push_back( right );
//				right = "";
//			} else if( str[i] == CHAR_NULL ){
//				pro.have_null = 1;
//			} else {
//				right += str[i];
//				if (!((str[i] >= 'A' && str[i] <= 'Z') || str[i - 1] >= 'A' && str[i - 1] <= 'Z' && str[i] == DYH)) {
//					terms.insert(str[i]);
//				}
//			}
//		}
//		if( right != "" ){
//			pro.right.push_back( right );
//		}
//		vector<Production>::iterator repeat = findProductionByLeft( productions, pro.left );
//		if( repeat != productions.end() ){// �ظ�����ߺϲ�
//			for( vector<string>::iterator sit = pro.right.begin(); sit != pro.right.end(); sit++ ){
//				repeat->right.push_back( *sit );
//			}
//			if( pro.have_null == 1 ){
//				repeat->have_null = 1;
//			}
//		} else {
//			productions.push_back( pro );
//		}
//	}
//	g = Grammar(productions[0].left, productions, nons, terms);
//	if( !g.productions.empty() ) {
//		grammars.push_back(g);
//	}
//}

// ���ļ��ж�ȡ����ʽ 
void PUTIL::readGrammar(vector< Grammar >& grammars, string fileName) {

	char line[200];
	FILE* fp;
	errno_t err = fopen_s(&fp, fileName.c_str(), "r");
	int row = -1;
	//vector<Production> vp;
	Grammar g;
	string start;//��ʼ��
	vector<Production> productions;// P������ʽ
	set<char> terms;//�ս������
	set<string> nons;//���ս������
	if (fp == 0) {
		cout << "�ļ���ʧ��!" << endl;
		return;
	}
	while (fgets(line, sizeof(line), fp)) {// ÿ�ζ�ȡһ�� 
		Production pro;// ĳ���ս���Ĳ���ʽ 
		row++;
		pro.id = row;
		string str = line;
		if (str.find_last_of('\n') != -1) {// ȥ�����з� 
			str.erase(--str.end());
		}
		string left = "";
		int pos = str.find("::=");
		// ��ȡ���,�ɻ�ȡE' 
		if (pos == -1) { // ˵�����µ��ķ�
			if (!productions.empty()) {
				g = Grammar(productions[0].left, productions, nons, terms);
				start = "";
				productions = vector<Production>();
				terms = set<char>();
				nons = set<string>();
				grammars.push_back(g);
				row = -1;
			}
			continue;
		}
		else {
			for (int i = 0; i < pos; i++) {
				left += str[i];
			}
		}
		pro.left = left;
		nons.insert(left);
		// ��ȡ�ұ� 
		string right = "";
		for (int i = pos + 3; i < str.size(); i++) {
			if (str[i] == '|') {
				pro.right.push_back(right);
				right = "";
			}
			else if (str[i] == CHAR_NULL) {
				pro.have_null = 1;
			}
			else {
				right += str[i];
				if (!((str[i] >= 'A' && str[i] <= 'Z') || str[i - 1] >= 'A' && str[i - 1] <= 'Z' && str[i] == DYH)) {
					terms.insert(str[i]);
				}
			}
		}
		if (right != "") {
			pro.right.push_back(right);
		}
		vector<Production>::iterator repeat = findProductionByLeft(productions, pro.left);
		if (repeat != productions.end()) {// �ظ�����ߺϲ�
			for (vector<string>::iterator sit = pro.right.begin(); sit != pro.right.end(); sit++) {
				repeat->right.push_back(*sit);
			}
			if (pro.have_null == 1) {
				repeat->have_null = 1;
			}
		}
		else {
			productions.push_back(pro);
		}
	}
	terms.erase(' ');
	g = Grammar(productions[0].left, productions, nons, terms);
	if (!g.productions.empty()) {
		grammars.push_back(g);
	}

	fclose(fp);
}

// ��ӡ���пղ���ʽ�ķ��ս��Non
void PUTIL::printNonHaveNull(vector<Production>& vp) {
	cout << "���пղ���ʽ�ķ��ս�����£�" << endl;
	for (int i = 0; i < vp.size(); i++) {
		if (vp[i].have_null == 1) {
			cout << vp[i].left << " ";
		}
	}
	cout << endl;
}
/// @brief �������ս����Ԥ���������Ϊterms�е�˳�򣬺�����Ϊvp�е�left����
	/// @param vp 
	/// @param non 
map<LL1Key,string>  PUTIL::getLL1Table(vector<ProductionFirst>& vpf,vector<Production>& vp, vector<string>& terms) {
	vector<RecordFirst> vrf;
	vector<SymbolFirst> vsf;
	//vector<ProductionFirst> vpf;
	cout << "********************��ȡFRIST��ʼ**************" << endl;
	FirstUtil::getFirst(vp, vrf, vsf, vpf);
	cout << "********************��ȡFRIST�����**************" << endl;
	cout << "********************��ȡFOLLOW��ʼ**************" << endl;
	vector<SymbolFollow> vsfo = FollowUtil::getFollow(vp, vsf, vpf);
	cout << "********************��ȡFOLLOW�����**************" << endl;
	cout << "���ս����FIRST���£�" << endl;
	FirstUtil::printSymbolFirst(vsf);
	
	cout << "���ս����FOLLOW���£�" << endl;
	FollowUtil::printSymbolFollow(vsfo);
	// ��ʼ���﷨Ԥ���
	vector< vector<string> > LL1Table(vp.size(), vector<string>(terms.size()));
	// �������ʽ����
	vector<string> productions = initProductions(vp);
	cout << "����ʽ��" << endl;
	for (int i = 0; i < productions.size(); i++) {
		cout << i << " " << productions[i] << endl;
	}


	///����
	/*for (int i = 0; i < vpf.size() - 1; i++) {
		set<string> newFirst{};
		string pro = productions[vpf[i].proID];
		vector<string> vs = getRightsFromPro(pro);
		bool allNon = true;
		for (int j = 0; j < vs.size(); j++) {
			if (isNon(vs[j])) {
				int smIndex = findSymbolFirstByLeft(vsf, vs[j]);
				newFirst.insert(vsf[smIndex].firstSet.begin(), vsf[smIndex].firstSet.end());
				if (newFirst.find("@") == newFirst.end()) {
					allNon = false;
				}
			}
			else {
				allNon = false;
			}
			if (!allNon) {
				break;
			}
		}
		vpf[i].firstSet = newFirst;
	}*/


	cout << "����ʽ��FIRST��" << endl;
	FirstUtil::printGrammarFirst(vpf);

	
	///
	
	// ��ȡLL1Table����Ԥ���
	return initLL1Table( vp, productions, terms, vsf, vpf, vsfo);
}




map<LL1Key, string> PUTIL::getLL1TablePlus(GrammarPlus& gp){
	vector<RecordFirst> vrf;
	vector<SymbolFirst> vsf;
	vector<ProductionFirst> vpf;
	// �������ʽ����
	FirstUtil::getFirst(gp.productions, vrf, vsf, vpf);
	cout << "********************��ȡFRIST��**************" << endl;
	// �������ʽ����
	vector<SymbolFollow> vsfo = FollowUtil::getFollow(gp.productions, vsf, vpf);
	// �������ʽ����
	cout << "���ս����FIRST���£�" << endl;
	FirstUtil::printSymbolFirst(vsf);
	cout << "���ս����FOLLOW���£�" << endl;
	FollowUtil::printSymbolFollow(vsfo);
	// ��ʼ���﷨Ԥ���
	vector< vector<string> > LL1Table(gp.productions.size(), vector<string>(gp.terms.size()));
	// �������ʽ����
	vector<string> productions = initProductions(gp.productions);
	cout << "����ʽ��" << endl;
	for (int i = 0; i < productions.size(); i++) {
		cout << i << " " << productions[i] << endl;
	}
	cout << "����ʽ��FIRST��" << endl;
	FirstUtil::printGrammarFirst(vpf);
	
	vector<string> terms;
	for (string s : gp.terms) {
		terms.push_back(s);
	}
	// ��ȡLL1Table����Ԥ���
	return initLL1Table(gp.productions, productions, terms, vsf, vpf, vsfo);
}
// �ӱ�Ƿ�:��Ҫȥ���������� 
// 1.ֱ����ݹ飨E=E�� 
// 2.���������ʽ��E=E+T,T=i,P=i��,ȥ��P 
// 3.�޷����Ҳ�����ȫ���ս����S=i�����Ƶ��Ĳ���ʽ��E=E+T,T=i,P=G,G=P��,ȥ��P,G 
// �������ݽṹ
// Production���������ս�����������Ҳ���E=E+T|T�� 
// ���У��ӿ�ʼ��S�Ƶ����ķ��ս��ȫ��������У����ڱ�ǡ�
// �̺���ӳ�䣩��table[E][T]=1 ��ʾE���Ҳ���T��ʹ�ö�ά���飬Ҳ��multimapʵ�֡�
//	queue:  
// ���蹦��ģ�飺
// void eraseRepeatLeft( Production& p )
// ����ֱ��p��ֱ����ݹ� (E=E)
// bool addLabelForRight( Production p,queue q,map<string,bool>& vis )
// ���Ҳ����ֵķ��ս���ӱ�� 
// ����ֵ���Ҳ�����δ��ǵķ���true,����Ϊfalse�� Ŀǰû����; 
// ����ĳ�����ս���Ĳ���ʽp(Z=E+T),���Ҳ����ֵĲ���ʽ�������q,�����Ϊ���Ƶ���vis[E]=true
// vector<string> findLeftToTerm( vector<production>& vp )
// �ҳ�vp�������Ҳ�����ֱ���Ƴ�ȫΪ���ս����left
// �ҳ� left -> term{term} ������left 

// bool addLabelForLeft( string left,queue q,map<string,bool> rvis, int** table )
// bool addLabelForLeft( string left,queue q,map<string,bool> rvis, int** table )
// ����ֵ�������µı�Ǽ��뷵��true��rvis[left][new]��0��ֵΪ1���� 
// left���Ҳ����Ƶ���ȫΪ�ս��������table[left][non]=1�ģ����rvis[non]=true 

// vector<string> findLeftNotInVis( vector<production>& vp, map<string,bool> vis )
// �ҳ�����δ����ǵ�lef������ 

// void eraseLeftNotInVis( vector<production>& vp, map<string,bool> vis )

// �ӱ�Ƿ���
// ���裺
// 1.��ʼ��������ʼ��S������� q,���S(vis[S]=1),
void PUTIL::labelMethod(vector<Production>& vp) {
	// ��ʼ�����ݽṹ 
	queue<string> q;
	map<string, bool> vis;
	initMap(vp, vis);
	// �ڶ�����ɾ�������Ƶ���ȫΪ�ս����Non������Non��ɾ��������ĳЩ����ʽ��ɾ���� 
	// ���õ���һ��ɨ�裬�����ݣ������޷����洦����ǰ����Ӱ�� 
	bool isLoop = false;
	// ��E=E+T|S+F|T,T=S,S=i,F=P������ѭ��E��T��S��F���ڶ���ɾ����F=P,��ʱ����ǰ���E=S+F��Ҫɾ���� 
	// ��һ�����ӿ�ʼ���Ƶ������п��Գ��ֵ�
	// 1����ʼ��������ʼ��������в���� 
	q.push(vp[0].left);// ����ʼ�����뵽���� 
	vis[vp[0].left] = true;
	// �ӿ�ʼ���Ƶ� 
	while (!q.empty()) {
		// 2��ȡ��һ����ߵķ��ս�� 
		string left = q.front();
		q.pop();
		vector<Production>::iterator pro = findProductionByLeft(vp, left);
		// 3�����������ֱ����ݹ�
		eraseRepeatLeft(*pro);
		// 4�����÷��ս���ұ߳��ֵ�<δ��ǹ��ķ��ս��>��ǣ����������
		addLabelForRight(*pro, q, vis);
	}
	// 5��ɾ�����ܴӿ�ʼ�Ƴ��ķ��ս���������ʽ
	eraseLeftNotInVis(vp, vis);
	// 6��ɾ������ɾ��ֱ����ݹ鵼���޲���ʽ�ķ��ս��
	eraserNullProduction(vp);
	// �ڶ������ӿ����Ƴ�ȫΪ�ս���ķ��ս����ʼ
	// 1����ʼ������ֱ�����Ƶ���ȫΪ�ս����left���뵽����,����� 
	q = queue<string>();
	initMap(vp, vis);
	findLeftToTerm(vp, q, vis);// ���Ƶ���ȫΪ�ս���ļ������ 
	// ��ʼ���̺���E=E+T������table[T][E] = 1,��Ϊ���ս�����±�š�Ϊ�˷����̺���Ľ���
	vector< vector<int> > table(vp.size(), vector<int>(vp.size()));
	initTable(vp, table);
	while (!q.empty()) {
		// 2��ȡ��һ����ߵķ��ս��left 
		string left = q.front();
		q.pop();
		Production pro = *findProductionByLeft(vp, left);
		// 3�����̺��÷��ս��left�������<�����Ƶ���ȫ�ս���ķ��ս��>���
		addLabelForLeft(vp, left, q, vis, table);
	}
	// 4��ɾ���޷����Ƶ���ȫΪ�ս���ķ��ս��left
	eraseLeftNotInVis(vp, vis);// �÷������л��ݵ����� 
	// 5��ɾ���Ҳ�������ɾ�����ս���Ĳ���ʽ 
	isLoop = eraseNotExsitNon(vp);
	if (isLoop) {
		labelMethod(vp);
	}
	else {
		cout << "***********�ӱ�Ƿ��������***************" << endl;
	}
}
// findProduction:����left���� 
vector<Production>::iterator PUTIL::findProductionByLeft(vector<Production>& vp, Production& p) {
	return find(vp.begin(), vp.end(), p);
}
vector<Production>::iterator PUTIL::findProductionByLeft(vector<Production>& vp, string left) {
	Production p;
	p.left = left;
	return find(vp.begin(), vp.end(), p);
}

// ȥ��ĳ�����ս���� �ظ����壺E = E|EE 
void PUTIL::eraseRepeatLeft(Production& p) {
	for (int i = 0; i < p.right.size(); i++ ) {
		bool erase = false;
		if (p.right[i] == p.left) { // ��߱�ʶ�����ұ߲���ʽ��ͬ��������, ɾ�� 
			erase = true;
		}
		else {
			int len = p.left.size();
			int first = p.right[i].find(p.left);// �������� left�ĵ�һ�� 
			int second;// �ڶ��� 
			while (1) {
				if (first + len == p.right[i].size()) {// �ַ����������ˣ������ڵڶ����ˣ�˵���Ҳ�Ϊ left�ظ�������ɾ�� 
					erase = true;
					break;
				}
				second = p.right[i].find(p.left, first + len);//�ڶ�����λ��
				if (second == -1) {// �ڶ���û�ҵ�,���������ظ�����left�� ���ɾ�� 
					break;
				}
				else {
					if (first + len == second) {// ��һ���͵ڶ������ڣ���������һ����
						first = second;
					} else {// �����ظ�����left�����ɾ�� 
						break;
					}
				}
			}
		}
		if (erase) {
			p.right.erase(i+p.right.begin());
			i--;
		}
	}
}
// �������е�leftȥ�滻right�е�left 
bool PUTIL::swapLeftToRright(string& right, string& left, string& leftProduction) {
	bool isSwap = false;
	int pos = right.find(left);
	while (pos != -1) {
		right.erase(pos, left.size());
		right.insert(pos, leftProduction);
		pos = right.find(left, pos + left.size());
		isSwap = true;
	}
	return isSwap;
}
//  ���Ҳ����ֵķ��ս���ӱ�� ����������� 
void PUTIL::addLabelForRight(Production& p, queue<string>& q, map<string, bool>& vis) {
	for (vector<string>::iterator it = p.right.begin(); it != p.right.end(); it++) {
		vector<string> rights = getRightsFromPro(*it);
		for (int i = 0; i < rights.size(); i++) {
			if (isNon(rights[i])) {
				if (!vis[rights[i]]) {
					q.push(rights[i]);
					vis[rights[i]] = true;
				}
			}
			
		}
		/*string str = *it;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] >= 'A' && str[i] <= 'Z' && str[i] != p.left[0]) {
				string t = "";
				t += str[i];
				if (!vis[t]) {
					q.push(t);
					vis[t] = true;
				}

			}
		}*/
	}
}
//	�ҳ�vp�������Ҳ�����ֱ���Ƴ�ȫΪ���ս����left
//	�ҳ� left -> term{term} ������left 	
bool PUTIL::findLeftToTerm(vector<Production>& vp, queue<string>& q, map<string, bool>& rvis) {
	for (int i = 0; i < vp.size(); i++) {
		bool terms=true;
		for (int j = 0; j < vp[i].right.size(); j++) {
			terms = true;
			string right = vp[i].right[j];
			for (int k = 0; k < right.size(); k++) {
				if (right[k] >= 'A' && right[k] <= 'Z') {
					terms = false;
					break;
				}
			}
			if (terms) {
				break;
			}
		}
		if (terms) {
			q.push(vp[i].left);
			rvis[vp[i].left] = true;
		}
	}
	return true;
}
//	����ֵ�������µı�Ǽ��뷵��true��rvis[left][new]��0��ֵΪ1���� 
//  left���Ҳ����Ƶ���ȫΪ�ս��������table[left][non]=1�ģ����rvis[non]=true 	
bool PUTIL::addLabelForLeft(vector<Production>& vp, string termRight, queue<string>& q, map<string, bool>& rvis, vector< vector<int> >& table) {
	int pos = -1;
	while (vp[++pos].left != termRight) {}// �ҵ�termRight��vp�е�λ��
	for (int i = 0; i < vp.size(); i++) {
		if (table[pos][i] == 1 && !rvis[vp[i].left]) {// ��left���Է��Ƶ��ķ��ս�� 
			Production pro = vp[i];
			bool can = true;
			for (int j = 0; j < pro.right.size(); j++) {
				string right = pro.right[j];
				can = true;
				if (right.find(termRight) != -1) {// right�к���termRight 
					for (int k = 0; k < right.size(); k++) {
						string t = "";
						t += right[k];
						if (right[k] >= 'A' && right[k] <= 'Z' && rvis[t] == false) {// �����˷��ս�����Ҹ÷��ս�����Ƴ�ȫΪ�ս�� 
							can = false;
							break;
						}
					}
					if (can) {
						break;
					}
				}
				else {
					can = false;
				}
			}
			if (can) {
				rvis[pro.left] = true;
				q.push(pro.left);
			}
		}
	}
	return true;
}
//0 Z::=E+T
//1 E::=S+F|T
//2 F::=FP|P
//3 P::=G
//4 G::=F
//5 T::=T*i|i
//6 S::=i
// ��ǣ�S��T 
// ���У� 
// ��һ�� ���Ҳ����е�S����S=i����ʱ�����������Ƶ���S�ķ��ս�� 
// �ҳ�����δ����ǵ�left������ 	
vector<string> PUTIL::findLeftNotInVis(vector<Production>& vp, map<string, bool>& vis) {
	return vector<string>();
}


// ɾ��δ����ǵĲ���ʽ 
void PUTIL::eraseLeftNotInVis(vector<Production>& vp, map<string, bool>& vis) {
	for (int i = 0; i < vp.size(); i++) {
		if (!vis[vp[i].left]) {
			vp.erase(vp.begin() + i);
			i--;
		}
	}
}
// ��ʼ��map��� 
void PUTIL::initMap(vector<Production>& vp, map<string, bool>& vis) {
	vis.clear();
	for (int i = 0; i < vp.size(); i++) {
		vis.insert(make_pair(vp[i].left, false));
	}

}
// ɾ���ղ���ʽ 
void PUTIL::eraserNullProduction(vector<Production>& vp) {
	for (int i = 0; i < vp.size(); i++) {
		if (vp[i].right.size() == 0) {
			vp.erase(vp.begin() + i);
			i--;
		}
	}
}
// ɾ���Ҳ����ֲ����ڵķ��ս���Ĳ���ʽ(E=E+P��������Pɾ��E),ɾ���˷���true 
// ��ɾ���ķ���true 
bool PUTIL::eraseNotExsitNon(vector<Production>& vp) {

	map<string, bool> m;
	for (int i = 0; i < vp.size(); i++) {// ������г�������ߵķ��ս�� 
		m.insert(make_pair(vp[i].left, true));
	}
	bool isLoop = false;//��Ҫѭ��:ĳ�����ս�����Ҳ�ȫ��ɾ��,�÷��ս������Ӱ����ɨ��� 
	for (int i = 0; i < vp.size(); i++) {
		//			cout << i << endl;
		for (int j = 0; j < vp[i].right.size(); j++) {
			string pro = vp[i].right[j];
			for (int k = 0; k < pro.size(); k++) {
				string t = "";
				t += pro[k];
				if (pro[k] >= 'A' && pro[k] <= 'Z' && m[t] == false) {// ɾ��ĳ������ʽ 
					vp[i].right.erase(j + vp[i].right.begin());
					j--;
					isLoop = true;
					break;
				}
			}
		}
		if (vp[i].right.size() == 0) {// ��ǰ���ս����Ҫɾ��
			vp.erase(vp.begin() + i);
			i--;
			//				 isLoop = true;// �ٴ�ѭ�� 
		}
	}
	if (isLoop) {// �������ɾ����Ӱ��ǰ�棬���ظ�ִ�� 
		eraseNotExsitNon(vp);
	}

	return isLoop;
}
/// <summary>
/// ��ʼ����Ǳ�,����ÿ��Ϊ<���ս���������±�>
/// </summary>
/// <param name="vp">�ķ������в���ʽ</param>
/// <param name="table">��Ǳ�</param>
void PUTIL::initTable(vector<Production>& vp, vector< vector<int> >& table) {
	for (int i = 0; i < vp.size(); i++) {
		vp[i].id = i;
	}
	for (int i = 0; i < vp.size(); i++) {
		string left = vp[i].left;
		for (int j = 0; j < vp[i].right.size(); j++) {
			string right = vp[i].right[j];
			for (int k = 0; k < right.size(); k++) {
				string t = "";
				if (right[k] >= 'A' && right[k] <= 'Z') {
					t += right[k];
					int id = (*findProductionByLeft(vp, t)).id;
					table[id][i] = 1;
				}
			}
		}
	}
}
/// <summary>
/// ��ӡ��Ǳ�
/// </summary>
/// <param name="vp"></param>
/// <param name="vis"></param>
void PUTIL::print(vector<Production>& vp, map<string, bool>& vis) {
	for (int i = 0; i < vp.size(); i++) {
		cout << vp[i].left;
		if (vis[vp[i].left]) {
			cout << " true" << endl;
		}
		else {
			cout << " false" << endl;
		}
	}
}
/// <summary>
/// ��ӡ��Ǳ�
/// </summary>
/// <param name="vp"></param>
/// <param name="table"></param>
void PUTIL::printTable(vector<Production>& vp, vector< vector<int> >& table) {
	cout << "\t";
	int len = vp.size();
	for (int i = 0; i < len; i++) {
		cout << vp[i].left << "\t";
	}
	cout << "\n";
	for (int i = 0; i < len; i++) {
		cout << vp[i].left << "\t";
		for (int j = 0; j < len; j++) {
			cout << table[i][j] << "\t";
		}
		cout << endl;
	}
}
/// <summary>
/// ��ӡ�ַ�������
/// </summary>
/// <param name="q"></param>
void PUTIL::printQueue(queue<string> q) {
	while (!q.empty()) {
		cout << q.front() << endl;
		q.pop();
	}
}
/// <summary>
/// �ж��Ƿ�Ϊ���ս��
/// </summary>
/// <param name="str">�ַ�</param>
/// <returns>���ս������true������false</returns>
bool PUTIL::isNon(string str) {
	if (str[0] >= 'A' && str[0] <= 'Z') {
		return true;
	}
	return false;
}
/// <summary>
/// �Զ����·���
/// </summary>
/// <param name="src">���������ַ���</param>
/// <param name="LL1Table">����Ԥ���</param>
/// <param name="vp">����ʽ��</param>
/// <param name="terms">�ս����</param>
void PUTIL::topToBottom(string src, map<LL1Key,string>& LL1, vector<Production>& vp, vector<string>& terms) {
	// ������ִ���$��β����������ĸ������$��
	src += "$";
	stack<string> s;
	int currentIndex = 0;
	// ���ȼ��뿪ʼ�����ַ���������EOS
	s.push("$");
	s.push(vp[0].left);
	cout << "�������Ϊ��" << src << endl << "\t\t";
	printStack(s);
	LL1Key key;
	string getStr = "";
	while (!s.empty() && currentIndex < src.size() ) {
		while (src[currentIndex] == '@') {
			currentIndex++;
		}
		// ȡ������ķ���
		string top = s.top();
		s.pop();
		if (top == "$") {
			currentIndex++;
			break;
		}
		else if (top == "@") {
			
		}
		else if (isNon(top)) {// ���ս��
			string pro;
			key.input = src[currentIndex];
			key.non = top;
			// ���
			if (LL1.find(key) != LL1.end()) {
				pro = LL1[key];
			}
			else {// δ�鵽���˳�
				break;
			}
			// ������ʽ����ջ
			vector<string> rights = getRightsFromPro(pro);
			for (int i = rights.size() - 1; i >= 0; i--) {
				s.push(rights[i]);
			}
			// ��ӡ�м���Ϣ
			cout << "������ƥ�䣺<" << top << "," << key.input << "> = " << pro << endl;
			cout << getStr << "\t\t";
			printStack(s);
		}
		else {
			getStr += src[currentIndex];
			cout << "�ִ�ƥ��: " << src[currentIndex] << "\n" << getStr << "\t\t";
			if (top[0] == src[currentIndex]) {
				currentIndex++;
			}
			else {
				break;
			}
			printStack(s);
		}
	}
	if (currentIndex != src.size()) {
		cout << "�Ƶ�����,�ⲻ��һ������!" << endl;
	}
	else {
		cout << "����һ�����ӡ�" << endl;
	}
}

void PUTIL::topToBottomNoProcess(string src, map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms) {
	// ������ִ���$��β����������ĸ������$��
	src += "$";
	stack<string> s;
	int currentIndex = 0;
	// ���ȼ��뿪ʼ�����ַ���������EOS
	s.push("$");
	s.push(vp[0].left);
	cout << "�������Ϊ��" << src << endl;
	LL1Key key;
	string getStr = "";
	while (!s.empty() && currentIndex < src.size()) {
		while (src[currentIndex] == '@') {
			currentIndex++;
		}
		// ȡ������ķ���
		string top = s.top();
		s.pop();
		if (top == "$") {
			currentIndex++;
			break;
		}
		else if (top == "@") {

		}
		else if (isNon(top)) {// ���ս��
			string pro;
			key.input = src[currentIndex];
			key.non = top;
			// ���
			if (LL1.find(key) != LL1.end()) {
				pro = LL1[key];
			}
			else {// δ�鵽���˳�
				break;
			}
			// ������ʽ����ջ
			vector<string> rights = getRightsFromPro(pro);
			for (int i = rights.size() - 1; i >= 0; i--) {
				s.push(rights[i]);
			}
		}
		else {
			getStr += src[currentIndex];
			if (top[0] == src[currentIndex]) {
				currentIndex++;
			}
			else {
				break;
			}
		}
	}
	if (currentIndex != src.size()) {
		cout << "�Ƶ�����,�ⲻ��һ������!" << endl;
	}
	else {
		cout << "����һ�����ӡ�" << endl;
	}
}
/// <summary>
/// ��ĳ������ʽ�������ذ�����˳������ķ��ս�����ս����vector
/// </summary>
/// <param name="pro">����ʽ</param>
/// <returns>�ַ�����</returns>
vector<string> PUTIL::getRightsFromPro(string pro) {
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
		else if( pro[i]== '$') {
			t += pro[i];
		}
		rights.push_back(t);
	}
	return rights;
}

/// <summary>
/// ��ʼ������ʽ���飬���ķ��в���ʽ���ֵ�˳��洢
/// </summary>
/// <param name="vp">�ķ������в���ʽ</param>
/// <returns>����ʽ����</returns>
vector<string> PUTIL::initProductions(vector<Production>& vp) {
	vector<string> productions;
	for (int i = 0; i < vp.size(); i++) {
		//cout << "left : " << vp[i].left << endl;
		for (vector<string>::iterator it = vp[i].right.begin(); it != vp[i].right.end(); it++) {
			productions.push_back(*it);
		}
		if (vp[i].have_null == 1) {
			productions.push_back(STRING_NULL);
		}
	}
	return productions;
}
/// <summary>
/// ��ѯLL1����ĳ�����ս��������λ��
/// </summary>
/// <param name="left">�������ս��</param>
/// <param name="vp">�������LL1Table�Ĳ���ʽ</param>
/// <returns></returns>
int PUTIL::findLeftIndexInLL1(string left, vector<Production>& vp) {
	for (int i = 0; i < vp.size(); i++) {
		if (vp[i].left == left) {
			return i;
		}
	}
	return vp.size();
}
/// <summary>
/// ��ѯ���ս����SymbolFirst��λ��
/// </summary>
/// <param name="vsf">SymbolFirst��</param>
/// <param name="left">����Non</param>
/// <returns>���������±�</returns>
int PUTIL::findSymbolFirstByLeft(vector<SymbolFirst>& vsf, string left) {
	int i = 0;
	while (i < vsf.size()) {
		if (vsf[i].left == left) {
			break;
		}
		i++;
	}
	return i;
}
/// <summary>
/// ���ҷ��ս����SymbolFollow�е��±�
/// </summary>
/// <param name="vsfo"></param>
/// <param name="left">����Non</param>
/// <returns>�����±꣬δ�ҵ�����vsfo.size()</returns>
int PUTIL::findSymbolFollowByLeft(vector<SymbolFollow>& vsfo, string left) {
	int i = 0;
	while (i < vsfo.size()) {
		if (vsfo[i].left == left) {
			break;
		}
		i++;
	}
	return i;
}
/// <summary>
/// ��ѯProductionFirst�з��ս����First��ĳ���ս�����ڵ��±�
/// </summary>
/// <param name="left">�������ս��</param>
/// <param name="term">�����ս��</param>
/// <param name="vpf"></param>
/// <returns>���ڲ���ʽ���±�,��vector<string> productions,���±���ͬ</returns>
int PUTIL::findProID(string left, string term, vector<ProductionFirst>& vpf) {
	int i = 0;
	while (i < vpf.size()) {
		if (vpf[i].left == left && vpf[i].firstSet.find(term) != vpf[i].firstSet.end()) {// ƥ�䵽FIRST
			break;
		}
		i++;
	}
	return vpf[i].proID;
}
/// <summary>
/// ��ʼ��LL1Table��
/// </summary>
/// <param name="LL1Tabl">�����ɵ�Ԥ�������Ҫ��size���ڵ���vp.size()</param>
/// <param name="vp">�ķ������в���ʽ,ÿ��Ԫ����һ����Ԫ��<���ս��,��Ӧ�����в���ʽ></param>
/// <param name="productions">�ķ��Ĳ���ʽ�����ķ��в���ʽ�ĳ���˳������</param>
/// <param name="terms">�ķ��г��ֵķ��ս����</param>
/// <param name="vsf">SymbolFirst,���ս����FIRST��</param>
/// <param name="vpf">����ʽ��First��</param>
/// <param name="vsfo">���ս����Follow��</param>
map<LL1Key, string> PUTIL::initLL1Table( vector<Production>& vp, vector<string>& productions, vector<string>& terms, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf, vector<SymbolFollow>& vsfo) {
	string strNull = "";
	string strEnd = "";
	strNull += CHAR_NULL;
	strEnd += EOS;

	map<LL1Key, string> LL1;
	for (int i = 0; i < vp.size(); i++) {
		string left = vp[i].left;
		//�ҵ����ս����FIRST
		int symbolFirstIndex = findSymbolFirstByLeft(vsf, left);
		for (string term : vsf[symbolFirstIndex].firstSet) {
			if (term == "@") {
				// FIRST���������ʽ				
				// �ҵ�FOLLOW
				int symbolFollowIndex = findSymbolFollowByLeft(vsfo, left);
				for (string item : vsfo[symbolFollowIndex].followSet) {
					LL1.insert({ LL1Key(left,item[0]),"@"});
				}
			} else {
				// FIRSTƥ��
				int proID = findProID(left, term, vpf);
				LL1.insert({ LL1Key(left,term[0]),productions[proID]});
			}
		}
	}
	/*for (int i = 0; i < vpf.size(); i++) {
		string head = vpf[i].left;
		string pro = productions[vpf[i].proID];
		for (string c : vpf[i].firstSet) {			
			if (c != "@") {
				LL1Key key;
				key.non = head;
				key.input = c[0];
				if (LL1.find(key) == LL1.end()) {
					LL1.insert({ key, pro });
				}
			}
			else {
				int foIndex = findSymbolFollowByLeft(vsfo, head);
				for (string cc : vsfo[foIndex].followSet) {
					LL1Key key;
					key.non = head;
					key.input = cc[0];
					if (LL1.find(key) == LL1.end()) {
						LL1.insert({ key, pro });
					}
				}
			}
		}
	}*/
	return LL1;
}
/// <summary>
/// ��ӡջ�е�Ԫ��
/// </summary>
/// <param name="s"></param>
void PUTIL::printStack(stack<string> s) {
	while (!s.empty()) {
		cout << s.top();
		s.pop();
	}
	cout << endl;
}
/// <summary>
/// ��ӡԤ�������
/// </summary>
/// <param name="LL1Table"></param>
/// <param name="vp"></param>
/// <param name="terms"></param>
void PUTIL::printLL1Table(map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms) {
	for (string item : terms) {
		cout << "\t" << item[0];
	}
	cout << endl;
	for (int i = 0; i < vp.size(); i++) {
		cout << vp[i].left;
		for (int j = 0; j < terms.size(); j++) {
			map<LL1Key, string>::iterator it = LL1.find(LL1Key(vp[i].left, terms[j][0]));
			if (it != LL1.end()) {
				cout << "\t" << it->second;
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
}
/// <summary>
/// ��ȡ�ķ��е������ս��
/// </summary>
/// <param name="vp"></param>
/// <returns></returns>
vector<string> PUTIL::getTerms(vector<Production>&vp) {
	set<string> s;
	for (int i = 0; i < vp.size(); i++) {
		for (int j = 0; j < vp[i].right.size(); j++) {
			string pro = vp[i].right[j];
			for (int k = 0; k < pro.size(); k++) {
				if ( !( pro[k]>='A' && pro[k]<='Z') && pro[k] != CHAR_NULL && pro[k]!=DYH) {
					string t = "";
					t += pro[k];
					s.insert(t);
				}
			}
		}
	}
	vector<string> vs;
	for (set<string>::iterator it = s.begin(); it != s.end(); it++) {
		vs.push_back(*it);
	}
	vs.push_back("$");
	return vs;
}
// ��ȥ��ݹ�
void PUTIL::EliminateLeftRecursion(vector<Production>& vp) {
	string start = vp[0].left;
	// ��һ���������ս�������ҿ�ʼ����Ϊ�����е����һ��
	//Ԥ��������ʼ��������ս����Z�������ڣ�������������"Z"��ѿ�ʼ������ΪZ�������ս������������
	map<string, int> m;// ��¼���ս����vp�е��±�
	preprocessing(vp, m);
	// �ڶ�����������ݹ飬���� Non(i) ::= Non(i+1)Term | Non(i+2)Term | Non(i+3)Term | Non(i+4)Term .....
	for (int i = 0; i < vp.size(); i++) {// ѭ��ĳ�����ս���������ʽ
		// Non:���ս����Term:�ս����Non(i):�ķ��ĵ�i�����ս��
		// ��ȥ�����ݹ�,ȷ�����з��ս����:Non(i) ::= Non(i)Term | Non(i+1)Term | Non(i+2)Term | Non(i+3)Term .....
		EliminateIndirectLeftRecursion( vp,i,m);
		// ��ȥֱ����ݹ�,��Non(i) ::= Non(i)Term�����
		EliminateDirectLeftRecursion(vp,i);
	}
	// ʹ��ʼ��λ������ĵ�һ��Ԫ��
	vector<Production>::iterator it = findProductionByLeft(vp, start);
	Production p = *it;
	*it = vp[0];
	vp[0] = p;
	// ��������ȥ�����ɴӿ�ʼ���Ƶ��Ĳ���ʽ
	eraseNonNotDerivation(vp);
}
/// <summary>
/// ɾ���޷��ӿ�ʼ���Ƶ����ķ��ս��
/// </summary>
void PUTIL::eraseNonNotDerivation(vector<Production>& vp) {
	// ��ʼ�����ݽṹ 
	queue<string> q;
	map<string, bool> vis;
	initMap(vp, vis);
	// �ڶ�����ɾ�������Ƶ���ȫΪ�ս����Non������Non��ɾ��������ĳЩ����ʽ��ɾ���� 
	// ���õ���һ��ɨ�裬�����ݣ������޷����洦����ǰ����Ӱ�� 
	bool isLoop = false;
	// ��E=E+T|S+F|T,T=S,S=i,F=P������ѭ��E��T��S��F���ڶ���ɾ����F=P,��ʱ����ǰ���E=S+F��Ҫɾ���� 
	// ��һ�����ӿ�ʼ���Ƶ������п��Գ��ֵ�
	// 1����ʼ��������ʼ��������в���� 
	q.push(vp[0].left);// ����ʼ�����뵽���� 
	vis[vp[0].left] = true;
	// �ӿ�ʼ���Ƶ� 
	while (!q.empty()) {
		// 2��ȡ��һ����ߵķ��ս�� 
		string left = q.front();
		q.pop();
		vector<Production>::iterator pro = findProductionByLeft(vp, left);
		// 3�����÷��ս���ұ߳��ֵ�<δ��ǹ��ķ��ս��>��ǣ����������
		addLabelForRight(*pro, q, vis);
	}
	// 4��ɾ�����ܴӿ�ʼ�Ƴ��ķ��ս���������ʽ
	eraseLeftNotInVis(vp, vis);
	// 5��ɾ������ɾ��ֱ����ݹ鵼���޲���ʽ�ķ��ս��
	eraserNullProduction(vp);
}

//Ԥ����
void PUTIL::preprocessing(vector<Production>& vp, map<string, int>& m) {
	//����ʼ���������������
	Production p = vp[0];
	vp[0] = vp[vp.size()-1];
	vp[vp.size() - 1] = p;
	for (int i = 0; i < vp.size(); i++) {
		m.insert(make_pair(vp[i].left, i));
	}
}
// ��ȥ�����ݹ�,ȷ�����з��ս����:Non(i) ::= Non(i)Term | Non(i+1)Term | Non(i+2)Term | Non(i+3)Term .....
void PUTIL::EliminateIndirectLeftRecursion(vector<Production>& vp, int pos, map<string, int>& m) {
	vector<string> newRights;
	for (int j = 0; j < vp[pos].right.size(); j++) {
		string pro = vp[pos].right[j];
		// ��ȡ����ʽ�ĵ�һ�����ս��
		string non = "";
		int nonPos;
		if (pro[0] >= 'A' && pro[0] <= 'Z') {
			non += pro[0];
			nonPos = m[non];
		}
		else {
			nonPos = pos;
		}
		if (nonPos < pos) {// �׷��ս�������ڸ��ַ�����ǰ��,�����
			vector<string> rights = getRightsFromPro(pro);
			for (vector<string>::iterator it = vp[nonPos].right.begin(); it != vp[nonPos].right.end(); it++) {
				rights[0] = *it;
				string newPro = getProFromRights(rights);
				newRights.push_back(newPro);
			}
		}
		else {// �����֣�ֱ�Ӽ��뵽�µĲ���ʽ����
			newRights.push_back(pro);
		}
	}	
	vp[pos].right = newRights;
}
/// <summary>
/// ��ȥֱ����ݹ�,��Non(i) ::= Non(i)Term�����.E=E+T|T => E = TE' ;  E' = +TE'|@
/// </summary>
/// <param name="vp"></param>
/// <param name="pos"></param>
void PUTIL::EliminateDirectLeftRecursion(vector<Production>& vp, int pos) {
	// ��ֱ�ӿ���ݹ�Ĳ���ʽ�벻�ɵķֿ�����Ϊ[first,second),[second,size-1)
	vector<string>::iterator range = sortForEliminateDirectLeftRecursion(vp, pos);// ����
	vector<string>::iterator begin = vp[pos].right.begin();
	vector<string>::iterator end = vp[pos].right.end();
	if (range != begin) {// ����ֱ����ݹ�
		Production newPro;
		string newLeft = vp[pos].left + "'";
		newPro.left = newLeft;
		newPro.have_null = 1;
		vector<string>::iterator it = begin;
		while (it != range) {// ����E'���²���ʽ�� ���� E' = aE'|@
			string pro = *it;
			pro.erase(0, 1);
			pro += newLeft;
			newPro.right.push_back(pro) ;
			it++;
		}
		int size = range - begin;
		while (size--) {// ɾ��Դ���ս������ݹ����ʽ
			vp[pos].right.erase(vp[pos].right.begin());
		}
		it = vp[pos].right.begin();
		end = vp[pos].right.end();
		while (it != end) {// ����Դ���ս���Ĳ���ʽ ���� E = tE'
			string newRight = *it;
			newRight += newLeft;
			*it = newRight;
			it++;
		}
		vp.push_back(newPro);
	}	
}
/// <summary>
/// ���ڴ���ֱ����ݹ�����򣬴���󣺲���ʽǰ�����ǵ�ǰ���ս����ͷ��.
/// </summary>
/// <param name="vp"></param>
/// <param name="pos"></param>
/// <returns>����[ 0, pos )Ϊֱ����ݹ����ڵ�����</returns>
vector<string>::iterator PUTIL::sortForEliminateDirectLeftRecursion(vector<Production>& vp, int pos) {
	vector<string>::iterator left = vp[pos].right.begin();
	vector<string>::iterator right = vp[pos].right.begin();
	vector<string>::iterator end = vp[pos].right.end();
	string non = vp[pos].left;
	while (right != end) {
		if (right->find(non) == 0) {
			string t = *left;
			*left = *right;
			*right = t;
			left++;
		}
		right++;
	}
	return left;
}

void PUTIL::switchNon(vector<Production>& vp, string src1, string src2) {
	for (int i = 0; i < vp.size(); i++) {
		// �����滻���ַ�
		if (vp[i].left == src1) {
			vp[i].left = src2;
		}
		else if (vp[i].left == src2) {
			vp[i].left = src1;
		}
		for (int j = 0; j < vp[i].right.size(); j++) {
			string pro = vp[i].right[j];
			if (pro.find(src1) != pro.size() || pro.find(src2) != pro.size() ) {
				vector<string> vs = getRightsFromPro(pro);
				for (vector<string>::iterator it = vs.begin(); it != vs.end(); it++) {
					if (*it == src1) {
						*it = src2;
					}
					else if (*it == src2) {
						*it = src1;
					}
				}
				pro = getProFromRights(vs);
				vp[i].right[j] = pro;
			}
		}
	}
}

string PUTIL::getProFromRights(vector<string> vs) {
	string pro = "";
	for (int i = 0; i < vs.size(); i++) {
		pro += vs[i];
	}
	return pro;
}

void PUTIL::fixNumber(vector<Production>& vp) {
	for (int i = 0; i < vp.size(); i++) {
		vp[i].id = i;
	}
}
