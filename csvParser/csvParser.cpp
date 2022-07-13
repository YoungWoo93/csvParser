#include "csvParser.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

#include <deque>

CSVinterface* CSVinterface::instance = nullptr;
mutex CSVinterface::m;


CSVinterface* CSVinterface::getInstance()
{
	scoped_lock<mutex> lock(m);

	if (instance == nullptr)
		instance = new CSVinterface();

	return instance;
}


CSVinterface::~CSVinterface()
{

}

CSVinterface::CSVinterface()
{

}

bool CSVinterface::read(string dir, vector<string>& datas)
{
	scoped_lock<mutex> lock(m);
	ifstream file(dir);

	if (!file.is_open())
		return false;

	string temp;
	while (getline(file, temp)) 
		datas.push_back(temp);
	
	file.close();

	return true;
}

bool CSVinterface::read(string dir, vector<vector<string>>& container)
{
	for (auto& v : container)
		v.clear();
	container.clear();

	vector<string> buffer;
	if (!read(dir, buffer))
		return false;


	for (auto str : buffer)
	{
		container.push_back(vector<string>());
		parser(str, container.back());
	}

	return true;
}

bool CSVinterface::read(string dir, vector<map<string, string>>& container)
{
	for (auto& m : container)
		m.clear();
	container.clear();

	vector<string> buffer;
	if (!read(dir, buffer))
		return false;

	vector<string> keys;
	parser(buffer.front(), keys);
	
	for (auto it = buffer.begin()+1; it != buffer.end(); it++)
	{
		vector<string> values;
		parser(*it, values);

		for (int i = 0; i < keys.size() - values.size(); i++)
			values.push_back("");

		container.push_back(map<string, string>());
		for (int i = 0; i < keys.size(); i++)
			container.back()[keys[i]] = values[i];
		
	}


	return true;
}



bool CSVinterface::write(string dir, vector<string>& datas)
{
	scoped_lock<mutex> lock(m);
	ofstream file(dir);

	if (!file.is_open())
		return false;

	for (auto str : datas)
	{
		file << str << endl;
	}

	file.close();

	return true;
}

bool CSVinterface::write(string dir, vector<vector<string>>& container)
{
	vector<string> buffer;

	for (auto v : container)
	{
		string temp;
		maker(v, temp);

		buffer.push_back(temp);
	}

	if (!write(dir, buffer))
		return false;
	

	return true;
}

bool CSVinterface::write(string dir, vector<map<string, string>>& container)
{
	vector<string> buffer;

	vector<string> keys;
	for (auto pairs : container.front())
	{
		keys.push_back(pairs.first);
	}
	string temp;
	maker(keys, temp);

	buffer.push_back(temp);

	for (auto m : container)
	{
		vector<string> values;
		for (auto pairs : m)
			values.push_back(pairs.second);

		temp.clear();
		maker(values, temp);

		buffer.push_back(temp);
	}

	if (!write(dir, buffer))
		return false;


	return true;
}


void CSVinterface::parser(string str, vector<string>& v)
{
	string endStr = ",";
	deque<char> dq;

	for (auto it = str.begin(); it < str.end(); it++)
	{
		if (dq.empty())
		{
			if (*it == '"')
				endStr = "\",";
			else
				endStr = ",";

			dq.push_back(*it);
		}
		else
		{
			if (endStr == ",")
			{
				if (it == str.end())
				{
					v.push_back(string(dq.begin(), dq.end()));
					dq.clear();
					break;
				}
				else if (!str.compare(it - str.begin(), endStr.length(), endStr))
				{
					v.push_back(string(dq.begin(), dq.end()));
					dq.clear();

					if (it + 1 == str.end())
					{
						v.push_back("");
					}
					else if (*(it + 1) == ',')
					{
						v.push_back("");
						it++;
					}
				}
				else
				{
					dq.push_back(*it);
				}
			}
			else
			{
				if (it >= str.end() - 1)
				{
					dq.pop_front();
					v.push_back(string(dq.begin(), dq.end()));
					dq.clear();
					break;
				}
				else if (!str.compare(it - str.begin(), endStr.length(), endStr))
				{
					dq.pop_front();
					v.push_back(string(dq.begin(), dq.end()));
					dq.clear();
					it++;
				}
				else if (!str.compare(it - str.begin(), 2, "\"\""))
				{
					dq.push_back(*it);
					it++;
				}
				else
				{
					dq.push_back(*it);
				}
			}
		}
	}

	if(!dq.empty())
		v.push_back(string(dq.begin(), dq.end()));
}

void CSVinterface::maker(vector<string>& v, string& str)
{
	for (auto _str : v)
	{
		if (_str.find(',') != string::npos || _str.find('\"') != string::npos)
		{
			str += '\"';
			for (auto character : _str)
			{
				if (character == '\"')
					str += '\"';

				str += character;
			}
			str += '\"';
			str += ",";
		}
		else
		{
			str += _str;
			str += ",";
		}
	}

	str.pop_back();
}