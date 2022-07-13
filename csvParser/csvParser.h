#pragma once

#include <fstream>

#include <vector>
#include <map>
#include <algorithm>

//
using namespace std;

class CSV {
public:
	CSV();
	~CSV();
	CSV(string dir);

	bool read(string dir);
	bool write(string dir);

	int field(string str);
	const vector<string> keys();

	size_t size();


public:
	vector<string>& operator [](const int index);

	vector<vector<string>>::iterator begin();
	vector<vector<string>>::iterator end();
	vector<vector<string>>::reverse_iterator rbegin();
	vector<vector<string>>::reverse_iterator rend();


private:

	void parser(string str);
	void maker(vector<string>& v, string& str);

public:
	string dir;
	vector<vector<string>> data;

private:
	map<string, int> index;
};