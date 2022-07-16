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
	CSV inter1, inter2, inter3;
	inter1.read("testFile/test1.csv");
	inter2.read("testFile/test2.csv");
	for (auto it : inter1)
	{
		for (auto item : it)
		{
			cout << item << "\t";
		}

		cout << endl;
	}
	cout << endl;

	for (int i = 0; i < inter2.size(); i++)
	{
		cout << i << ": " << inter2[i][inter2.field("name")];
		cout << endl;
	}
	cout << endl;

	inter2.eraseColumn(1);
	for (int i = 0; i < inter2.size(); i++)
	{
		cout << i << ": " << inter2[i][inter2.field("name")];
		cout << endl;
	}
	cout << endl;

	inter2.eraseColumn(inter2.size() - 1);
	for (int i = 0; i < inter2.size(); i++)
	{
		cout << i << ": " << inter2[i][inter2.field("name")];
		cout << endl;
	}
	cout << endl;

	inter1.write("testFile/test1-copy.csv");
	inter2.write("testFile/test2-copy.csv");

	return 0;
}