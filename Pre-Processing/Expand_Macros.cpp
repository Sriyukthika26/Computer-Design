#include <bits/stdc++.h>
using namespace std;

int main()
{
	string line;
	
	ifstream infile("Inputs/Input_Macros.cpp");
	ofstream outfile("Outputs/Macros_Removed.cpp");
	
	unordered_map <string , string> macros;
	vector<string> codeLines;
	
	while(getline(infile, line))
	{	
		int i = 0;
		while(i<line.length() && isspace(line[i])) i++;
		if(i+1 < line.length() && line[i] == '#' && line[i+1] == 'd')
		{
			istringstream iss(line.substr(i));
			string directive;
			iss>> directive;
			
			if(directive == "#define")
			{
				string key, value;
				iss>>key;
				getline(iss, value);
				value = regex_replace(value, regex("^ +| +$"), "");
				if(!key.empty() && !value.empty())
				{
					macros[key] = value;
				}
			}
			continue;
		}
		codeLines.push_back(line);
	}
	
	for(string &codeLine : codeLines)
	{
		for(const auto &macro : macros)
		{
			codeLine = regex_replace(codeLine, regex("\\b"+ macro.first + "\\b"), macro.second);
		}
		outfile<<codeLine<<endl;
	}
	
	infile.close();
	outfile.close();
	return 0;
}
