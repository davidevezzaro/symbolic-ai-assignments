#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <iostream>
#include <stack>
#include <sstream>
#include <queue>

using namespace std;

struct rule {
    vector<string>LHS;
    string RHS;
    bool available = true;
};

struct bc {
    vector<rule>rules;
    vector<string>facts;
    int numberInitialFacts = 0;
    string goal;
    vector<int> path;
};

int i =0;
int l = 0;

void PrintRule(bc& f, int k);
bool BackwardChaining(string goal, vector<string>& stack);

bc bc_run;
fstream file;

void PrintDepth(int n);

void PrintRule(bc& f, int k) {
    cout << "\t   ";
    string temp = "  R" + to_string(k + 1) + ":";
    cout << right << setw(5) << temp;
    cout << " ";
    for (int j = 0; j < f.rules[k].LHS.size(); j++) {
        cout << f.rules[k].LHS[j] << ", ";
    }
    cout << "\b\b";
    cout << " -> ";
    cout << f.rules[k].RHS;
}

void Tokenize(string const& str, const char delim, vector<string>& out)
{
    // construct a stream from the string 
    stringstream ss(str);

    string s;
    while (getline(ss, s, delim)) {
        out.push_back(s);
    }
}

void ReadFromFile(string input) {
    file.open(input);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.find("1) Rules") != string::npos) {//start reading the rules
                while (getline(file, line)) {
                    rule record;
                    if (line == "") {
                        break;
                    }
                    else {
                        //to remove the comments
                        line = line.substr(0, line.find("\t"));

                        //extract the rules from the string
                        const char delim = ' ';
                        vector<string> tokens;
                        Tokenize(line, delim, tokens);

                        //save the right side in the data structure
                        record.RHS = tokens[0];
                        tokens.erase(tokens.begin());

                        //then save the left hand side in the data structure
                        while (!tokens.empty()) {
                            record.LHS.push_back(tokens[0]);
                            tokens.erase(tokens.begin());
                        }
                    }
                    bc_run.rules.push_back(record);
                }
            }
            else if (line.find("2) Facts") != string::npos) {//start reading the facts
                getline(file, line);
                const char delim = ' ';
                Tokenize(line, delim, bc_run.facts);
            }
            else if (line.find("3) Goal") != string::npos) {//read the goal
                getline(file, line);
                bc_run.goal = line;
            }
        }
    }
    bc_run.numberInitialFacts= bc_run.facts.size();
    file.close();
}

bool BackwardChaining(string goal, vector<string>& stack) {
    if (find(bc_run.facts.begin(), bc_run.facts.end(), goal) != bc_run.facts.end()) {//if a goal is found in the vector
        
        i++;

        string temp = to_string(i) + ") ";
        cout << "\n\t";
        cout << right << setw(5) << temp;
        PrintDepth(l);
        cout << "Goal " << goal << ". ";

        cout << "Fact (";
        if (find(bc_run.facts.begin(), bc_run.facts.begin() + bc_run.numberInitialFacts, goal) != bc_run.facts.begin() + bc_run.numberInitialFacts) {// if it's an initial fact
            cout << "initial), as facts are ";
            for (int j = 0; j < bc_run.numberInitialFacts; j++) {
                cout << bc_run.facts[j] << ", ";
            }
            cout << "\b\b. ";
        }else if (find(stack.begin(), stack.end(), goal) != stack.end()) {//if it is in the stack
            cout << "presently inferred). Facts ";
            for (int j = 0; j < bc_run.numberInitialFacts; j++) {
                cout << bc_run.facts[j] << ", ";
            }
            cout << "\b\b ";
            cout << "and ";
            for (int j = bc_run.numberInitialFacts; j < bc_run.facts.size(); j++) {
                cout << bc_run.facts[j] << ", ";
            }
            cout << "\b\b. ";
        }else {//otherwise means earlier inferred 
            cout << "earlier inferred), as facts ";
            for (int j = 0; j < bc_run.numberInitialFacts; j++) {
                cout << bc_run.facts[j] << ", ";
            }
            cout << "and ";
            for (int j = bc_run.numberInitialFacts; j < bc_run.facts.size(); j++) {
                cout << bc_run.facts[j] << ", ";
            }
            cout << "\b\b. ";
        }
        return true;
    }
    else if (find(stack.begin(), stack.end(), goal) != stack.end()) {//if the current goal is already in the stack
        
        i++;

        string temp = to_string(i) + ") ";
        cout << "\n\t";
        cout << right << setw(5) << temp;
        PrintDepth(l);
        cout << "Goal " << goal << ". ";
        cout << "Cycle. ";

        l--;

        return false;
    }
    
    vector<int> matches;
    for (int i = 0; i < bc_run.rules.size(); i++) {
        if (bc_run.rules[i].RHS == goal) {//find a matching rule
            matches.push_back(i);
        }
    }

    if (matches.empty()) {//if no matching rule
        i++;
        string temp = to_string(i) + ") ";
        cout << "\n\t";
        cout << right << setw(5) << temp;
        PrintDepth(l);
        cout << "Goal " << goal << ". ";

        cout << "No rules. ";
        l--;
        return false;
    }
    
    bool matched = true;
    for (int m : matches) {
        i++;
        bool fail = false;
        rule extractedRule = bc_run.rules[m];

        string temp = to_string(i) + ") ";
        cout << "\n\t";
        cout << right << setw(5) << temp;
        PrintDepth(l);
        cout << "Goal " << goal << ". ";

        cout << "Find R" << m + 1 << ": ";
        for (int j = 0; j < extractedRule.LHS.size(); j++) {
            cout << extractedRule.LHS[j] << ",";
        }
        cout << "\b";
        cout << " -> " << extractedRule.RHS << ". ";
        cout << "New goals ";
        for (int j = 0; j < extractedRule.LHS.size(); j++) {
            cout << extractedRule.LHS[j] << ", ";
        }
        cout << "\b\b.";
        l++;

        for (int j = 0; j < extractedRule.LHS.size(); j++) {//push the lhs in the stack
            stack.push_back(goal);
            bool found = BackwardChaining(extractedRule.LHS[j], stack);
            stack.pop_back();
            if (found) {
                continue;
            }
            fail = true;
            break;
        }

        cout << "Back, ";
        if (fail) {
            bc_run.facts.erase(bc_run.facts.begin()+bc_run.numberInitialFacts, bc_run.facts.end());
            bc_run.path.clear();

            cout << "FAIL. ";
            matched = false;
            continue;
        }
        else {
            cout << "OK. ";
            bc_run.facts.push_back(goal);
            bc_run.path.push_back(m);
            stack.push_back(goal);

            matched = true;
            break;
        }
    }
    if (!matched) {
        i++;
       
        string temp = to_string(i) + ") ";
        cout << "\n\t";
        cout << right << setw(5) << temp;
        PrintDepth(l);
        cout << "Goal " << goal << ". ";

        cout << "No more rules. ";
        l--;
        return false;
    }
    l--;
    return BackwardChaining(goal, stack);
}



int main()
{
    
    cout << "BACKWARD CHAINING" << endl;

    cout << "USE: " << endl;
    cout << "Select one input: [1,2,3,4,5,6,7,8,9,10,11]" << endl;
    cout << " 1. Test 1" << endl;
    cout << " 2. Test 2" << endl;
    cout << " 3. Test 3" << endl;
    cout << " 4. Test 4" << endl;
    cout << " 5. Test 5" << endl;
    cout << " 6. Test 6" << endl;
    cout << " 7. Test 7" << endl;
    cout << " 8. Test 8" << endl;
    cout << " 9. Test 9" << endl;
    cout << "10. Test 10" << endl;
    cout << "11. Test 11" << endl;
       
    int input;
    cin >> input;

    system("CLS");

    switch (input) {
    case 1:
        ReadFromFile("test1.txt");
        break;
    case 2:
        ReadFromFile("test2.txt");
        break;
    case 3:
        ReadFromFile("test3.txt");
        break;
    case 4:
        ReadFromFile("test4.txt");
        break;
    case 5:
        ReadFromFile("test5.txt");
        break;
    case 6:
        ReadFromFile("test6.txt");
        break;
    case 7:
        ReadFromFile("test7.txt");
        break;
    case 8:
        ReadFromFile("test8.txt");
        break;
    case 9:
        ReadFromFile("test9.txt");
        break;
    case 10:
        ReadFromFile("test10.txt");
        break;
    case 11:
        ReadFromFile("test11.txt");
        break;
    }

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "\t  1) Rules" << endl;
    for (int i = 0; i < bc_run.rules.size(); i++) {
        PrintRule(bc_run, i);
        cout << endl;
    }
    cout << "\t  2) Facts ";
    for (int i = 0; i < bc_run.facts.size(); i++) {
        cout << bc_run.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "\t  3) Goal " << bc_run.goal << ".";
    cout << endl<<endl;
#pragma endregion

#pragma region PRINT PART 2
    cout << "PART 2. Trace" << endl;
    
    vector<string>stack;
    BackwardChaining(bc_run.goal,stack);
    
    if (bc_run.path.size() != 0) {
        cout << "OK." << endl << endl;
    }
    else {
        cout << endl << endl;
    }
#pragma endregion

#pragma region PRINT PART 3

    cout << "PART 3. Results"<<endl<<endl;
    if (bc_run.path.size() != 0) {//if a goal is achieved
        cout << "\t  1) Goal " << bc_run.goal << " achieved." << endl;
        cout << "\t  2) Path: ";
        for (int i = 0; i < bc_run.path.size(); i++) {
            cout << "R" << bc_run.path[i] + 1 << ", ";
        }
        cout << "\b\b." << endl;
    }
    else {
        if (i == 1) {
            cout << "\t  1) Goal " << bc_run.goal << " among facts. Empty path"<<endl;
        }
        else {
            cout << "\t  1) No path" << endl;
        }
    }
    
#pragma endregion
    
}

void PrintDepth(int n) {
    for (int i = 0; i < n; i++) {
        cout << "-";
    }
}
