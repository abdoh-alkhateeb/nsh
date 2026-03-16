#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<string> history;
    string input;

    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit") break;

        history.push_back(input);

        if (input == "history") {
            for (size_t i = 0; i < history.size(); i++) {
                cout << i + 1 << " " << history[i] << endl;
            }
        } else {
            cout << "You typed: " << input << endl;
        }
    }

    return 0;
}
