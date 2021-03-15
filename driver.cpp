// Iterative_Method_Simulator.cpp : This file contains the 'main' function. Program execution begins and ends there.

///////////////////////////////////////////////////////////////////////////////////////
// Desc: Iterative Method Multiplication Simulator                                   //
// Class: CpE 5110                                                                   //
// Instructor: Dr. Ali Hurson                                                        //
// Date: 3/15/2021                   SP21                                            //
// Piotr Pogorzelski, Levi Vieth                                                     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <iomanip> 
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>

#include "driver.h"

using namespace std; 
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

int main()
{
    // Declare variables
    vector<bitset<16>> operand1s;
    vector<bitset<16>> operand2s;
    string operand1;
    string operand2;

    int operand1_count = 0;
    int operand2_count = 0;

    cout << "--- Iterative Method Multiplication Simulator ---";

    // Read in operands from files
    cout << "\n\nReading operands from files (binary).....\n\n";

    #pragma region File input operand1
    // Read in binary operand1 from file
    ifstream op1_input_file("operand1.txt");
    if (op1_input_file.is_open())
    {
        while (getline(op1_input_file, operand1))
        {
            operand1s.push_back(bitset<16>(operand1));
            cout << "Operand1[" << operand1_count << "]: " << (operand1s[operand1_count]).to_ulong() << '\n';
            operand1_count++;
        }
        op1_input_file.close();
    }
    else
    {
        cout << "\nUnable to open operands1 file";
        return 0;
    }
    #pragma endregion

    #pragma region File input operand2
    // Read in binary operand2 from file
    ifstream op2_input_file("operand2.txt");
    if (op2_input_file.is_open())
    {
        cout << "\n\n"; // spacing 

        while (getline(op2_input_file, operand1))
        {
            operand2s.push_back(bitset<16>(operand1));
            cout << "Operand2[" << operand2_count << "]: " << (operand2s[operand2_count]).to_ulong() << '\n';
            operand2_count++;
        }
        op2_input_file.close();
    }
    else
    {
        cout << "\nUnable to open operands2 file";
        return 0;
    }
    #pragma endregion

    simulate_IM(operand1_count, operand1s, operand2s); // Begin simulation
}


void simulate_IM(int op_count, vector<bitset<16>> op1, vector<bitset<16>> op2)
{
    cout << "yeet";
}
