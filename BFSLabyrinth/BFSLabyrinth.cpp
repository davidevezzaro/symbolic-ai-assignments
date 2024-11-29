#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <iostream>
#include <stack>

using namespace std;

int M; // The dimensions of the labyrinth.
int N;
int MN; // The number of cells M*N.

vector<vector<int>>LAB, LABCOPY;
vector<int>FX, FY;
stack<string>rules;
stack<string>nodes;
fstream file;
ofstream out;

void ReadFromFile(string input);

string s;

int L = 2;
int close_counter = 1;

int CX[5]; // 4 production rules.
int CY[5];

int CLOSE; // The counter for a closed node.
int NEWN; // The counter for an opened node.
int K; // The counter for a production.
int X, Y; // The start position of the agent.
int U, V, I, J;
bool YES;
int rules_counter = 0;

int ruleMapper(int k) {
    switch (k)
    {
    case 1:return 3;
        break;
    case 2:
        return 4;
        break;
    case 3:
        return 1;
        break;
    case 4:
        return 2;
        break;
    default:
        return -1;
    }
}

void ReadFromFile(string input) {
    int r = 0;
    file.open(input);
    string word;

    if (file.is_open()) {
        LAB.push_back(std::vector<int>());//dummy row
        LAB.push_back(std::vector<int>());//real first row

        LABCOPY.push_back(std::vector<int>());//dummy row
        LABCOPY.push_back(std::vector<int>());//real first row

        r++;

        LAB[r].push_back(-2);//dummy column
        LABCOPY[r].push_back(-2);//dummy column

        while (file >> word) {
            if (!word.find("Y=")) {
                //new row added
                LAB.push_back(std::vector<int>());
                LABCOPY.push_back(std::vector<int>());

                r++;

                LAB[r].push_back(-2);//dummy column
                LABCOPY[r].push_back(-2);//dummy column
            }
            else if (!word.find("Line")) {}
            else if (!word.find("Start")) {
                //remove the last row added
                Y = LAB[r].back();
                LAB[r].pop_back();
                LABCOPY[r].pop_back();

                X = LAB[r].back();
                LAB[r].pop_back();
                LABCOPY[r].pop_back();
                
                LAB.pop_back();
                LABCOPY.pop_back();

                break;
            }
            else {
                LAB[r].push_back(stoi(word));
                LABCOPY[r].push_back(stoi(word));
            }
        }
    }
    file.close();
    M = LAB[1].size() - 1;//number of columns
    N = LAB.size() - 1;//number of rows

    //to fill the support array
    for (int i = 0;i <= M * N;i++) {
        FX.push_back(-1);
        FY.push_back(-1);
    }

    cout << "";
}

void BACK(int U, int V) {
    // Collect the path from the exit to start.
    // INPUT: 1) U, V - the coordinates of the exit, and 2) global LABCOPY.
    // OUTPUT: LAB.
    int K, UU, VV;
    LAB[V][U] = LABCOPY[V][U]; // The exit position is marked.
    //nodes
    string node_to_add = "[X=" + to_string(U) + ",Y=" + to_string(V) + "]";
    nodes.push(node_to_add);

    K = 5;
    do {
        // The search within 4 productions. Search for cell LABCOPY[UU,VV]
        // with the mark which is 1 less than LABCOPY[U,V].
        K = K - 1;
        if (K > 0) {
            UU = U + CX[K];
            VV = V + CY[K];
            if (1 <= UU && UU <= M && 1 <= VV && VV <= N) {
                // Inside the boarders.
                if (LABCOPY[VV][UU] == LABCOPY[V][U] - 1) {
                    LAB[VV][UU] = LABCOPY[VV][UU]; // Marking a cell in LAB.
                    U = UU;
                    V = VV;
                    // Swapping the variables.

                    //list of rules
                    string rule_to_add = "R" + to_string(ruleMapper(K));
                    rules.push(rule_to_add);
                    //rules.push_back(rule_to_add);

                    //nodes
                    node_to_add = "[X=" + to_string(U) + ",Y=" + to_string(V) + "]";
                    nodes.push(node_to_add);
                }
            }
        }
        else {
            K = 5; //reset the counter
        }
    } while (LABCOPY[V][U] != 2);
}

int main() {

    cout << "BFS LABYRINTH" << std::endl;

    cout << "USE: " << std::endl;
    cout << "Select one input: [1,2,3]" <<endl;
    cout << "1. 7x7 labyrinth"<<endl;
    cout << "2. 17x9 labyrinth"<<endl;
    cout << "3. 20x15 labyrinth" << endl;
    int input;
    cin >> input;

    system("CLS");

    // 1. Reading the labyrinth.
    switch (input) {
    case 1:
        ReadFromFile("7x7.txt");
        break;
    case 2:
        ReadFromFile("17x9.txt");
        break;
    case 3:
        ReadFromFile("20x15.txt");
        break;
    }

    out = ofstream("out.txt");

    // 2. Reading the starting position of the agent and set the starting point
    LABCOPY[Y][X] = 2;
    L = LABCOPY[Y][X];

    // 3. Initialising 4 production rules.
#pragma region Forming four production rules
    CX[1] = -1; //{Go West. 4 }
    CY[1] = 0;

    CX[2] = 0; // {Go "North" but actually is South because the rows are reversed }
    CY[2] = -1;

    CX[3] = 1; //{Go East. 2 }
    CY[3] = 0;

    CX[4] = 0;  //{Go "South" but actually is North because the rows are reversed  }
    CY[4] = 1;
#pragma endregion

    // 4. Assigning initial values.
    FX[1] = X;
    FY[1] = Y;
    CLOSE = 1;
    NEWN = 1;
    YES = false;

    // 5. Breadth-first search -- the "wave" algorithm.
    if (X == 1 || X == M || Y == 1 || Y == N) {
        // If an exit is reached then finish.
        YES = true;
        U = X;
        V = Y;
    }

#pragma region PrintPart1
    cout << "PART 1. Data" << endl;
    out << "PART 1. Data" << endl;
    cout << " 1.1 Labyrinth" << endl << endl;
    out << " 1.1 Labyrinth" << endl << endl;


    cout << "   Y,V ^" << endl;
    out << "   Y,V ^" << endl;

    int v = N;

    for (int i = N;i >= 1;i--) {
        cout << "   ";
        out << "   ";
        
        cout << right << setw(3) << v << " | ";
        out << right << setw(3) << v << " | ";


        for (int j = 1;j <= M;j++) {
            cout << right << setw(2) << to_string(LABCOPY[i][j]) << "\t";
            out << right << setw(2) << to_string(LABCOPY[i][j]) << "\t";
        }
        std::cout << endl;
        out << endl;

        v--;
    }
    cout << "        ";
    out << "        ";

    for (int i = 1;i < M;i++) {
        std::cout << "---------";
        out << "----";
    }

    cout << "-->" << " X, U" << endl;
    out << "-->" << " X, U" << endl;

    cout << "\t";
    out << "\t";


    cout << "  1" << "\t";
    out << "      1" << "\t";

    for (int u = 2;u <= M;u++) {
        cout << right << setw(2) << to_string(u) << "\t";
        out << right << setw(2) << to_string(u) << "\t";
    }
    cout << endl << endl;
    out << endl << endl;


    cout << " 1.2 Initial position X="<<X<<", Y="<<Y<<", "<<"NEWN="<<NEWN<<"." << endl << endl;
    out << " 1.2 Initial position X=" << X << ", Y=" << Y << ", " << "NEWN=" << NEWN << "." << endl << endl;

#pragma endregion

#pragma region PrintPart2
    cout << "PART 2. Trace" << endl << endl;
    out << "PART 2. Trace" << endl << endl;

    cout << "WAVE 0, label L=\"" << L << "\". Initial position X=" << X << ", Y=" << Y << ", NEWN=" << 1 << "." << endl << endl;
    out << "WAVE 0, label L=\"" << L << "\". Initial position X=" << X << ", Y=" << Y << ", NEWN=" << 1 << "." << endl << endl;

    //M:column
    //N:rows
    if (X > 1 && X < M && Y > 1 && Y < N) {
        // The loop through the nodes.
        do {
            X = FX[CLOSE];
            Y = FY[CLOSE]; // Coordinates of node to be closed.
            K = 0;

            // The loop through 4 production rules.
            do {
                K = K + 1;
                U = X + CX[K];
                V = Y + CY[K];

                if (LABCOPY[Y][X] + 1 != L) {
                    L = LABCOPY[Y][X] + 1;
                    cout << "WAVE " << L - 2 << ", label L=\"" << L << "\"" << endl;
                    out << "WAVE " << L - 2 << ", label L=\"" << L << "\"" << endl;
                }
                if (K == 1) {
                    cout << "\tClose CLOSE=" << CLOSE << ", X=" << FX[CLOSE] << ", Y=" << FY[CLOSE] << "." << endl;
                    out << "\tClose CLOSE=" << CLOSE << ", X=" << FX[CLOSE] << ", Y=" << FY[CLOSE] << "." << endl;
                }

                if (LABCOPY[V][U] == 0) {

                    //FREE CELL
                    // The cell is free
                    LABCOPY[V][U] = LABCOPY[Y][X] + 1; // Set the new wave's number in the matrix

                    if (U == 1 || U == M || V == 1 || V == N) {
                        // Boarder.
                        YES = true; // Success. Here BACK(U,V) could be called.
                        cout << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". Free. NEWN=" << NEWN + 1 << ". Terminal." << endl;
                        out << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". Free. NEWN=" << NEWN + 1 << ". Terminal." << endl;
                        rules_counter++;
                    }
                    else {
                        // Placing a newly opened node into front's end.
                        NEWN = NEWN + 1;
                        FX[NEWN] = U;
                        FY[NEWN] = V;
                        cout << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". Free. NEWN=" << NEWN << "." << endl;
                        out << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". Free. NEWN=" << NEWN << "." << endl;
                        rules_counter++;
                    }
                }
                else {
                    if (LABCOPY[V][U] == 1) {//found a wall
                        //Apply a production rule and found a wall
                        cout << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". Wall." << endl;
                        out << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". Wall." << endl;
                        rules_counter++;
                    }
                    else {
                        //Apply a production rule and found a wall
                        cout << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". CLOSED or OPEN." << endl;
                        out << "\t\t R" << K << ". X=" << U << ", Y=" << V << ". CLOSED or OPEN." << endl;
                        rules_counter++;
                    }
                }
            } while (K != 4 && !YES); // Each of 4 productions is checked or success.
            CLOSE = CLOSE + 1; // Next node will be closed.
            cout << endl;
            out << endl;
        } while (CLOSE <= NEWN && !YES);
    }

    // 6. Printing the path found.
    if (YES) {
        BACK(U, V); // Collecting the path.
        // Here a procedure should be called to print the path.
    }
    else {
        cout << "No path." << endl;
        out << "No path." << endl;
        return 0;
    }

#pragma endregion

#pragma region Printing Part 3
    cout << "PART 3. Results" << endl << endl;
    out << "PART 3. Results" << endl << endl;

    cout << " 3.1 Path is found." << endl << endl;
    out << " 3.1 Path is found." << endl << endl;

    cout << " 3.2 Path graphically:" << endl << endl;
    out << " 3.2 Path graphically:" << endl << endl;


    cout << "LABCOPY"<<endl;
    out << "LABCOPY" << endl;

    cout << "   Y,V ^" << endl;
    out << "   Y,V ^" << endl;

    v = N;

    for (int i = N;i >= 1;i--) {
        std::cout << "   ";
        out << "   ";

        cout << right << setw(3) << v << " | ";
        out << right << setw(3) << v << " | ";

        for (int j = 1;j <= M;j++) {
            cout << right << setw(2) << to_string(LABCOPY[i][j]) << "\t";
            out << right << setw(2) << to_string(LABCOPY[i][j]) << "\t";
        }
        cout << endl;
        out << endl;

        v--;
    }
    cout << "        ";
    out << "        ";

    for (int i = 1;i < M;i++) {
        cout << "---------";
        out << "----";

    }

    cout << "-->" << " X, U" << endl;
    out << "-->" << " X, U" << endl;

    cout << "\t";
    out << "\t";


    cout << "  1" << "\t";
    out << "      1" << "\t";
    
    for (int u = 2;u <= M;u++) {
        cout << right << setw(2) << to_string(u) << "\t";
        out << right << setw(2) << to_string(u) << "\t";
    }
    cout << endl << endl;
    out << endl << endl;

    //print list of rules
    cout << " 3.3. Rules: ";
    out << " 3.3. Rules: ";

    while (!rules.empty()) {
        cout << rules.top() << ", ";
        out << rules.top() << ", ";
        rules.pop();
    }

    cout << "\b\b." << endl << endl;
    out.seekp(-2, std::ios_base::cur);
    out << "." << endl << endl;

    //print list of nodes
    cout << " 3.4. Nodes: ";
    out << " 3.4. Nodes: ";
    while (!nodes.empty()) {
        cout << nodes.top() << ", ";
        out << nodes.top() << ", ";
        nodes.pop();
    }

    cout << "\b\b." << endl << endl;
    out.seekp(-2, std::ios_base::cur);
    out << "." << endl << endl;

    //print list of rules
    cout << " 3.4. Number of productions applied: "<<rules_counter<<endl<<endl;
    out << " 3.4. Number of productions applied: " << rules_counter;
#pragma endregion


    return 0;
}