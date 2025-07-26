#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <regex>

using namespace std;

// A struct to hold detailed information about a macro
struct MacroDefinition {
    string name;
    vector<string> params;
    string body;
    bool isFunctionLike = false;
};

// Helper function to trim whitespace from both ends of a string
string trim(const string& str) {
    return regex_replace(str, regex("^ +| +$"), "");
}

int main() {
    ifstream infile("Inputs/Input_Macros.cpp");
    ofstream outfile("Outputs/Macros_Removed.cpp");

    unordered_map<string, MacroDefinition> macros;
    vector<string> codeLines;

    string line;
    while (getline(infile, line)) {
        string trimmedLine = trim(line);

        // Detect and parse #define directives
        if (trimmedLine.rfind("#define", 0) == 0) {
            stringstream ss(trimmedLine.substr(7)); // Skip "#define "
            string key, body;
            
            ss >> key;
            getline(ss, body);
            body = trim(body);

            MacroDefinition def;
            def.body = body;

            size_t openParen = key.find('(');
            if (openParen != string::npos) {
                // It's a function-like macro, e.g., SQUARE(x)
                def.isFunctionLike = true;
                def.name = key.substr(0, openParen);

                size_t closeParen = key.find(')');
                string paramsStr = key.substr(openParen + 1, closeParen - openParen - 1);
                
                stringstream pss(paramsStr);
                string param;
                while(getline(pss, param, ',')) {
                    def.params.push_back(trim(param));
                }
            } else {
                // It's an object-like macro, e.g., MAX
                def.isFunctionLike = false;
                def.name = key;
            }
            
            if (!def.name.empty() && !def.body.empty()) {
                macros[def.name] = def;
            }
            continue; // Skip adding the #define line to output
        }

        // All other lines are added to the codeLines vector
        codeLines.push_back(line);
    }

    // Process the code lines to substitute macros
    for (string &codeLine : codeLines) {
        for (const auto &pair : macros) {
            const auto& macro = pair.second;
            
            if (macro.isFunctionLike) {
                // Handle function-like macros, e.g., SQUARE(i)
                regex pattern("\\b" + macro.name + "\\s*\\(([^)]*)\\)");
                smatch match;

                while (regex_search(codeLine, match, pattern)) {
                    string passed_args_str = match[1].str();
                    string replacement_body = macro.body;
                    replacement_body = regex_replace(replacement_body, regex("\\b" + macro.params[0] + "\\b"), passed_args_str);
                    codeLine.replace(match.position(0), match.length(0), replacement_body);
                }

            } else {
                // Handle object-like macros, e.g., MAX
                codeLine = regex_replace(codeLine, regex("\\b" + macro.name + "\\b"), macro.body);
            }
        }
        outfile << codeLine << endl;
    }

    infile.close();
    outfile.close();
    return 0;
}