#include "UI.h"


void UI::readFunction(vector<string>& vs, string fileName) {
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, fileName.c_str(), "r");
	char line[200];
	while (fgets(line, sizeof(line), fp)) {
		string str = line;
		if (line[0] != '\n') {
			vs.push_back(str);
		}
	}
	fclose(fp);
}
pair<int, int> UI::select(vector< Grammar >& grammar, vector<string>& vs) {
	cout << "��ȡ�������ķ����£�" << endl;
	for (int i = 0; i < grammar.size(); i++) {
		cout << "********** �ķ�" << i << ": " << " L(" << grammar[i].start << ") ���£� ***********" << endl;
		PUTIL::printGrammar(grammar[i]);
		cout << endl;
	}
	cout << "Ŀǰ֧�ֵĹ������£�" << endl;
	for (int i = 0; i < vs.size(); i++) {
		cout << i << "��" + vs[i];
	}
	cout << endl<<endl;
	cout << "**************��������Ҫ�Ĳ��������ʽΪ���ķ���� ���ܱ�š�***************" << endl;
	cout << "**************              ���� -1 �˳�����                 ***************" << endl;
	int number1, number2;
	cin >> number1;
	if (number1 != -1) {
		cin >> number2;
	}
	return make_pair(number1, number2);
}