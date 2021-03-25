#pragma once
// Declare functions here

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

// General functions
void simulate_IM(bitset<16> op1, bitset<16> op2); // Simulate Iterative Method (IM) 16x16
bitset<16> simulateIM_8x8(bitset<8> op1, bitset<8> op2); // Simulate 8x8 iterative method used with 16x16 version

int getLength(bitset<16> op); // Get length of operand
int getRoundedLength(int lenOp1, int lenOp2); // Get rounded lengths of operator either 8 or 16

bitset<8> getLeft8bits(bitset<16> op); // Get the high order 8 bits from a 16 bit operand
bitset<8> getRight8bits(bitset<16> op); // Get the low order 8 bits from a 16 bit operand
bitset<4> getLeft4bits(bitset<8> op); // Get the high order 4 bits from an 8 bit operand
bitset<4> getRight4bits(bitset<8> op); // Get the low order 4 bits from an 8 bit operand

bitset<8> multiplier4x4(bitset<4> op1, bitset<4> op2); // Simulate 4x4 mulitplier

bitset<16> concat16_ac_bd(bitset<8> ac, bitset<8> bd); // Concatinate "ac" and "bd" terms for 8x8 IM
bitset<32> concat32_ac_bd(bitset<16> ac, bitset<16> bd); // Concatinate "ac" and "bd" terms for 16x16 IM

bitset<16> pad_16bit(bitset<8> bc_ad); // Shift left bc_ad to center it before final addition for 8x8 IM
bitset<32> pad_32bit(bitset<16> bc_ad); // Shift left bc_ad to center it before final addition for 16x16 IM
