#include "csvParser.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

#include <deque>



CSV::~CSV()
{

}
CSV::CSV()
{

}
CSV::CSV(string dir)
{
	read(dir);
}

bool CSV::read(string _dir)
{
	dir = _dir;
	data.clear();
	index.clear();

	ifstream file(dir);

	if (!file.is_open())
		return false;

	string temp;
	while (getline(file, temp))
		parser(temp);
	
	file.close();

	for (int i = 0; i < data[0].size(); i++)
	{
		if (index.find(data[0][i]) != index.end())
		{
			int count = 1;
			string temp = data[0][i] + "(" + to_string(count++) + ")";

			while(index.find(temp) != index.end())
				temp = data[0][i] + "(" + to_string(count++) + ")";

			index[temp] = i;
		}
		else
			index[data[0][i]] = i;
	}

	return true;
}
bool CSV::write(string dir)
{
	ofstream file(dir);

	if (!file.is_open())
		return false;

	for (auto v : data)
	{
		string temp;
		maker(v, temp);

		file << temp << endl;
	}

	file.close();

	return true;
}

int CSV::field(string str)
{
	return index[str];
}
const vector<string> CSV::keys()
{
	vector<pair<string, int>> keyList(index.begin(), index.end());
	sort(keyList.begin(), keyList.end(), [](const pair<string, int>& a, const pair<string, int>& b)
		{
			return a.second < b.second;
		}
	);

	vector<string> ret;
	for (auto k : keyList)
		ret.push_back(k.first);

	return ret;
}


size_t CSV::size()
{
	return data.size();
}



vector<string>& CSV::operator [](const int index)
{
	return data[index];
}

vector<vector<string>>::iterator CSV::begin()
{
	return data.begin();
}
vector<vector<string>>::iterator CSV::end()
{
	return data.end();
}
vector<vector<string>>::reverse_iterator CSV::rbegin()
{
	return data.rbegin();
}
vector<vector<string>>::reverse_iterator CSV::rend()
{
	return data.rend();
}





void CSV::parser(string str)
{
	string endStr = ",";
	deque<char> dq;
	data.push_back(vector<string>());

	for (auto it = str.begin(); it < str.end(); it++)
	{
		if (dq.empty())
		{
			if (*it == '"')
				endStr = "\",";
			else
				endStr = ",";

			if (endStr == ",")
			{
				if (!str.compare(it - str.begin(), endStr.length(), endStr))
				{
					data.back().push_back("");
					if (it + 1 == str.end())
						data.back().push_back("");

					continue;
				}
			}

			dq.push_back(*it);
		}
		else
		{
			if (endStr == ",")
			{
				if (!str.compare(it - str.begin(), endStr.length(), endStr))
				{
					data.back().push_back(string(dq.begin(), dq.end()));
					dq.clear();

					if (it + 1 == str.end())
						data.back().push_back("");

				}
				else
				{
					dq.push_back(*it);
				}
			}
			else
			{
				if (!str.compare(it - str.begin(), endStr.length(), endStr))
				{
					dq.pop_front();
					data.back().push_back(string(dq.begin(), dq.end()));
					dq.clear();
					it++;

					if (it + 1 == str.end())
						data.back().push_back("");
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

	if (!dq.empty())
		data.back().push_back(string(dq.begin(), dq.end()));
}
void CSV::maker(vector<string>& v, string& str)
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
