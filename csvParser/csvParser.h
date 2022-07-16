#pragma once

#include <fstream>

#include <vector>
#include <map>
#include <algorithm>

//
using namespace std;

typedef vector<vector<string>> CSVrow;
typedef vector<string> CSVcolumn;

class CSVException : public std::exception {
public:
	CSVException(const int no, const char* msg) : exceptionNumber(no){
		strcpy_s(message, 256, msg);
	}
	char* what() {
		return message;
	}

private:
	int exceptionNumber;
	/// <summary>
	/// no	| resone
	/// -----------------------------------
	/// 1	| size error
	/// 2	| index error
	/// 3	| used ambiguous key
	/// </summary>
	char message[256];
};

class CSV {
public:
	CSV();
	~CSV();
	CSV(const string& dir);

	bool read(const string& dir);
	bool write(const string& dir);

	size_t field(const string& str);
	const vector<string> keys();
	size_t size();

	void addVertical(const vector<string>& line);
	void addHorizontal(const vector<string>& line);
	void addColumn(const CSVcolumn& column);
	void addKey(const string& key);

	void eraseVerticalLine(const size_t index);
	void eraseHorizontalLine(const size_t index);
	void eraseColumn(const size_t index);
	void eraseKey(const string& key);

	void insertVerticalLine(const size_t index, const vector<string>& line);
	void insertHorizontalLine(const size_t index, const vector<string>& line);
	void insertColumn(const size_t index, const CSVcolumn& column);
	void insertKey(const size_t index, const string& key);

public:
	vector<string>& operator [](const int index);

	CSVrow::iterator begin();
	CSVrow::iterator end();
	CSVrow::reverse_iterator rbegin();
	CSVrow::reverse_iterator rend();


private:
	void keyMapSync();
	

private:
	string dir;

	vector<vector<string>> data;
	map<string, size_t> keyMap;
};

void CSVparser(const string& str, CSV& csv);
void CSVstringify(const CSVcolumn& column, string& str);