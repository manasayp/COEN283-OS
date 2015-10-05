#include<algorithm>
#include<pthread.h>
#include "hashTable.h"
#include "hashUtils.h"

#define MAX_THREADS 500
using namespace std;

hashTable *threadSafeHash;

struct args
{
	int numOperations;
	vector<string> song_list;
	int songListLength;
	pthread_mutex_t rw_lock;
};

struct args Main_Thread_Args;

void *hashOperations(void *threadId)
{
	long tid;
	op_t random_operation_type;
    tid = (long)threadId;
	vector<string>::iterator it = Main_Thread_Args.song_list.begin();
	int numOps = Main_Thread_Args.numOperations;

	while(numOps)
	{
	    random_operation_type = findRandomOpType(table_sz);
		unsigned int hash_key = stringFold(*it);
		int hash_index;

		if(random_operation_type == OP_TYPE_GET)
		{
			 pthread_mutex_lock(&Main_Thread_Args.rw_lock);
			 cout << "Thread Id : " << tid << " Operation : GET  Song name : " << *it <<endl;
			 pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
			 hash_index = threadSafeHash->get(hash_key,*it);
			 if(hash_index != -1)
			 {
				 pthread_mutex_lock(&Main_Thread_Args.rw_lock);
				 cout << "Success , Song found in index : " << hash_index <<endl;
				 pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
			 }
			 else
			 {
				pthread_mutex_lock(&Main_Thread_Args.rw_lock);
				cout << "Failed, Song is not found in the Hash Table" <<endl;
				pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
			 }
	    }
	    else
	    {
	    	pthread_mutex_lock(&Main_Thread_Args.rw_lock);
			cout << "Thread Id : " << tid << " Operation : PUT  Song name : " << *it <<endl;
			pthread_mutex_unlock(&Main_Thread_Args.rw_lock);

			hash_index = threadSafeHash->put(hash_key,*it);

			if(hash_index != -1)
			{
				pthread_mutex_lock(&Main_Thread_Args.rw_lock);
				cout << "Success, Song added to Hash Table at index : " << hash_index <<endl;
				pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
			}
			else
			{
				pthread_mutex_lock(&Main_Thread_Args.rw_lock);
				cout << "Failed, Song already exists in the Hash Table" << endl;
				pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
			}
		}
		numOps--;
		it++;
		cout <<endl;
	}

	pthread_mutex_lock(&Main_Thread_Args.rw_lock);
	cout << "Thread " << tid << "Terminating" << endl;
	pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
    pthread_exit(NULL);
}

int main()
{
	time_t t;

	/* Intialize random number generator */
    srand((unsigned int) time(&t));

    int num_of_threads = 0,rc;
	//int songListLength = 0;
    cin >> num_of_threads;
    cin >> Main_Thread_Args.numOperations;
    string temp;
	//vector<string> song_list;


	std::getline(std::cin,temp);
	long int i = 0;
	//while(!cin.eof())
	while(i < Main_Thread_Args.numOperations)
	{
		std::getline(std::cin,temp);
		if(temp.at(0)!= '#')
		{
			temp.erase(remove(temp.begin(),temp.end(), '\"'),temp.end());
			Main_Thread_Args.song_list.push_back(temp);
		}
		//std::getline(std::cin,temp);
		i++;
	}

	Main_Thread_Args.songListLength = Main_Thread_Args.song_list.size();

	if(Main_Thread_Args.songListLength < Main_Thread_Args.numOperations)
	{
		cout << "Number of songs in the list is less than number of operations per thread...exiting" <<endl;
		getchar();
		return 0;
	}

	if(num_of_threads > MAX_THREADS)
	{
		cout << "Maximum thread support exceeded" << endl;
		getchar();
		exit(-1);
	}

	pthread_t threads[num_of_threads];

	cout <<endl <<endl;
	table_sz = compute_ht_size(Main_Thread_Args.songListLength);
	threadSafeHash = new hashTable(table_sz);

	if (pthread_mutex_init(&Main_Thread_Args.rw_lock, NULL) != 0)
	{
	     cout << " mutex init failed\n";
	     getchar();
	     return 1;
	}

	for(i=0; i< num_of_threads; i++)
	{
	     printf("In main: creating thread %ld\n", i);
	     rc = pthread_create(&threads[i], NULL, hashOperations, (void *)i);
	     if (rc){
	        printf("ERROR; return code from pthread_create() is %d\n", rc);
	        getchar();
	        exit(-1);
	     }
	}

	//cout <<endl;
	
	//threadSafeHash->printContents();
	getchar();getchar();
	int m;
	cin >> m;
    return 0;
}

