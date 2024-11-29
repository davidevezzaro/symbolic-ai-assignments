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
    bool flag1 = false;
    bool flag2 = false;
    vector<string>LHS;
    string RHS;
};

struct fc {
    vector<rule>rules;
    vector<string>facts;
    string goal;
    queue<string> path;
    int nFlag1 = 0;
};

int iterationCounter;
bool result = false;
bool isGoalInFacts = false;

int numberInitialFacts;
void PrintRule(fc &f,int k);

vector<string>lackingFacts;
fc fc_cyras, fc_negnevitsky;
fstream file;

void FindFacts(fc &f, int k) {//if the vector is zero means the rule is applicable
    lackingFacts.clear();
    for (int i = 0; i < f.rules[k].LHS.size(); i++) {

        lackingFacts.push_back(f.rules[k].LHS[i]);
       
        for (int j = 0; j < f.facts.size(); j++) {
            if (!lackingFacts.empty()) {
                if (lackingFacts.back().compare(f.facts[j]) == 0) {
                    lackingFacts.pop_back();
                }
            }
            
        }
    }
}

void tokenize(std::string const& str, const char delim, std::vector<std::string>& out)
{
    // construct a stream from the string 
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

void ReadFromFile(string input) {
    file.open(input);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.find("1) Rules") != std::string::npos) {//start reading the rules
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
                        tokenize(line, delim, tokens);

                        //save the right side in the data structure
                        record.RHS = tokens[0];
                        tokens.erase(tokens.begin());

                        //then save the left hand side in the data structure
                        while (!tokens.empty()) {
                            record.LHS.push_back(tokens[0]);
                            tokens.erase(tokens.begin());
                        }
                    }
                    fc_cyras.rules.push_back(record);
                    fc_negnevitsky.rules.push_back(record);
                }
            }
            else if (line.find("2) Facts") != std::string::npos) {//start reading the facts
                getline(file, line);
                const char delim = ' ';
                tokenize(line, delim, fc_cyras.facts);
                tokenize(line, delim, fc_negnevitsky.facts);
            }
            else if (line.find("3) Goal") != std::string::npos) {//read the goal
                getline(file, line);
                fc_cyras.goal = line;
                fc_negnevitsky.goal = line;
            }
        }
    }
    file.close();
}

void PrintRule(fc& f, int k) {
    cout << "\t";
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

void ForwardChainingCyras() {
    int k = 0;
    result = false;
    isGoalInFacts = false;
    bool isRHSInFacts = false;

    while (k < fc_cyras.rules.size()) {//if k is in the set of rules
        isRHSInFacts = false;

        if (k == 0) {//to print the nth iteration
            cout << endl << "\tITERATION " << iterationCounter + 1 << endl;
        }

        if (fc_cyras.rules[k].flag1 == false && fc_cyras.rules[k].flag2 == false) {//if both the flags are set to 0

            FindFacts(fc_cyras,k);//check if there's a matching rule

            for (int i = 0; i < fc_cyras.facts.size(); i++) {//check if RHS is already in the inferred facts
                if (fc_cyras.rules[k].RHS == fc_cyras.facts[i]) {
                    isRHSInFacts = true;
                }
            }
            
            if (lackingFacts.size() == 0 && isRHSInFacts==false) {//means LHS of the kth rule is a subset of facts
                fc_cyras.rules[k].flag1 = true;//apply the rule and raise flag1
                fc_cyras.facts.push_back(fc_cyras.rules[k].RHS);//then add RHS to facts
                fc_cyras.path.push("R"+ to_string(k + 1));

                PrintRule(fc_cyras,k);

                cout << " apply. Raise flag1. Facts ";
                
                for (int i = 0; i < numberInitialFacts; i++) {//print initial facts
                    cout << fc_cyras.facts[i] << ", ";
                }

                cout << "\b\b"<<" and ";

                for (int i = numberInitialFacts; i < fc_cyras.facts.size(); i++) {//print inferred facts
                    cout << fc_cyras.facts[i] << ", ";
                }
                cout << "\b\b."<<endl;

                //GOAL ACHIEVED - STOPPING CRITERIA
                if (fc_cyras.rules[k].RHS.compare(fc_cyras.goal) == 0) {
                    cout << "\t  Goal achieved." << endl;
                    result = true;
                    break;
                }
                
                //if no rule is matching then
                k = 0; //restart from the first rule
                iterationCounter++;
            }
            else{//if there's at least one lacking fact
                if (lackingFacts.size() > 0) {
                    //means not even a rule appliable
                    PrintRule(fc_cyras,k);

                    cout << " not applied, because of lacking ";
                    for (int i = 0; i < lackingFacts.size(); i++) {
                        cout << lackingFacts[i] << ", ";
                    }
                    cout << "\b\b."<<endl;
                    k++;//try with the next rule

                }else if(isRHSInFacts==true){
                    //RHS in facts so raise flag2
                    fc_cyras.rules[k].flag2 = true;
                    PrintRule(fc_cyras,k);
                    cout << " not applied, because RHS in facts. Raise flag2." << endl;
                    k++;
                }                
            }
        }
        else{
            PrintRule(fc_cyras,k);
            cout << " skip, because flag1 raised."<<endl;
            k++;//try with the next rule
        }
    } 
    if (!result) {//check is the goal is in the facts
        for (int i = 0; i < fc_cyras.facts.size(); i++) {
            if (fc_cyras.facts[i].compare(fc_cyras.goal) == 0) {
                isGoalInFacts = true;
            }
        }
    }
}

void ForwardChainingNegnevitsky() {
    int k = 0;
    iterationCounter = 0;
    result = false;
    isGoalInFacts = false;
    bool isRHSInFacts = false;
    int oldNFlag1=0;

    while (k < fc_negnevitsky.rules.size()) {
        isRHSInFacts = false;

        if (k == 0) {
            oldNFlag1 = fc_negnevitsky.nFlag1;
            iterationCounter++;
            cout << endl << "\tITERATION " << iterationCounter<< endl;    
        }

        if (fc_negnevitsky.rules[k].flag1 == false && fc_negnevitsky.rules[k].flag2 == false) {

            FindFacts(fc_negnevitsky,k);//check if the rule is appliable

            for (int i = 0; i < fc_negnevitsky.facts.size(); i++) {//check if the RHS is contained in the facts
                if (fc_negnevitsky.rules[k].RHS == fc_negnevitsky.facts[i]) {
                    isRHSInFacts = true;
                }
            }

            if (lackingFacts.size() == 0 && isRHSInFacts == false) {//means LHS is equal to a subset of facts
                fc_negnevitsky.rules[k].flag1 = true;//raise flag 1, means apply the rule
                fc_negnevitsky.facts.push_back(fc_negnevitsky.rules[k].RHS);//then add the right side to the set of facts
                fc_negnevitsky.path.push("R" + to_string(k + 1));
                fc_negnevitsky.nFlag1++;

                PrintRule(fc_negnevitsky, k);

                cout << " apply. Raise flag1. Facts ";

                //print initial facts
                for (int i = 0; i < numberInitialFacts; i++) {
                    cout << fc_negnevitsky.facts[i] << ", ";
                }

                cout << "\b\b" << " and ";
                
                //print inferred facts
                for (int i = numberInitialFacts; i < fc_negnevitsky.facts.size(); i++) {
                    cout << fc_negnevitsky.facts[i] << ", ";
                }
                cout << "\b\b." << endl;

                //GOAL ACHIEVED - STOPPING CRITERIA
                if (fc_negnevitsky.rules[k].RHS.compare(fc_negnevitsky.goal) == 0) {
                    cout << "\t  Goal achieved." << endl;
                    result = true;
                    break;
                }

                k++; //continue with the next rule
            }
            else {
                if (lackingFacts.size() > 0) {
                    //no LHS contained, so rule not appliable
                    PrintRule(fc_negnevitsky, k);

                    cout << " not applied, because of lacking ";
                    for (int i = 0; i < lackingFacts.size(); i++) {
                        cout << lackingFacts[i] << ", ";
                    }
                    cout << "\b\b." << endl;
                    k++;//try with the next rule
                }
                else if (isRHSInFacts == true) {
                    //raise flag2
                    fc_negnevitsky.rules[k].flag2 = true;
                    PrintRule(fc_negnevitsky, k);
                    cout << " not applied, because RHS in facts. Raise flag2." << endl;
                    k++;
                }
            }
        }
        else {
            PrintRule(fc_negnevitsky, k);
            cout << " skip, because flag1 raised." << endl;
            k++;//try with the next rule
        }
        
        if (k == fc_negnevitsky.rules.size()) {//if i reached the last rule then restart the iteration
            k = 0;
            if (oldNFlag1 == fc_negnevitsky.nFlag1) {//STOPPING CRITERIA
                break;
            }
        }
    }
    if (!result) {
        for (int i = 0; i < fc_negnevitsky.facts.size(); i++) {
            if (fc_negnevitsky.facts[i].compare(fc_negnevitsky.goal) == 0) {
                isGoalInFacts = true;
            }
        }
    }
}

void Test1();
void Test2();
void Test3();
void Test4();
void Test5();
void Test6();

int main()
{
    cout << "FORWARD CHAINING" << std::endl;

    cout << "USE: " << std::endl;
    cout << "Select one input: [1,2,3,4,5,6]" << endl;
    cout << "1. Test 1" << endl;
    cout << "2. Test 2" << endl;
    cout << "3. Test 3" << endl;
    cout << "4. Test 4" << endl;
    cout << "5. Test 5" << endl;
    cout << "6. Test 6" << endl;
    
    int input;
    cin >> input;

    system("CLS");

    switch (input) {
    case 1:
        Test1();
        break;
    case 2:
        Test2();
        break;
    case 3:
        Test3();
        break;
    case 4:
        Test4();
        break;
    case 5:
        Test5();
        break;
    case 6:
        Test6();
        break;
    }
}

void Test1() {
    ReadFromFile("test1.txt");
    numberInitialFacts = fc_cyras.facts.size();

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "     1)\tRules" << endl;
    for (int i = 0; i < fc_cyras.rules.size(); i++) {
        PrintRule(fc_cyras,i);
        cout << endl;
    }
    cout << "     2) Facts ";
    for (int i = 0; i < fc_cyras.facts.size(); i++) {
        cout << fc_cyras.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "     3) Goal " << fc_cyras.goal << ".";
    cout << endl;
#pragma endregion
#pragma PRINT PART 2
    cout << endl << "PART 2. Trace"<<endl;
    ForwardChainingCyras();
#pragma endregion
#pragma region PRINT PART 3
    cout << endl << "PART 3. Results" << endl;
    if (result) {
        cout << "\t1) Goal " << fc_cyras.goal << " achieved" << endl;
        cout << "\t2) Path ";
        while (!fc_cyras.path.empty()) {
            cout << fc_cyras.path.front();
            fc_cyras.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;
    }
    if (!result && !isGoalInFacts) {
        cout << "\t1) Empty path." << endl;
    }
    if (!result && isGoalInFacts) {
        cout << "\t1)Goal " << fc_cyras.goal << " in facts. Empty path." << endl;
    }
#pragma endregion

}

void Test2() {
    ReadFromFile("test2.txt");
    numberInitialFacts = fc_cyras.facts.size();

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "     1)\tRules" << endl;
    for (int i = 0; i < fc_cyras.rules.size(); i++) {
        PrintRule(fc_cyras, i);
        cout << endl;
    }
    cout << "     2) Facts ";
    for (int i = 0; i < fc_cyras.facts.size(); i++) {
        cout << fc_cyras.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "     3) Goal " << fc_cyras.goal << ".";
    cout << endl;
#pragma endregion

#pragma region PRINT PART 2
    cout << endl << "PART 2. Trace using Cyras method"<<endl;
    ForwardChainingCyras();
#pragma endregion

#pragma region PRINT PART 3
    cout << endl << "PART 3. Trace using Negnevitsky method"<<endl;
    ForwardChainingNegnevitsky();
#pragma endregion

#pragma region PRINT PART 4
    cout << endl << "PART 4. Results" << endl;
    if (result) {
        int ncyraspath, nnegnevitskypath;
        ncyraspath = fc_cyras.path.size();

        cout << "\t1) Goal " << fc_cyras.goal << " achieved" << endl;
        cout << "\t2) Cyras path ";
        while (!fc_cyras.path.empty()) {
            cout << fc_cyras.path.front();
            fc_cyras.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;

        cout << "\t3) Negnevitsky path ";
        nnegnevitskypath = fc_negnevitsky.path.size();

        while (!fc_negnevitsky.path.empty()) {
            cout << fc_negnevitsky.path.front();
            fc_negnevitsky.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;

        if (ncyraspath < nnegnevitskypath) {
            cout << "\t4) Cyras wins." << endl;
        }
        else if (ncyraspath > nnegnevitskypath) {
            cout << "\t4) Negnevitsky wins." << endl;
        }
        else {
            cout << "\t4) Draw." << endl;
        }
    }
    if (!result && !isGoalInFacts) {
        cout << "\t1) Empty path." << endl;
    }
    if (!result && isGoalInFacts) {
        cout << "\t1)Goal " << fc_cyras.goal << " in facts. Empty path." << endl;
    }
#pragma endregion

}

void Test3() {
    ReadFromFile("test3.txt");
    numberInitialFacts = fc_cyras.facts.size();

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "     1)\tRules" << endl;
    for (int i = 0; i < fc_cyras.rules.size(); i++) {
        PrintRule(fc_cyras, i);
        cout << endl;
    }
    cout << "     2) Facts ";
    for (int i = 0; i < fc_cyras.facts.size(); i++) {
        cout << fc_cyras.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "     3) Goal " << fc_cyras.goal << ".";
    cout << endl;
#pragma endregion

#pragma region PRINT PART 2
    cout << endl << "PART 2. Trace using Cyras method"<<endl;
    ForwardChainingCyras();
#pragma endregion

#pragma region PRINT PART 3
    cout << endl << "PART 3. Trace using Negnevitsky method"<<endl;
    ForwardChainingNegnevitsky();
#pragma endregion

#pragma region PRINT PART 4
    cout << endl << "PART 4. Results" << endl;
    if (result) {
        int ncyraspath, nnegnevitskypath;
        ncyraspath = fc_cyras.path.size();

        cout << "\t1) Goal " << fc_cyras.goal << " achieved" << endl;
        cout << "\t2) Cyras path ";
        while (!fc_cyras.path.empty()) {
            cout << fc_cyras.path.front();
            fc_cyras.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;

        cout << "\t3) Negnevitsky path ";
        nnegnevitskypath = fc_negnevitsky.path.size();

        while (!fc_negnevitsky.path.empty()) {
            cout << fc_negnevitsky.path.front();
            fc_negnevitsky.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;

        if (ncyraspath < nnegnevitskypath) {
            cout << "\t4) Cyras wins." << endl;
        }
        else if (ncyraspath > nnegnevitskypath) {
            cout << "\t4) Negnevitsky wins." << endl;
        }
        else {
            cout << "\t4) Draw." << endl;
        }
    }
    if (!result && !isGoalInFacts) {
        cout << "\t1) Empty path." << endl;
    }
    if (!result && isGoalInFacts) {
        cout << "\t1)Goal " << fc_cyras.goal << " in facts. Empty path." << endl;
    }
#pragma endregion

}

void Test4() {
    ReadFromFile("test4.txt");
    numberInitialFacts = fc_cyras.facts.size();

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "     1)\tRules" << endl;
    for (int i = 0; i < fc_cyras.rules.size(); i++) {
        PrintRule(fc_cyras, i);
        cout << endl;
    }
    cout << "     2) Facts ";
    for (int i = 0; i < fc_cyras.facts.size(); i++) {
        cout << fc_cyras.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "     3) Goal " << fc_cyras.goal << ".";
    cout << endl;
#pragma endregion

#pragma region PRINT PART 2
    cout << endl << "PART 2. Trace"<<endl;
    ForwardChainingCyras();
#pragma endregion

#pragma region PRINT PART 3
    cout << endl << "PART 3. Results" << endl;
    if (result) {
        cout << "\t1) Goal " << fc_cyras.goal << " achieved" << endl;
        cout << "\t2) Path ";
        while (!fc_cyras.path.empty()) {
            cout << fc_cyras.path.front();
            fc_cyras.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;
    }
    if (!result && !isGoalInFacts) {
        cout << "\t1) Empty path." << endl;
    }
    if (!result && isGoalInFacts) {
        cout << "\t1) Goal " << fc_cyras.goal << " in facts. Empty path." << endl;
    }
#pragma endregion

}

void Test5() {
    ReadFromFile("test5.txt");
    numberInitialFacts = fc_cyras.facts.size();

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "     1)\tRules" << endl;
    for (int i = 0; i < fc_cyras.rules.size(); i++) {
        PrintRule(fc_cyras, i);
        cout << endl;
    }
    cout << "     2) Facts ";
    for (int i = 0; i < fc_cyras.facts.size(); i++) {
        cout << fc_cyras.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "     3) Goal " << fc_cyras.goal << ".";
    cout << endl;
#pragma endregion

#pragma region PRINT PART 2
    cout << endl << "PART 2. Trace"<<endl;
    ForwardChainingCyras();
#pragma endregion

#pragma region PRINT PART 3
    cout << endl << "PART 3. Results" << endl;
    if (result) {

        cout << "\t1) Goal " << fc_cyras.goal << " achieved" << endl;
        cout << "\t2) Path ";
        while (!fc_cyras.path.empty()) {
            cout << fc_cyras.path.front();
            fc_cyras.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;
    }
    if (!result && !isGoalInFacts) {
        cout << "\t1) Empty path." << endl;
    }
    if (!result && isGoalInFacts) {
        cout << "\t1)Goal " << fc_cyras.goal << " in facts. Empty path." << endl;
    }
#pragma endregion

}

/*void Test6() {
    ReadFromFile("test6.txt");
    numberInitialFacts = fc_negnevitsky.facts.size();

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "     1)\tRules" << endl;
    for (int i = 0; i < fc_negnevitsky.rules.size(); i++) {
        PrintRule(fc_negnevitsky, i);
        cout << endl;
    }
    cout << "     2) Facts ";
    for (int i = 0; i < fc_negnevitsky.facts.size(); i++) {
        cout << fc_negnevitsky.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "     3) Goal " << fc_negnevitsky.goal << ".";
    cout << endl;
#pragma endregion

#pragma region PRINT PART 2
    cout << endl << "PART 2. Trace using Negnevitsky method";
    ForwardChainingNegnevitsky();
#pragma endregion

#pragma region PRINT PART 3
    cout << endl << "PART 3. Results" << endl;
    if (result) {

        cout << "\t1) Goal " << fc_negnevitsky.goal << " achieved" << endl;
        cout << "\t2) Path ";
        while (!fc_negnevitsky.path.empty()) {
            cout << fc_negnevitsky.path.front();
            fc_negnevitsky.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;
    }
    if (!result && !isGoalInFacts) {
        cout << "\t1) Empty path." << endl;
    }
    if (!result && isGoalInFacts) {
        cout << "\t1)Goal " << fc_negnevitsky.goal << " in facts. Empty path." << endl;
    }
#pragma endregion

}*/

void Test6() {
    ReadFromFile("test6.txt");
    numberInitialFacts = fc_cyras.facts.size();

#pragma region PRINT PART 1
    cout << "PART 1. Data" << endl << endl;
    cout << "     1)\tRules" << endl;
    for (int i = 0; i < fc_cyras.rules.size(); i++) {
        PrintRule(fc_cyras, i);
        cout << endl;
    }
    cout << "     2) Facts ";
    for (int i = 0; i < fc_cyras.facts.size(); i++) {
        cout << fc_cyras.facts[i] << ", ";
    }
    cout << "\b\b.";
    cout << endl;
    cout << "     3) Goal " << fc_cyras.goal << ".";
    cout << endl;
#pragma endregion

#pragma region PRINT PART 2
    cout << endl << "PART 2. Trace using Cyras method"<<endl;
    ForwardChainingCyras();
#pragma endregion

#pragma region PRINT PART 3
    cout << endl << "PART 3. Trace using Negnevitsky method"<<endl;
    ForwardChainingNegnevitsky();
#pragma endregion

#pragma region PRINT PART 4
    cout << endl << "PART 4. Results" << endl;
    if (result) {
        
        cout << "\t1) Goal " << fc_cyras.goal << " achieved" << endl;
        cout << "\t2) Cyras path ";
        while (!fc_cyras.path.empty()) {
            cout << fc_cyras.path.front();
            fc_cyras.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;

        cout << "\t3) Negnevitsky path ";
        
        while (!fc_negnevitsky.path.empty()) {
            cout << fc_negnevitsky.path.front();
            fc_negnevitsky.path.pop();
            cout << ", ";
        }
        cout << "\b\b." << endl;

    }
    if (!result && !isGoalInFacts) {
        cout << "\t1) Empty path." << endl;
    }
    if (!result && isGoalInFacts) {
        cout << "\t1)Goal " << fc_cyras.goal << " in facts. Empty path." << endl;
    }
#pragma endregion

}

