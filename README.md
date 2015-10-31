# ThreadSafeHash
COEN283 - Operating Systems Programming Assignment-0
Thread Safe Hash Table Implementation using C++ and pthread library
The input consists of 
1) number of threads to run concurrently, 
2) number of operations (put or get chosen randomly) a thread need to handle, and
3) a list of sound names. 

Operation is chosen depending on the MSB of a 32 bit random number folded into a f-bit number where f = ceil(lg s) 
The input to the hash function is a text string, and the output of the hash function is an unsigned integer.
The input string is divided into 4-byte chucks (fill with zeros if the last chunk is not full), reverse the bits for all odd numbered chunks, exclusive OR of all those chunks together, the result 
value is then mod by the table size to get hash index, and return the index. 

Hash table is implemented as an array of pointers,where each pointer points to a linked list holding all the song names
hashed to the particular array index. pthread api's are used to implement threads and to implement lock/unlock mechanism.

lock implementation :
1. One lock per linked list is used
2. lock/unlock for output

Each thread process the list of sounds one by one for number of operations (the second input number) times, then exit. 







