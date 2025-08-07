#include <iostream>
#include <fstream>
#include <stack>
#include <unordered_set>
#include <string>
#include <sstream>
using namespace std;

bool isMacroDefined(const unordered_set<string>& macros, const string& macro) {
    return macros.find(macro) != macros.end();
}

int main() {
    ifstream inFile("Inputs/input_conditional.c");
    ofstream outFile("Outputs/Output_Conditional.c");

    unordered_set<string> definedMacros;
    stack<bool> includeStack;
    bool shouldInclude = true;

    string line;
    while (getline(inFile, line)) {
        string trimmed = line;
        // Remove leading whitespace
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));

        if (trimmed.find("#define") == 0) {
            stringstream ss(trimmed);
            string directive, macro;
            ss >> directive >> macro;
            definedMacros.insert(macro);
            // Don't write this to output file
            continue;
        }

        else if (trimmed.find("#ifdef") == 0) {
            string macro = trimmed.substr(7); // after "#ifdef "
            macro.erase(0, macro.find_first_not_of(" \t"));
            bool condition = isMacroDefined(definedMacros, macro);
            includeStack.push(shouldInclude);
            shouldInclude = shouldInclude && condition;
            continue;
        }

        else if (trimmed.find("#ifndef") == 0) {
            string macro = trimmed.substr(8); // after "#ifndef "
            macro.erase(0, macro.find_first_not_of(" \t"));
            bool condition = !isMacroDefined(definedMacros, macro);
            includeStack.push(shouldInclude);
            shouldInclude = shouldInclude && condition;
            continue;
        }

        else if (trimmed.find("#else") == 0) {
            if (!includeStack.empty()) {
                bool prev = includeStack.top();
                shouldInclude = prev && !shouldInclude;
            }
            continue;
        }

        else if (trimmed.find("#endif") == 0) {
            if (!includeStack.empty()) {
                shouldInclude = includeStack.top();
                includeStack.pop();
            }
            continue;
        }

        // Write only active code
        if (shouldInclude)
            outFile << line << endl;
    }

    inFile.close();
    outFile.close();

    cout << "Preprocessing complete. Check output.c." << endl;

    return 0;
}
