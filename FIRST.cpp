#include "FIRST.h"


RecordFirst::RecordFirst() {
	left = "";
	dependent = "";
	firstSet.clear();
}
RecordFirst::RecordFirst(int _proID) {
	proID = _proID;
	left = "";
	dependent = "";
	firstSet.clear();
}
/// @brief ����һ�������ʽ�ļ�¼
/// @param _proID 
/// @param emp 
RecordFirst::RecordFirst(int _proID, string _left) {
	proID = _proID;
	left = _left;
	dependent = "";
	firstSet.clear();
	firstSet.insert("@");
}

SymbolFirst::SymbolFirst() {
	left = "";
	firstSet.clear();
}

ProductionFirst::ProductionFirst() {
	proID = -1;
	left = "";
	firstSet.clear();
}