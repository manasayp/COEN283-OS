#if 1
#include"hashUtils.h"

using namespace std;
 
unsigned int table_sz =  0;

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
	return findNextPrime(song_length);
}

unsigned int stringToInt(string s, unsigned int start_pos)
{
	unsigned int result = 0,i = 0, str_length;

	str_length = s.length();
	
	/* Extract 4 characters and place them in to integer */
	while(((start_pos + i) < str_length) && (i < 4))
	{
	   result <<= 8;
	   result |= (s[start_pos + i]);
	   i++;
	}
	
	/* If end of string, compensate by filling 0s */
	if(i < 4)
	{
	   result = result << ((4-i) * 8);
	}

	return(result);
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

unsigned int stringFold(string s)
{
	unsigned int result = 0,foldedStr = 0,i = 0;
	int strLength = s.length();
	int chunkNum = 0;

	cout << s <<endl;
	
	for(i = 0; i < strLength; i += 4)
	{
		result = stringToInt(s,i);
		
		if(++chunkNum & 1)
		{
			foldedStr ^= reverseBits(result);
		}
		else
		{
			foldedStr ^= result;
		}	
	}
	return foldedStr;
}

unsigned int randomFold(unsigned int num, unsigned int bit_chunk_len)
{
	unsigned int result = 0;
	unsigned int mask = 0;
	
	if(bit_chunk_len>=32)
	{
		/* Large number, return incoming number*/
		return num;
	}
	else
	{
		/*Generate mask for bit chunk*/
		mask = (1 << bit_chunk_len) - 1;
		
		/* Repeat for all chunks */
		while(num)
		{
			/* Ex-OR the extracted chunks*/
			result = result ^ (num & mask);
			/* Prepare for next chunk*/
			num = num >> bit_chunk_len;
		}
	}
	
	return result;
}

op_t findRandomOpType(unsigned int table_size)
{
	int randon_num;
	op_t result_op = OP_TYPE_GET;
	unsigned int fold;
	
	fold = ceil(log((double)table_sz));
	randon_num = (int)rand();

	/* Find MSB of random folded result */
	if(randomFold((unsigned int)randon_num,fold) & (1 << (fold-1)))
	{
		result_op = OP_TYPE_PUT;
		printf("PUT\n");
	}
	else
	{
	    result_op = OP_TYPE_GET;
		printf("GET\n");
	}
	return result_op;
}
#if 0
int main()
{
	time_t t;
	op_t random_operation_type;

	/* Intialize random number generator */
    srand((unsigned int) time(&t));

	/* Determine List Length after parsing the input */
	int list_length = 8;
	
	table_sz = compute_ht_size(list_length);
	/* Find the random operation type */

	random_operation_type = findRandomOpType(table_sz);

	unsigned int hash_key = stringFold("Listen to the music");  // Answer should be 946358539
	
	printf("Hash Index = %d\n",hash_key);
	return 0;
}
#endif
#endif