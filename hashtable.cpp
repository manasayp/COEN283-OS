#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>

using namespace std;

unsigned int table_sz;

bool isPrime(unsigned int f)
{
	int count = 0;
	if(f == 2 || f == 3)
		return true;

	if(f % 2 == 0  || f % 3  == 0)
		return false;

	for(int j = 4; j < f; j++)
	{
		if(f % j == 0)
		{
			count++;
			break;
		}
	}
	if(count == 0)
		return true;
	return false;
}

unsigned int findNextPrime(unsigned int f)
{
	if(isPrime(f))
		return f;

	while(!isPrime(++f))
	{
	}
	return f;
}
		
unsigned int compute_ht_size(unsigned int song_length)
{
	unsigned int table_sz = findNextPrime(song_length);
	return table_sz;
}
string string_to_hex(string& input)
{
    const char* hex_num = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(hex_num[c >> 4]);
        output.push_back(hex_num[c & 0xF]);
    }
    return output;
}

long* split_chunk(string s,int chunk_size,int num_chunks)
{
	int strLength = s.length(), j = 0, n;
	
	string *chunks = new string[num_chunks];

	for(int i = 0 ; i < strLength; i += chunk_size)
	{
		if (i + chunk_size > strLength)
		{
			chunks[j] =  s.substr(i,(strLength  - i));
			int k = chunk_size - chunks[j].length();
			while(k--)
				chunks[j].push_back(0);
			j++;
		}
		else
		    chunks[j++] =  s.substr(i,chunk_size);
	}

	long *blocks = (long *)new long[num_chunks];
	
	for(int i = 0 ; i < num_chunks; i ++)
	{
		cout << chunks[i] << endl;
	    blocks[i] = std::stol(string_to_hex(chunks[i]),nullptr,16);
		cout << blocks[i] <<endl;
	}

	if(chunks)
	{
		delete[] chunks;
	}
	return blocks;
}

unsigned int reverseBits(unsigned int num)
{
	unsigned int count = sizeof(num) * 8 - 1;
	unsigned int reverse_num = num;
	
	num >>= 1; 
	while(num)
	{
	reverse_num <<= 1;	 
	reverse_num |= num & 1;
	num >>= 1;
	count--;
	}
	reverse_num <<= count;
	return reverse_num;
}

unsigned long xor_fold(long *blocks,int n)
{
	unsigned long result = 0;

	for(int i = 0; i < n; i++)
	{
		result ^= blocks[i];
	}
	return result;
}

int hash_key(string s)
{
	int strLength = s.length(), n;
	if(strLength%4  == 0)
	    n = (strLength/4);
	else
		n = (strLength/4)+1;

	long *chunks= split_chunk(s,4,n);
	
	for(int i = 0; i < n ;i += 2)
	{
		chunks[i] = reverseBits(chunks[i]);
		cout << chunks[i] << "\n";
	}

	unsigned long result = xor_fold(chunks,n);

	if(chunks)
	{
		delete[] chunks;
	}
	return result % table_sz;
}

int * split_random(int r,int f, int n)
{
	return 0;
}
short find_op_type(int l)
{
	unsigned int arr[] = {0,1,0,7,0,31,0,127,0,0,0,2047,0,8191,0,0,0,131071,0,524287,0,0,0,8388607,0,0,0,0,0,536870911,0,2147483647};
	int r = (int)rand();
	int f = findNextPrime(ceil(log((double)l)));

	int n,result = 0;
	if(32 % f == 0)
		n = 32/f;
	else
		n = (32/f)+1;

	int *a = new int[n], k = 0;

	for(int i = 0; i < n; i++)
	{
		a[i] = (r >> k) & arr[f];
		k = k+f;
		cout << a[i] << endl;
	}

	for(int i = 0; i < n; i++)
	{
		result ^= a[i];
	}

	if((result >> (f-1)) & 1)
		printf("PUT");
	else
		printf("GET");
	return 0;
}

int main()
{
	table_sz = compute_ht_size(8);
	printf("%d\n",hash_key("Listen to the music"));
	find_op_type(45);
	return 0;
}