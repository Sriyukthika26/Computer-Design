#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool is_escaped(const string& line, int pos) {
    int backslashes = 0;
    pos--;
    while (pos >= 0 && line[pos] == '\\') {
        backslashes++;
        pos--;
    }
    return (backslashes % 2 == 1); // Odd = escaped
}

int main() {
    ifstream infile("Inputs/Input_Comments.cpp");
    ofstream outfile("Outputs/NoComments.cpp");

    string line;
    bool in_block_comment = false;

    while (getline(infile, line)) {
        string result;
        bool in_string = false;

        for (size_t i = 0; i < line.size(); ++i) {
            if (in_block_comment) {
                if (i + 1 < line.size() && line[i] == '*' && line[i + 1] == '/') {
                    in_block_comment = false;
                    i++; // skip '/'
                }
                continue;
            }

            if (!in_string && i + 1 < line.size()) {
                // Start of block comment
                if (line[i] == '/' && line[i + 1] == '*') {
                    in_block_comment = true;
                    i++; // skip '*'
                    continue;
                }
                // Start of line comment
                if (line[i] == '/' && line[i + 1] == '/') {
                    break; // skip rest of the line
                }
            }

            // Toggle string literal detection
            if (line[i] == '"' && !is_escaped(line, i)) {
                in_string = !in_string;
            }

            result += line[i];
        }

        if (!in_block_comment) {
            outfile << result << endl;
        }
    }

    infile.close();
    outfile.close();

    cout << "Comments removed and written to Outputs/NoComments.cpp" << endl;
    return 0;
}
