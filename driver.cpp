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

// Get inputs, hand information to simulator
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

    // Iterate through all problems
    for (int i = 0; i < operand1_count; i++)
    {
        simulate_IM(operand1s[i], operand2s[i]); // Begin simulation
    }
}

// Main function for simulating iterative method
void simulate_IM(bitset<16> op1, bitset<16> op2)
{
    //cout << "\n" << dec << op1.to_ulong() << " x " << op2.to_ulong() << " = " << hex << (op1.to_ullong() * op2.to_ulong()); // Answers in Hexadecimal
    
    
    // Operations should be rounded to 1 of 2 categories --> 8x8, or 16x16 depending on which ever operand is larger
    // at minimum must use 4x4 multiplier --> so round to either 8x8 or 16x16
    // This is used later for calculating correct timing but all operations will be 16x16

    int roundedLen = getRoundedLength(getLength(op1), getLength(op2));
    //cout << "\nRounded length: " << roundedLen;










    // Notes:
    // Four 4x4 Multiplier units available to run simultaneously, each has a delay of 21dt
    // Results should be in Binary and Hexadecimal
    // Execution time must be calculated in terms of dt. AND/OR gate has delay of 1dt

    // Step 1: split each operand into 2x8 bit sets;
}

// Return length of operand
int getLength(bitset<16> op)
{
    int len = 0;

    for (int i = 15; i > 0; i--)  // Find first 1 bit from high-order bits to get true length (disregard leading 0's)
    {
        if (op[i] == 1)
        {
            len = i + 1;
            i = 0; // Terminate
        }
    }
    return len;
}

// Return length rounded to nearest 8 or 16 bit
int getRoundedLength(int lenOp1, int lenOp2)
{
    int roundedLen = 0;

    //Determine 8 or 16 bit
    if (lenOp1 > lenOp2)
    {
        if (lenOp1 <= 8) { roundedLen = 8; } // Round to 8 bits
        else { roundedLen = 16; } // Round to 16 bits (keep original)
    }
    else if (lenOp2 > lenOp1)
    {
        if (lenOp2 <= 8) { roundedLen = 8; } // Round to 8 bits
        else { roundedLen = 16; } // Round to 16 bits (keep original)
    }
    else // Equal length, use Op1
    {
        if (lenOp1 <= 8) { roundedLen = 8; } // Round to 8 bits
        else { roundedLen = 16; } // Round to 16 bits (keep original)
    }

    return roundedLen;
}
