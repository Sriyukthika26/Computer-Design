#include <bits/stdc++.h>
using namespace std;

unordered_set<string> keywords = {
	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
	"goto",
	"if",
	"int",
	"long",
	"register",
	"return",
	"short",
	"aligned",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while"};

unordered_set<string> operators = {
	"+",
	"-",
	"*",
	"/",
	"%",
	"++",
	"--",
	"=",
	"+=",
	"-=",
	"*=",
	"/=",
	"%=",
	"==",
	"!=",
	">",
	"<",
	">=",
	"<=",
	"&&",
	"||",
	"!",
	"&",
	"|",
	"^",
	"~",
	"<<",
	">>",
	"<<=",
	">>=",
	"&=",
	"|=",
	"^="};

unordered_set<char> specialSymbols = {
	'{',
	'}',
	'[',
	']',
	'(',
	')',
	';',
	',',
	'.',
	'#'};

unordered_map<string, pair<string, string>> symbolTable;
int countTokens = 0;
int line = 0;

bool isKeyword(const string &token)
{
	return keywords.count(token);
}

bool isIdentifier(const string &token)
{
	if (token.empty())
		return false;
	if (!isalpha(token[0]) && token[0] != '_')
		return false;
	for (char c : token)
	{
		if (!isalnum(c) && c != '_')
		{
			cerr << "Lexical error at line " << line << ": unrecognized character " << c << endl;
			return false;
		}
	}
	if (isKeyword(token))
		return false;
	if (symbolTable.count(token) == 0)
	{
		symbolTable[token] = {"IDENTIFIER", "-"};
	}
	return true;
}

bool isConstant(const string &token)
{
	try
	{
		size_t pos;
		int val = stoi(token, &pos);
		if (pos == token.size())
		{
			symbolTable[token] = {"CONSTANT", token};
			return true;
		}
	}
	catch (...)
	{
	}

	try
	{
		size_t pos;
		float val = stof(token, &pos);
		if (pos == token.size())
		{
			symbolTable[token] = {"CONSTANT", token};
			return true;
		}
	}
	catch (...)
	{
	}

	if (token.size() == 3 && token.front() == '\'' && token.back() == '\'')
	{
		symbolTable[token] = {"CONSTANT", token};
		return true;
	}

	return false;
}

bool isString(const string &token)
{
	if (token.size() >= 2 && token.front() == '\"' && token.back() == '\"')
	{
		symbolTable[token] = {"STRING_LITERAL", token};
		return true;
	}
	return false;
}

bool isOperator(const string &token)
{
	return operators.count(token);
}

bool isSpecialSymbol(char c)
{
	return specialSymbols.count(c);
}

vector<string> tokenizeLine(const string& line) {
    vector<string> tokens;
    string token;
    bool inString = false;

    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];

        if (inString) {
            token += c;
            if (c == '\"') {
                tokens.push_back(token);
                token.clear();
                inString = false;
            }
            continue;
        }

        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            continue;
        }

        if (c == '\"') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            token += c;
            inString = true;
            continue;
        }

        string op;
        if (i + 2 < line.size()) {
            op = line.substr(i, 3);
            if (isOperator(op)) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(op);
                i += 2;
                continue;
            }
        }

        if (i + 1 < line.size()) {
            op = line.substr(i, 2);
            if (isOperator(op)) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(op);
                i += 1;
                continue;
            }
        }

        op = string(1, c);
        if (isOperator(op) || isSpecialSymbol(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(op);
        } else {
            token += c;

			if (isdigit(c) && i + 1 < line.size() && line[i + 1] == '.') {
                while (i + 1 < line.size() && (isdigit(line[i + 1]) || line[i + 1] == '.')) {
                    token += line[++i];
                }
            }
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

string removeCommentsAndDirectives(const string &inputLine, bool &inBlockComment)
{
	string result;
	size_t i = 0;

	if (!inBlockComment && !inputLine.empty() && inputLine[0] == '#')
	{
		return "";
	}

	while (i < inputLine.size())
	{
		if (!inBlockComment && i + 1 < inputLine.size() && inputLine[i] == '/' && inputLine[i + 1] == '/')
		{
			break;
		}
		else if (!inBlockComment && i + 1 < inputLine.size() && inputLine[i] == '/' && inputLine[i + 1] == '*')
		{
			inBlockComment = true;
			i += 2;
		}
		else if (inBlockComment && i + 1 < inputLine.size() && inputLine[i] == '*' && inputLine[i + 1] == '/')
		{
			inBlockComment = false;
			i += 2;
		}
		else if (!inBlockComment)
		{
			result += inputLine[i++];
		}
		else
		{
			i++;
		}
	}

	return result;
}

void scan(const string &fileName)
{
	ifstream inFile(fileName);
	ofstream tokensOut("OutputLexAnalysis/tokens.txt");

	if (!inFile)
	{
		cerr << "Error: cannot open file" << endl;
		exit(1);
	}

	string text;
	bool inBlockComment = false;

	while (getline(inFile, text))
	{
		++line;

		string cleanText = removeCommentsAndDirectives(text, inBlockComment);
		if (cleanText.empty()) continue;

		vector<string> tokens = tokenizeLine(cleanText);
		for (const string &tok : tokens)
		{
			if (tok.empty())
				continue;
			countTokens++;
			if (isKeyword(tok))
			{
				tokensOut << "<KEYWORD, " << tok << ">\n";
			}
			else if (isIdentifier(tok))
			{
				tokensOut << "<IDENTIFIER, " << tok << ">\n";
			}
			else if (isConstant(tok))
			{
				tokensOut << "<CONSTANT, " << tok << ">\n";
			}
			else if (isString(tok))
			{
				tokensOut << "<STRING_LITERAL, " << tok << ">\n";
			}
			else if (isOperator(tok))
			{
				tokensOut << "<OPERATOR, " << tok << ">\n";
			}
			else if (tok.size() == 1 && isSpecialSymbol(tok[0]))
			{
				tokensOut << "<SPECIAL_SYMBOL, " << tok << ">\n";
			}
			else
			{
				cerr << "Lexical Error at line " << line << ": Unknown token '" << tok << "'\n";
			}
		}
	}

	tokensOut << "Total Tokens: " << countTokens << endl;
	inFile.close();
	tokensOut.close();
}

int main()
{

	string fileName = "inputLex.c";
	scan(fileName);

	ofstream symOut("OutputLexAnalysis/symTable.txt");
	for (const auto &entry : symbolTable)
	{
		symOut << entry.first << " -> " << entry.second.first << ", " << entry.second.second << "\n";
	}
	symOut.close();

	return 0;
}