#include "csvParser.h"


#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <map>

#include <list>
#include <vector>
#include <string>
#include <deque>

#include <iostream>

using namespace std;

int main()
{
	CSVinterface* inter = CSVinterface::getInstance();

	vector<vector<string>> temp;
	inter->read("test1.csv", temp);

	for (auto v : temp)
	{
		for (auto s : v)
		{
			cout << s << "\t";
		}
		cout << endl;
	}

	cout << endl;

	vector<map<string, string>> temp2;
	inter->read("test2.csv", temp2);

	for (auto m : temp2)
	{
		for (auto s : m)
		{
			cout << "(" << s.first << ", " << s.second << ")" << "\t";
		}
		cout << endl;
	}

	inter->write("test1-copy.csv", temp);
	inter->write("test2-copy.csv", temp2);

	return 0;
}