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
    static uint8_t num_multipliers = 0;
    static uint8_t num_additions = 0;

    int operand1_count = 0;
    int operand2_count = 0;

    cout << "--- Iterative Method Multiplication Simulator ---";

    // Read in operands from files
    cout << "\n\nReading operands from files (binary).....";

    #pragma region File input operand1
    // Read in binary operand1 from file
    ifstream op1_input_file("operand1.txt");
    if (op1_input_file.is_open())
    {
        while (getline(op1_input_file, operand1))
        {
            operand1s.push_back(bitset<16>(operand1));
            //cout << "Operand1[" << operand1_count << "]: " << (operand1s[operand1_count]).to_ulong() << '\n';
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
            //cout << "Operand2[" << operand2_count << "]: " << (operand2s[operand2_count]).to_ulong() << '\n';
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

    // Table formating
    cout << "\n============================ Iterative Method ===========================";
    cout << "\n| Op1 (Hex) | Op2 (Hex)| Answer | #Execution Time dt |";
    cout << "\n---------------------------------------------------------------------------";

    uint8_t op_length;
    
    
    // Iterate through all problems
    for (int i = 0; i < operand1_count; i++)
    {
        
        op_length = getRoundedLength(getLength(operand1s[i]), getLength(operand2s[i]));
               

        //*Note:
        // number of multiplications and additions are added within starting at 4x4 for x's and 8x8 for +'s
        // since simulation always runs 16x16, we have to pick out the 
        // rounded length of the operands to then decide how many x's and +'s
        // are actually there, otherwise it will always be 16 multiplications and 10 additions,
        // even for 4x4 and 8x8
         

        simulate_IM(operand1s[i], operand2s[i], num_multipliers, num_additions); // Begin simulation

        //truncate additions and multiplications for 4bit and 8bit operands, otherwise
        //leave for 16bit
        if (op_length == 4) 
        { 
            num_multipliers = 1; 
            num_additions = 0; 
        }

        else if (op_length == 8) 
        { 
            num_multipliers = 4;
            num_additions = 2;
        }
        //calculate and display the timings
        cout << dec << "\t\t " << calculate_timing(num_multipliers, num_additions);
        
        //reset for next set of operands
        num_multipliers = 0;
        num_additions = 0;

    }

    cout << "\n==================================================================+========\n";
}

// Main function for simulating iterative method
void simulate_IM(bitset<16> op1, bitset<16> op2, uint8_t & num_multiplications, uint8_t & num_additions)
{
    //cout << "\n" << dec << op1.to_ulong() << " x " << op2.to_ulong() << " = " << hex << (op1.to_ullong() * op2.to_ulong()); // Answers in Hexadecimal (debug)
    
    // Output operand1 and operand2 onto the output table
    cout << "\n  " << uppercase << hex << op1.to_ulong() << "\t\t"<<op2.to_ulong();

       
    // Operations should be rounded to 1 of 2 categories --> 8x8, or 16x16 depending on which ever operand is larger
    // at minimum must use 4x4 multiplier --> so round to either 8x8 or 16x16
    // This is used later for calculating correct timing but all operations will be 16x16

    int roundedLen = getRoundedLength(getLength(op1), getLength(op2));
    //cout << "\nRounded length: " << roundedLen;


    // Step 1.) Determine bc , ac , bd, ad (8x8 multiplication --> sub-iterative methods)

    // get "a" , "b", "c", "d" terms from operand1 and operand2
    bitset<8> a = getLeft8bits(op1);
    bitset<8> b = getRight8bits(op1);
    bitset<8> c = getLeft8bits(op2);
    bitset<8> d = getRight8bits(op2);

    // Calculate "bc", "ac", "bd", "ad" terms using 8x8 iterative method --> 4x4 multipliers
    bitset<16> bc = simulateIM_8x8(b, c, num_multiplications, num_additions);
    bitset<16> ac = simulateIM_8x8(a, c, num_multiplications, num_additions);
    bitset<16> bd = simulateIM_8x8(b, d, num_multiplications, num_additions);
    bitset<16> ad = simulateIM_8x8(a, d, num_multiplications, num_additions);

    // Concatinate "ac" and "bd"
    bitset<32> acbd = concat32_ac_bd(ac, bd);

    // Add "bc" and "ad" terms
    bitset<16> sum_bc_ad = bc.to_ulong() + ad.to_ulong();

    // Pad sum_bc_ad with 0's so it's in the middle
    bitset<32> pad_bc_ad = pad_32bit(sum_bc_ad);

    // Final addition for answer
    bitset<32> answer = acbd.to_ulong() + pad_bc_ad.to_ulong();

    //keeping track for timing calculations. for the 24 and 32bit additions of acdb and bc + ad
    num_additions += 2;


    // Output answer to table in hex
    cout << "\t " << hex << answer.to_ulong();
    //cout << "\n16x16 Answer: " << hex << answer.to_ulong() << timing_display <<"\n\n";
    // Need to display #additions and #multiplications --> iterative method --> 4 multiplications of n/2 bit numbers and 2 additions 9 (notes)

    
    // Notes:
    // Four 4x4 Multiplier units available to run simultaneously, each has a delay of 21dt
    // Results should be in Binary and Hexadecimal
    // Execution time must be calculated in terms of dt. AND/OR gate has delay of 1dt
}

// Simulate 8x8 iterative method used with 16x16 version
bitset<16> simulateIM_8x8(bitset<8> op1, bitset<8> op2, uint8_t & num_multiplications, uint8_t & num_additions)
{
    // To calculate "bc", "ac", "bd", "ad" need to seperate "a", "b", "c", "d" terms
    bitset<4> a = getLeft4bits(op1);
    bitset<4> b = getRight4bits(op1);
    bitset<4> c = getLeft4bits(op2);
    bitset<4> d = getRight4bits(op2);

    // Calculate "bc", "ac", "bd", "ad" terms using 4x4 multipliers
    bitset<8> bc = multiplier4x4(b, c, num_multiplications);
    bitset<8> ac = multiplier4x4(a, c, num_multiplications);
    bitset<8> bd = multiplier4x4(b, d, num_multiplications);
    bitset<8> ad = multiplier4x4(a, d, num_multiplications);

      
    // Concatinate "ac" and "bd"
    bitset<16> acbd = concat16_ac_bd(ac, bd);

    // Add "bc" and "ad" terms
    bitset<8> sum_bc_ad = bc.to_ulong() + ad.to_ulong();

    // Pad sum_bc_ad with 0's so it's in the middle
    bitset<16> pad_bc_ad = pad_16bit(sum_bc_ad);

    // Final addition for answer
    bitset<16> answer = acbd.to_ulong() + pad_bc_ad.to_ulong();

    //keeping track for timing calculations. for the 12 and 16bit additions of acdb and bc + ad
    num_additions += 2;

 
    #pragma region Debugging
    //cout << "\n\nop1: " << op1;
    //cout << "\nop2: " << op2;
    //cout << "\na: " << a;
    //cout << "\nb: " << b;
    //cout << "\nc: " << c;
    //cout << "\nd: " << d;
    //cout << "\nbc: " << bc;
    //cout << "\nac: " << ac;
    //cout << "\nbd: " << bd;
    //cout << "\nad: " << ad;
    //cout << "\nconcatinate ac bd: " << acbd;
    //cout << "\nsum bc_ad: " << sum_bc_ad;
    //cout << "\npadded bc_ad: " << pad_bc_ad;
    //cout << "\nAnswer: " << answer << "\n\n";
    #pragma endregion

    return answer;
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

    //Determine if 4 or 8 or 16 bit
    if (lenOp1 > lenOp2)
    {
        if (lenOp1 <= 4) { roundedLen = 4; } //round to 4 bits
        else if (lenOp1 <= 8) { roundedLen = 8; } // Round to 8 bits
        else { roundedLen = 16; } // Round to 16 bits (keep original)
    }
    else if (lenOp2 > lenOp1)
    {
        if (lenOp2 <= 4) { roundedLen = 4; } // round to 4 bits
        else if (lenOp2 <= 8) { roundedLen = 8; } // Round to 8 bits
        else { roundedLen = 16; } // Round to 16 bits (keep original)
    }
    else // Equal length, use Op1
    {
        if (lenOp1 <= 4) { roundedLen = 4; } //round to 4 bits
        else if (lenOp1 <= 8) { roundedLen = 8; } // Round to 8 bits
        else { roundedLen = 16; } // Round to 16 bits (keep original)
    }

    return roundedLen;
}


// Get the high order 8 bits from a 16 bit operand
bitset<8> getLeft8bits(bitset<16> op)
{
    bitset<8> temp;

    for (int i = 15; i > 7; i--)
    {
        temp[7 - (15 - i)] = op[i]; // extract "a" or "c"
    }
    return temp;
}

// Get the low order 8 bits from a 16 bit operand
bitset<8> getRight8bits(bitset<16> op)
{
    bitset<8> temp;

    for (int j = 7; j >= 0; j--)
    {
        temp[j] = op[j]; // extract "b" or "d"
    }
    return temp;
}

// Get the high order 4 bits from an 8 bit operand
bitset<4> getLeft4bits(bitset<8> op)
{
    bitset<4> temp;

    for (int i = 7; i > 3; i--)
    {
        temp[3 - (7 - i)] = op[i]; // extract sub "a" or "c"
    }
    return temp;
}

// Get the low order 4 bits from an 8 bit operand
bitset<4> getRight4bits(bitset<8> op)
{
    bitset<4> temp;

    for (int j = 3; j >= 0; j--)
    {
        temp[j] = op[j]; // extract sub "b" or "d"
    }
    return temp;
}

// Simulate 4x4 mulitplier
bitset<8> multiplier4x4(bitset<4> op1, bitset<4> op2, uint8_t & num_multiplications)
{
    bitset<8> ans = op1.to_ulong() * op2.to_ulong();

    //a 4x4 is used

    num_multiplications++;


    return ans;
}


// Concatinate "ac" and "bd" terms for 8x8 IM
bitset<16> concat16_ac_bd(bitset<8> ac, bitset<8> bd)
{
    bitset<16> ans;

    for (int i = 15; i > 7; i--)
    {
        ans[15 - (15 - i)] = ac[7-(15-i)]; // concatinate "ac" to left side
    }

    for (int j = 7; j >= 0; j--)
    {
        ans[j] = bd[j]; // concatinate "bd" to right side
    }

    return ans;
}

// Concatinate "ac" and "bd" terms for 16x16 IM
bitset<32> concat32_ac_bd(bitset<16> ac, bitset<16> bd)
{
    bitset<32> ans;

    for (int i = 31; i > 15; i--)
    {
        ans[31 - (31 - i)] = ac[15 - (31 - i)]; // concatinate "ac" to left side
    }

    for (int j = 15; j >= 0; j--)
    {
        ans[j] = bd[j]; // concatinate "bd" to right side
    }

    return ans;
}


// Shift left bc_ad to center it before final addition for 8x8 IM
bitset<16> pad_16bit(bitset<8> bc_ad)
{
    bitset<16> ans;

    for (int i = 7; i >= 0; i--)
    {
        ans[i + 4] = bc_ad[i]; // get into the form 0000 bc_ad 0000
    }
    return ans;
}

// Shift left bc_ad to center it before final addition for 16x16 IM
bitset<32> pad_32bit(bitset<16> bc_ad)
{
    bitset<32> ans;

    for (int i = 15; i >= 0; i--)
    {
        ans[i + 8] = bc_ad[i]; // get into the form 0000 bc_ad 0000
    }
    return ans;
}

//finds and outputs the timing in delta T based off of the number of 
//4x4s used
int calculate_timing(const uint8_t multiplications, const uint8_t additions)
{

    int deltaT;

    //*Note:
    //delta T responsible from 4x4s -> run up to 4 in parallel ->21 dT each 
    //16x16 will always have 10 additions
    //8x8 will always have 2 10 additions
    //thus we can branch based off of the number of additions

    //4x4 case, additions = 0 since 4x4 gives us the full answer
    if (additions == 0)
    {
 
        deltaT = (21 * ((multiplications - 4) / 4) + 21);

    }

    //8x8 case
    // we have a 12bit addition and a 16bit addition
    // with CLA v.2 -> 17dT for the sum of 12bit, and 21dT for the sum of the 16bit 
    else if (additions == 2)
    {
        deltaT = (21 * ((multiplications - 4) / 4) + 21) + 38; 
    }
    // 16x16case
    // we have 4 8x8 additions = 4*38 (4 12bit and 4 16bit)
    // then one 24 bit addition and a 32 bit addition 29dt for the sum of 24 bit and 37 dT for the 32bit addition
    // 10 additions
    else
    {
        deltaT = (21 * ((multiplications - 4) / 4) + 21) + (4*38) + 66;
    }

   
    return deltaT;
}
