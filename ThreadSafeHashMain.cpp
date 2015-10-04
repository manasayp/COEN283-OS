#include"hashUtils.h"
#include<algorithm>
#include"HashTable.h"

using namespace std;

int main()
{
	time_t t;
	op_t random_operation_type;

	/* Intialize random number generator */
    srand((unsigned int) time(&t));

    int num_of_threads = 0,num_of_operations = 0;
	int songListLength = 0;
    cin >> num_of_threads;
    cin >> num_of_operations;
    string temp;
	vector<string> song_list;

	std::getline(std::cin,temp);
	int i = 0;
	//while(!cin.eof())
	while(i < num_of_operations)
	{
		std::getline(std::cin,temp);
		if(temp.front() != '#')
		{
			temp.erase(remove(temp.begin(),temp.end(), '\"'),temp.end());
		    song_list.push_back(temp);
		}
		//std::getline(std::cin,temp);
		i++;
	}

	songListLength = song_list.size();

	if(songListLength < num_of_operations)
	{
		cout << "Number of songs in the list is less than number of operations per thread...exiting" <<endl;
		return 0;
	}

	cout <<endl <<endl;
	table_sz = compute_ht_size(songListLength);
	hashTable threadSafeHash(table_sz);

	vector<string>::iterator it = song_list.begin();
	while(num_of_operations)
	{
		random_operation_type = findRandomOpType(table_sz);
		unsigned int hash_key = stringFold(*it);
		int hash_index;
		
		if(random_operation_type == OP_TYPE_GET)
		{
		   cout << "Thread Id : Operation : GET  Song name : " << *it <<endl;
		   if((hash_index = (threadSafeHash.get(hash_key,*it))) != -1)
		   {
			   cout << "Success , Song found in index : " << hash_index <<endl;
		   }
		   else
		   {
			   cout << "Failed, Song is not found in the Hash Table" <<endl;
		   }
		}
		else
		{
			cout << "Thread Id : Operation : PUT  Song name : " << *it <<endl;
			if((hash_index = (threadSafeHash.put(hash_key,*it))) != -1)
			{
				cout << "Success, Song added to Hash Table at index : " << hash_index <<endl;
			}
			else
			{
				cout << "Failed, Song already exists in the Hash Table" << endl;
			}
		}
		num_of_operations--;
		it++;
		cout <<endl;
	}
	cout <<endl;
	
	/*int hash_index;
	for(it = song_list.begin(); it != song_list.end(); it++)
	{
		unsigned int hash_key = stringFold(*it);
	    if((hash_index = (threadSafeHash.get(hash_key,*it))) != -1)
		{
			cout << " song : " << *it << "found at index : " << hash_index << "and hashed value is " << hex << hash_key << endl;
		}
		else
		{
			cout << "song not found" <<endl;
		}
		cout <<endl;
	}*/
    getchar();
    return 0;
}

