#include <iostream>
#include <iterator>
#include <vector>
#include <stdlib.h>

int iteration_number = 0;
char firstrod;
char secondrod;
char thirdrod;

void PrintStateOfARod(char key, std::vector<int>&state);
void HanoiTower(char x, char y, char z,int n);
void RecursiveHanoi(char x, char y, char z, std::vector<int>&state_x, std::vector<int>&state_y, std::vector<int>&state_z, int n);
void PrintState(char x,char y, char z, std::vector<int>&state_x, std::vector<int>&state_y, std::vector<int>&state_z);

int main()
{
    char x,y,z;
    int n;
    std::cout << "HANOI TOWER" << std::endl;

    do {
        std::cout << "USE: " << std::endl;
        std::cout << "1st parameter: name of the first rod" << std::endl;
        std::cout << "2nd parameter: name of the second rod" << std::endl;
        std::cout << "3rd parameter: name of the third rod" << std::endl;
        std::cout << "4th parameter: number of plates between 3 and 10 included" << std::endl;
        std::cout << "Please enter one parameter at a time"<<std::endl;
        std::cin >> x;
        std::cin >> y;
        std::cin >> z;
        std::cin >> n;
        if (n < 3 || n>10) {
            system("CLS");
            std::cout << "Please enter a valid number of plates, retry" << std::endl;
        }
    } while (n < 3 || n>10);

    firstrod = x;
    secondrod = y;
    thirdrod = z;

    system("CLS");
    std::cout << "   Number of iterations.Move description.State of the rods.\n"<<std::endl;
    HanoiTower(x,y,z,n);  
    return 0;
}


void HanoiTower(char x, char y, char z,int n) {
    iteration_number = 0;
    std::vector<int> state_x;
    std::vector<int> state_y;
    std::vector<int> state_z;

    for (int i = 0; i <= n;i++) {
        state_x.push_back(i);
        state_y.push_back(0);
        state_z.push_back(0);
    }

    //print initial state
    std::cout<<"   Initial state ";
    iteration_number++;
    PrintState(x,y,z,state_x, state_y, state_z);
    
    RecursiveHanoi(x,y,z,state_x, state_y, state_z, n);
}

void RecursiveHanoi(char x, char y, char z, std::vector<int> &state_x, std::vector<int>&state_y, std::vector<int>&state_z, int n) {
    if (n > 0) {
        RecursiveHanoi(x,z,y,state_x, state_z, state_y, n - 1); //part 1: move n-1 disks onto an intermediary rod
        std::cout <<"   "<< iteration_number++ << ".Move disk " << n << " from " << x << " to " << z << ". "; //part 2:
        state_z.at(n) = state_x.at(n);
        state_x.at(n) = 0;
        PrintState(x,y,z,state_x,state_y,state_z);
        RecursiveHanoi(y,x,z,state_y, state_x, state_z, n - 1); //part 3: move n-1 disks onto target
    }
}

void PrintState(char x, char y, char z, std::vector<int>&state_x, std::vector<int>&state_y, std::vector<int>&state_z) {
    //first rod
    if (x == firstrod) {
        PrintStateOfARod(x, state_x);
    }
    else if(y == firstrod) {
        PrintStateOfARod(y, state_y);
    }
    else {
        PrintStateOfARod(z, state_z);
    }
    std::cout << ", ";
    //second rod
    if (x == secondrod) {
        PrintStateOfARod(x, state_x);
    }
    else if (y == secondrod) {
        PrintStateOfARod(y, state_y);
    }
    else {
        PrintStateOfARod(z, state_z);
    }
    std::cout << ", ";
    //third rod
    if (x == thirdrod) {
        PrintStateOfARod(x, state_x);
    }
    else if (y == thirdrod) {
        PrintStateOfARod(y, state_y);
    }
    else {
        PrintStateOfARod(z, state_z);
    }
    std::cout <<"."<< std::endl;
}

void PrintStateOfARod(char key, std::vector<int>&state) {
    std::cout << key << "=(";
    for (int i = state.size()-1; i > 0;i--) {
        if (state.at(i) != 0)
            std::cout << state.at(i) << ",";
    }
    std::cout << ")";
}