#pragma once

#include <fstream>
#include <mutex>
#include <thread>

#include <vector>
#include <map>

//https://readystory.tistory.com/116
//https://www.interviewsansar.com/design-singleton-class-for-multithread-environment-with-explanation/
//
//
//
//
using namespace std;

class CSVinterface {
public:
	static CSVinterface* getInstance();

	~CSVinterface();
	bool read(string dir, vector<vector<string>>& container);
	bool read(string dir, vector<map<string, string>>& container);

	bool write(string dir, vector<vector<string>>& container);//
	bool write(string dir, vector<map<string, string>>& container);//

private:
	CSVinterface();
	bool read(string dir, vector<string>& datas);
	bool write(string dir, vector<string>& datas);//
	void parser(string str, vector<string>& v);
	void maker(vector<string>& v, string& str);

	string dir;
	static CSVinterface* instance;
	static mutex m;
};