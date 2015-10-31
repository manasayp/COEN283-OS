#ifndef __HASH_UTILS__
#define __HASH_UTILS__
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<iostream>
#include<string>
#include<time.h>
#include<vector>

using namespace std;

typedef enum op_t
{	
    OP_TYPE_GET = 0,
    OP_TYPE_PUT = 1
} op_t;

extern unsigned int table_sz;

bool isPrime(unsigned int f);
unsigned int findNextPrime(unsigned int f);
unsigned int compute_ht_size(unsigned int song_length);
unsigned int stringToInt(string s, unsigned int start_pos);
unsigned int reverseBits(unsigned int num);
unsigned int stringFold(string s);
unsigned int randomFold(unsigned int num, unsigned int bit_chunk_len);
op_t findRandomOpType(unsigned int table_size);
#endif
