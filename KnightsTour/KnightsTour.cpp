#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <string>

using namespace std;

int N; // Length of the board
int NN; // Number of squares 5*5
int CX[9], CY[9]; // 8 production rules

vector<vector<int>> BOARD; //Global database
//int BOARD[6][6]; // Global database
int trace_counter = 0;
int tree_depth = 0;
int X, Y, L;
bool YES;
string s = "";
ofstream out_short, out_long;

void printDepth(int n);
void TRY(int L, int X, int Y, bool &YES);
void printPart1();

void INITIALIZE() {
    // 1) Production set is formed
    CX[1] = 2; CY[1] = 1;
    CX[2] = 1; CY[2] = 2;
    CX[3] = -1; CY[3] = 2;
    CX[4] = -2; CY[4] = 1;
    CX[5] = -2; CY[5] = -1;
    CX[6] = -1; CY[6] = -2;
    CX[7] = 1; CY[7] = -2;
    CX[8] = 2; CY[8] = -1;

    // 2) Initialize the global database
    for (int i = 0; i <= N; i++) {
        BOARD.push_back(std::vector<int>());
        for (int j = 0; j <= N; j++) {
            BOARD[i].push_back(0);
        }
    }
}

void TRY(int L, int X, int Y, bool &YES) {
    int K; // Production number.
    int U, V; // New knight's position.
    K = 0;
    do { // Select each of 8 productions.
        K++;
        U = X + CX[K]; 
        V = Y + CY[K];
        // Check if the condition of the production is satisfied.
        if (U >= 1 && U <= N && V >= 1 && V <= N) { // Within the board.
            // Check if the cell is empty.
            if (BOARD[U][V] == 0) {
                // New position.
                //Free
                trace_counter++;
                string temp = to_string(trace_counter) + ") ";

                out_long << right << setw(10) << temp;
                printDepth(L - 2);
                out_long << "R" << K << ". U=" << U << ", V=" << V << ". L=" << L << ". Free. BOARD[" << U << "," << V << "]:=" << L << endl;

                BOARD[U][V] = L;

                // Check if all squares are visited.
                if (L < NN) { // If not all are visited then try with a new cell
                    TRY(L+1, U, V, YES);
                    
                    // If no success,
                    // then backtrack and free the position cause this path can't lead me to a solution
                    if (!YES) {
                        BOARD[U][V] = 0;
                    }
                }
                else {
                    YES = true; // When L=NN
                }
            }
            else {//the cell is already taken
                trace_counter++;
                string temp = to_string(trace_counter) + ") ";

                out_long << right << setw(10) << temp;
                printDepth(L-2);
                out_long << "R" << K << ". U=" << U << ", V=" << V << ". L=" << L << ". Thread.";
                if (K == 8) {
                    out_long << " Backtrack." << endl;
                }
                else {
                    out_long << endl;
                }
            }
        }
        else {
            //Out
            trace_counter++;
            string temp = to_string(trace_counter) + ") ";

            out_long <<right << setw(10) << temp;
            
            printDepth(L-2);

            out_long << "R" << K << ". U=" << U << ", V=" << V << ". L=" << L << ". Out.";
            if (K == 8) {
                out_long << " Backtrack." << endl;
            }
            else {
                out_long << endl;
            }
        }
    } while (K < 8 && !YES); //Either success or all productions were tried
}

int main() {
    cout << "KNIGHTS TOUR" << std::endl;

    cout << "USE: " << std::endl;
    cout << "1st parameter: number of cells of a square board" << std::endl;
    cout << "2nd parameter: starting x coordinate of the knight" << std::endl;
    cout << "3rd parameter: starting y coordinate of the knight" << std::endl;
    cout << "Please enter one parameter at a time" << std::endl;
    cin >> N;
    cin >> X;
    cin >> Y;

    system("CLS");
    
    YES = false;
    //N = 5;
    NN = N * N;
    //X = 5;
    //Y = 1;
    L = 2;

    out_short = ofstream ("out-short.txt");
    out_long = ofstream("out-long.txt");
    
    INITIALIZE();
    //PART 1
    std::cout << "PART 1. Data" << endl;
    out_short << "PART 1. Data" << endl;
    out_long << "PART 1. Data" << endl;

    std::cout << "1) Board: " << N << "x" << N << "." << endl;
    out_short << "1) Board: " << N << "x" << N << "." << endl;
    out_long << "1) Board: " << N << "x" << N << "." << endl;

    std::cout << "2) Initial position: X=" << X << ",Y=" << Y << ". L=" << L-1 << endl << endl;
    out_short << "2) Initial position: X=" << X << ",Y=" << Y << ". L=" << L-1 << endl << endl;
    out_long << "2) Initial position: X=" << X << ",Y=" << Y << ". L=" << L-1 << endl << endl;

    //initialize first position
    BOARD[X][Y] = 1;

    //PART 2
    out_long << "PART 2. Trace" << endl;
    TRY(L, X, Y, YES);
    if (!YES) {
        out_long << "Path doesn't exist."<<endl;
    }

    //PART 3
    std::cout <<"PART 3. Results" << endl;
    out_short << "PART 3. Results" << endl;
    out_long << endl<<"PART 3. Results" << endl;

    if (!YES) {
        std::cout << "1) Path doesn't exist. Trials=" << trace_counter << "." << endl;
        out_short << "1) Path doesn't exist. Trials=" << trace_counter << "." << endl;
        out_long << "1) Path doesn't exist. Trials=" << trace_counter << "." << endl;
    }
    else {
        std::cout << "1) A path is found. Trials=" << trace_counter << "."<<endl;
        out_short << "1) A path is found. Trials=" << trace_counter << "." << endl;
        out_long << "1) A path is found. Trials=" << trace_counter << "." << endl;
        
        std::cout << "2) Path graphically:"<<endl<<endl;
        out_short << "2) Path graphically:" << endl << endl;
        out_long << "2) Path graphically:" << endl << endl;
        
        std::cout << "Y, V ^" << endl;
        out_short << "Y, V ^" << endl;
        out_long << "Y, V ^" << endl;

        int v = N;
        for (int i = N;i ;i--) {
            std::cout << "   "<<v << " | ";
            out_short << "   " << v << " | ";
            out_long << "   " << v << " | ";

            for (int j = 1;j <= N;j++) {
                std::cout << BOARD[j][i]<<"\t";
                out_short << BOARD[j][i] << "\t";
                out_long << BOARD[j][i] << "\t";
            }
            std::cout << endl;
            out_short << endl;
            out_long << endl;

            v--;
        }
        std::cout << "     ";
        out_short << "     ";
        out_long << "     ";

        for (int i = 1;i < N;i++) {
            std::cout << "---------";
            out_short << "-----";
            out_long << "-----";
        }

        std::cout << "-->" << " X, U" << endl;
        out_short << "-->" << " X, U" << endl;
        out_long << "-->" << " X, U" << endl;
        
        std::cout << "\t";
        out_short << "\t";
        out_long << "\t";
        
        out_short << "   ";
        out_long << "   ";

        for (int u = 1;u <= N;u++) {
            std::cout << u << "\t";
            out_short << u << "\t";
            out_long << u << "\t";
        }
    }

    //close the txt files
    out_short.close();
    out_long.close();

    return 0;
}

void printDepth(int n) {
    s = "";
    for (int i = 0; i < n; i++) {
        s.append("-");
    }
    out_long << s;
}