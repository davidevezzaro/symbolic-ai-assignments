#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <iostream>
using namespace std;

vector<vector<int>>LAB; //Global database
vector<string>rules,nodes;
fstream file;
//{BACKTRACK1, i.e. depth-first, no infinite cycle.}
//Dimensions
int M;
int N;
//{4 production – shifts in X and Y.}
int CX[5];
int CY[5];
//{Move’s number. Starts from 2. Visited positions are marked.}
int L;
//{Agent’s initial position.}
int X;
int Y;
//{Loop variables.}
int I;
int J;
//{Number of trials.To compare effectiveness.}
int TRIAL;
//{True – success, false – failure.}
bool YES;
string s;
int trace_counter=0;

void ReadFromFile(string input);
void printDepth(int n);

void ReadFromFile(string input) {
	int r = 0;
	file.open(input);
	string word;

	if (file.is_open()) {
		LAB.push_back(std::vector<int>());//dummy row
		LAB.push_back(std::vector<int>());//real first row
		
		r++;
		
		LAB[r].push_back(-2);//dummy column
		

		while(file >> word) {
			if (!word.find("Y=")) {
				//new row added
				LAB.push_back(std::vector<int>());
				
				r++;
				
				LAB[r].push_back(-2);//dummy column
			}
			else if (!word.find("Line")) {}
			else if (!word.find("Start")){
				//remove the last row added
				Y=LAB[r].back();
				LAB[r].pop_back();
				X=LAB[r].back();
				LAB[r].pop_back();
				LAB.pop_back();
				break;
			}
			else {
				LAB[r].push_back(stoi(word));
			}
		}
	}
	file.close();
	M = LAB[1].size()-1;//number of columns
	N = LAB.size()-1;//number of rows
	cout << "";
}

void TRY1(int X, int Y, bool& YES) {
	//{The number of a production rule.}
	int K;
	//{Agent’s new position.}
	int U,V;
	//{K1}
	if (X == 1 || X == M || Y == 1 || Y == N) {
		YES = true;//true on the boarder

		string node_to_add = "[X=" + to_string(X) + ",Y=" + to_string(Y) + "]";
		nodes.push_back(node_to_add);
	}
	else {
		//{K2}
		K = 0;
		// {Next rule. Loop over production rules.}
		do {
			//{K3}
			K++;
			//{Agent’s new position.}
			U = X + CX[K];
			V = Y + CY[K];

			//{K4}
			if (LAB[V][U] == 0) {//{If a cell is free.}
				//{Number of trials.}
				TRIAL++;
				//{K5}

				//list of rules
				string rule_to_add = "R" + to_string(K);
				rules.push_back(rule_to_add);
				//list of nodes
				string node_to_add = "[X=" + to_string(X) + ",Y=" + to_string(Y) + "]";
				nodes.push_back(node_to_add);

				L++;
				//{Marking the cell.}
				LAB[V][U] = L;

				trace_counter++;
				string temp = to_string(trace_counter) + ") ";
				
				//FREE CELL
				cout << endl;
				cout << right << setw(6) << temp;
				printDepth(L - 3);
				cout << "R" << K << ". U=" << U << ", V=" << V << ". Free. L:=L+1=" << L << ". LAB[" << U << "," << V << "]:=" << L <<". ";

				//{K6}
				TRY1(U, V, YES);//{Recursive call.}
				//{K7}
				if (!YES) {//{If failure}
					//{K8}
					LAB[V][U] = -1;
					
					//BACKTRACK
					cout << endl;
					string temp = to_string(trace_counter) + ") ";
					cout << "      ";
					printDepth(L - 2);
					cout << "Backtrack from X=" << U << ", Y=" << V << ", L=" << L << ". LAB[" << U << "," << V << "]:=-1. L:=L-1=" << L - 1<<". ";
					
					//remove the rules from the valid rules 
					rules.pop_back();
					nodes.pop_back();

					L--;
				}
			}
			else {
				//WALL FOUND
				cout << endl;
				trace_counter++;
				string temp = to_string(trace_counter) + ") ";
				

				if (LAB[V][U] == 1) {
					cout << right << setw(6) << temp;
					printDepth(L - 2);
					cout << "R" << K << ". U=" << U << ", V=" << V << ". Wall. ";
				}
				else if(LAB[V][U]!=-2){
					cout << right << setw(6) << temp;
					printDepth(L - 2);
					cout << "R" << K << ". U=" << U << ", V=" << V << ". Thread. ";
				}
			}
		} while (!YES && K != 4);
	}
}
void TRY2(int X, int Y, bool& YES) {
	//{The number of a production rule.}
	int K;
	//{Agent’s new position.}
	int U, V;
	//{K1}
	if (X == 1 || X == M || Y == 1 || Y == N) {
		YES = true;//true on the boarder

		string node_to_add = "[X=" + to_string(X) + ",Y=" + to_string(Y) + "]";
		nodes.push_back(node_to_add);
	}
	else {
		//{K2}
		K = 0;
		// {Next rule. Loop over production rules.}
		do {
			//{K3}
			K++;
			//{Agent’s new position.}
			U = X + CX[K];
			V = Y + CY[K];

			//{K4}
			if (LAB[V][U] == 0) {//{If a cell is free.}
				//{Number of trials.}
				TRIAL++;
				//{K5}

				//list of rules
				string rule_to_add = "R" + to_string(K);
				rules.push_back(rule_to_add);
				string node_to_add = "[X=" + to_string(X) + ",Y=" + to_string(Y) + "]";
				nodes.push_back(node_to_add);

				L++;
				//{Marking the cell.}
				LAB[V][U] = L;

				trace_counter++;
				string temp = to_string(trace_counter) + ") ";

				//FREE CELL
				cout << endl;
				cout << right << setw(6) << temp;
				printDepth(L - 3);
				cout << "R" << K << ". U=" << U << ", V=" << V << ". Free. L:=L+1=" << L << ". LAB[" << U << "," << V << "]:=" << L << ". ";

				//{K6}
				TRY2(U, V, YES);//{Recursive call.}
				//{K7}
				if (!YES) {//{If failure}
					//{K8}
					LAB[V][U] = 0;

					//BACKTRACK
					cout << endl;
					string temp = to_string(trace_counter) + ") ";
					cout << "      ";
					printDepth(L - 2);
					cout << "Backtrack from X=" << U << ", Y=" << V << ", L=" << L << ". LAB[" << U << "," << V << "]:=-1. L:=L-1=" << L - 1 << ". ";

					//remove the rules from the valid rules 
					rules.pop_back();
					nodes.pop_back();

					L--;
				}
			}
			else {
				//WALL FOUND
				cout << endl;
				trace_counter++;
				string temp = to_string(trace_counter) + ") ";


				if (LAB[V][U] == 1) {
					cout << right << setw(6) << temp;
					printDepth(L - 2);
					cout << "R" << K << ". U=" << U << ", V=" << V << ". Wall. ";
				}
				else if (LAB[V][U] != -2) {
					cout << right << setw(6) << temp;
					printDepth(L - 2);
					cout << "R" << K << ". U=" << U << ", V=" << V << ". Thread. ";
				}
			}
		} while (!YES && K != 4);
	}
}

void printDepth(int n) {
	s = "";
	for (int i = 0; i < n; i++) {
		s.append("-");
	}
	cout << s;
}

int main()
{
	cout << "DFS LABYRINTH" << std::endl;

	cout << "USE: " << std::endl;
	cout << "1st parameter: number of test [1,2,3]" << std::endl;
	cout << "2nd parameter: variant of the algorithm [1,2]" << std::endl;
	cout << "Please enter one parameter at a time" << std::endl;
	int test, variant;
	cin >> test;
	cin >> variant;

	system("CLS");

	cout << "TEST:" << test << " VARIANT:" << variant << endl<<endl;
	switch (test) {
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
	
#pragma region PrintPart1
	cout << "PART 1. Data" << endl;
	cout << " 1.1 Labyrinth" << endl << endl;

	std::cout << "   Y,V ^" << endl;

	int v = N;

	for (int i = N;i >= 1;i--) {
		std::cout << "   ";
		cout << right << setw(3) << v << " | ";

		for (int j = 1;j <= M;j++) {
			//std::cout << LAB[i][j] << "\t";
			cout << right << setw(2) << to_string(LAB[i][j]) << "\t";
		}
		std::cout << endl;

		v--;
	}
	std::cout << "        ";

	for (int i = 1;i < M;i++) {
		std::cout << "---------";

	}

	std::cout << "-->" << " X, U" << endl;

	std::cout << "\t";

	cout << "  1" << "\t";
	for (int u = 2;u <= M;u++) {
		std::cout << right << setw(2) << to_string(u) << "\t";
	}
	cout << endl << endl;

#pragma endregion

#pragma region Initialize starting point
	L = 2;
	LAB[Y][X] = L;
	cout <<" 1.2. Initial position X=" << X << ", Y=" << Y << "." << " L=" << L << "." << endl;
#pragma endregion

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

#pragma region {4. Initialising variables.} 
	YES = false;
	TRIAL = 0;
#pragma endregion

#pragma region {5. Invoking the BACKTRACK1 procedure.} 
	cout << endl << "PART 2. Trace" << endl;
	switch (variant) {
	case 1:
		TRY1(X, Y, YES);
		break;
	case 2:
		TRY2(X, Y, YES);
		break;
	}
	
	if (YES) {
		std::cout << "Terminal." << endl;
	}
	else {
		std::cout << "Path does not exist." << endl; //{No paths exist.}
	}
#pragma endregion

#pragma region Printing Part 3
	cout << endl << "PART 3. Results"<<endl<<endl;
	cout << " 3.1 Path is found." << endl << endl;
	cout << " 3.2 Path graphically:" << endl << endl;

	std::cout << "   Y,V ^" << endl;

	v = N;

	for (int i = N;i >= 1;i--) {
		std::cout << "   "; 
		cout <<right << setw(3) << v << " | ";

		for (int j = 1;j <= M;j++) {
			cout << right << setw(2) << to_string(LAB[i][j])<<"\t";
		}
		std::cout << endl;

		v--;
	}
	std::cout << "        ";

	for (int i = 1;i < M;i++) {
		std::cout << "---------";

	}

	std::cout << "-->" << " X, U" << endl;

	std::cout << "\t";


	cout << "  1" << "\t";
	for (int u = 2;u <= M;u++) {
		std::cout << right<<setw(2) << to_string(u) << "\t";
	}
	cout << endl<<endl;

	//print list of rules
	cout << " 3.3. Rules: ";
	for (int i = 0; i < rules.size();i++) {
		cout << rules[i] << ", ";
		if (i%40 == 0 &&i!=0) {
			cout << endl<<"\t     ";
		}
	}
	cout << "\b\b." << endl<<endl;

	//print list of nodes
	cout << " 3.4. Nodes: ";
	for (int i = 0; i < nodes.size();i++) {
		cout << nodes[i] << ", ";
		if (i%15 == 0 && i != 0) {
			cout << endl<<"\t     ";
		}
	}
	cout << "\b\b." << endl << endl;
#pragma endregion

	return 0;
}


