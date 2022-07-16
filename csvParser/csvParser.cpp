#include "csvParser.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <deque>



CSV::~CSV()
{

}
CSV::CSV()
{

}
CSV::CSV(const string& dir){
	read(dir);
}

bool CSV::read(const string& _dir)
{
	dir = _dir;
	data.clear();
	keyMap.clear();

	ifstream file(dir);

	if (!file.is_open())
		return false;

	string temp;
	while (getline(file, temp))
		CSVparser(temp, *this);
	
	file.close();

	keyMapSync();

	return true;
}
bool CSV::write(const string& dir)
{
	ofstream file(dir);

	if (!file.is_open())
		return false;

	for (auto v : data)
	{
		string temp;
		CSVstringify(v, temp);

		file << temp << endl;
	}

	file.close();

	return true;
}

size_t CSV::field(const string& str){
	return keyMap[str];
}
const vector<string> CSV::keys(){
	return data[0];
}
size_t CSV::size(){
	return data.size();
}

void CSV::addVertical(const vector<string>& line) {
	if (data.empty())
		throw CSVException(2, "add vertical line to empty CSV");
	if (!data.empty() && line.size() != data.size())
		throw CSVException(1, "add vertical line size mismatch");

	for (int i = 0; i < line.size(); i++)
		data[i].push_back(line[i]);

	keyMapSync();
};
void CSV::addHorizontal(const vector<string>& line) {
	if (!data.empty() && line.size() != data.front().size())
		throw CSVException(1, "add horizontal line size mismatch");

	data.push_back(line);
};
void CSV::addColumn(const CSVcolumn& column) { 
	if (!data.empty() && column.size() != data.front().size())
		throw CSVException(1, "add column size mismatch");

	data.push_back(column);
};
void CSV::addKey(const string& key) {
	if (data.empty())
		throw CSVException(2, "add key to empty CSV");

	if (keyMap.find(key) != keyMap.end())
		keyMap[key] = SIZE_MAX;
	else
		keyMap[key] = data.front().size();
	
	data.front().push_back(key);
	for (int i = 1; i < data.size(); i++)
		data[i].push_back("");
};

void CSV::eraseVerticalLine(const size_t index) {
	if (index >= data.front().size())
		throw CSVException(2, "erase vertical line index over");

	for (auto iter = data.begin(); iter != data.end(); iter++)
		iter->erase(iter->begin() + index);

	keyMapSync();
};
void CSV::eraseHorizontalLine(const size_t index) {
	if (index >= data.size())
		throw CSVException(2, "erase horizontal line index over");

	data.erase(data.begin() + index);
};
void CSV::eraseColumn(const size_t index) {
	if(index >= data.size())
		throw CSVException(2, "erase column index over");

	data.erase(data.begin() + index);
};
void CSV::eraseKey(const string& key) {
	if (keyMap.find(key) == keyMap.end())
		throw CSVException(3, "erase not exist key");

	size_t index = keyMap[key];

	if (index == SIZE_MAX)
		throw CSVException(3, "erase ambiguous key");

	eraseVerticalLine(index);
	keyMapSync();
};

void CSV::insertVerticalLine(const size_t index, const vector<string>& line) {
	if (data.empty())
		throw CSVException(2, "insert vertical line to empty CSV");
	if (index > data.front().size())
		throw CSVException(2, "insert vertical line index over");
	if (!data.empty() && line.size() != data.size())
		throw CSVException(1, "insert vertical line size mismatch");

	for (int i = 0; i < data.size(); i++)
		data[i].insert(data[i].begin() + index, 1, line[i]);

	keyMapSync();
};
void CSV::insertHorizontalLine(const size_t index, const vector<string>& line) {
	if (index > data.size())
		throw CSVException(2, "insert hrizontal line index over");
	if (!data.empty() && line.size() != data.front().size())
		throw CSVException(1, "insert hrizontal line size mismatch");

	data.insert(data.begin() + index, line);
};
void CSV::insertColumn(const size_t index, const CSVcolumn& column) {
	if (index > data.size())
		throw CSVException(2, "insert column index over");
	if (!data.empty() && column.size() != data.front().size())
		throw CSVException(1, "insert column size mismatch");

	data.insert(data.begin() + index, column);
};
void CSV::insertKey(const size_t index, const string& key) {
	if (data.empty())
		throw CSVException(2, "insert key to empty CSV");
	if (index > data.front().size())
		throw CSVException(2, "insert key index over");

	data.front().insert(data.front().begin() + index, 1, key);
	for (int i = 1; i < data.size(); i++)
		data[i].insert(data[i].begin() + index, 1, "");

	keyMapSync();
};

vector<string>& CSV::operator [](const int keyMap)
{
	return data[keyMap];
}

CSVrow::iterator CSV::begin()
{
	return data.begin();
}
CSVrow::iterator CSV::end()
{
	return data.end();
}
CSVrow::reverse_iterator CSV::rbegin()
{
	return data.rbegin();
}
CSVrow::reverse_iterator CSV::rend()
{
	return data.rend();
}



void CSV::keyMapSync()
{
	keyMap.clear();
	
	for (int i = 0; i < data[0].size(); i++)
	{
		if (keyMap.find(data[0][i]) != keyMap.end())
			keyMap[data[0][i]] = SIZE_MAX;
		else
			keyMap[data[0][i]] = i;
	}
}

void CSVparser(const string& str, CSV& csv)
{
	string endStr = ",";
	deque<char> dq;
	CSVcolumn data;

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
					data.push_back("");
					if (it + 1 == str.end())
						data.push_back("");

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
					data.push_back(string(dq.begin(), dq.end()));
					dq.clear();

					if (it + 1 == str.end())
						data.push_back("");

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
					data.push_back(string(dq.begin(), dq.end()));
					dq.clear();
					it++;

					if (it + 1 == str.end())
						data.push_back("");
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
		data.push_back(string(dq.begin(), dq.end()));

	csv.addColumn(data);
}

void CSVstringify(const CSVcolumn& v, string& str)
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
