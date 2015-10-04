#include"hashUtils.h"
#include"ThreadSafeHash.cpp"

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

	for(int i = 0; i < num_of_operations; i++)
	{
        std::getline(std::cin,temp);
		song_list.push_back(temp);
	}

	songListLength = song_list.size();
	table_sz = compute_ht_size(songListLength);
	hashTable threadSafeHash(table_sz);

	vector<string>::iterator it = song_list.begin();
	while(num_of_operations)
	{
		random_operation_type = findRandomOpType(table_sz);

		if(random_operation_type == OP_TYPE_GET)
		{
			unsigned int hash_key = stringFold(*it);
		}
		else
		{

		}
	}
    getchar();
    return 0;
}

