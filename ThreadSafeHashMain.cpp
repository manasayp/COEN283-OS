#include <algorithm>
#include <pthread.h>
#include "hashTable.h"
#include "hashUtils.h"
#include <string>

#define MAX_THREADS 500
using namespace std;

hashTable *threadSafeHash;

/*thread arguments*/
struct args
{
    int numOperations;
    vector<string> song_list;
    int songListLength;
    pthread_mutex_t rw_lock;
};

struct args Main_Thread_Args;

/*Function given during Thread create. Each thread processes the song list for number of operations time*/
void *hashOperations(void *threadId)
{
    long tid;
    op_t random_operation_type;
    tid = (long)threadId;

    string s[Main_Thread_Args.songListLength];
    int i = 0;
    vector<string>::iterator it = Main_Thread_Args.song_list.begin();

    for(i = 0;i< Main_Thread_Args.songListLength;i++,it++)
    {
    	s[i] = *it;
    }

    int numOps = Main_Thread_Args.numOperations;
    i = 0;

    while(numOps)
    {
        random_operation_type = findRandomOpType(table_sz);

	unsigned int hash_key = stringFold(s[i]);

    	int hash_index;

	if(random_operation_type == OP_TYPE_GET)
	{
	    hash_index = threadSafeHash->get(hash_key, s[i]);
	    if(hash_index != -1)
	    {
	        pthread_mutex_lock(&Main_Thread_Args.rw_lock);
                cout << " Thread Id: " <<tid <<" Operation : GET  Song name : " <<  s[i] << " Hash Key : " << hash_key;
                cout << " Success , Song found in index : " << hash_index <<endl;
		pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
	    }
	    else
	    {
		pthread_mutex_lock(&Main_Thread_Args.rw_lock);
		cout << " Thread Id : " << tid;
                cout << " Operation : GET Song name : " << s[i] << " Hash Key : " << hash_key;
                cout << " Failed, Song is not found in the Hash Table" <<endl;
		pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
	    }
	}
	else
	{
	    hash_index = threadSafeHash->put(hash_key, s[i]);

	    if(hash_index != -1)
	    {
	        pthread_mutex_lock(&Main_Thread_Args.rw_lock);
                cout << " Thread Id :" << tid;
		cout << " Operation : PUT  Song name : " << s[i] << " Hash Key : " << hash_key;
		cout << " Success, Song added to Hash Table at index : " << hash_index <<endl;
		pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
	    }
	    else
	    {
		pthread_mutex_lock(&Main_Thread_Args.rw_lock);
                cout << " Thread Id :" << tid;
		cout << " Operation : PUT  Song name : " << s[i] << " Hash Key : " << hash_key;
		cout << " Failed, Song already exists in the Hash Table" << endl;
		pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
	    }
        }
	numOps--;
	i++;
	cout << endl;
    }

    pthread_mutex_lock(&Main_Thread_Args.rw_lock);
    cout << endl << "************ Thread Id : " << tid <<  " Terminating ***********" << endl;
    pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
    pthread_exit(NULL);
}

/*Main Function*/
int main()
{
    time_t t;

    /* Intialize random number generator */
    srand((unsigned int) time(&t));
    
    string temp;
    int num_of_threads = 0,rc;
    std::getline(std::cin,temp);
    
    while((temp[0] == '#') | (temp[0] == '/'))
    {
       std::getline(std::cin,temp);
    }

    num_of_threads = atoi(temp.c_str());
    temp.clear();

    if(num_of_threads > MAX_THREADS)
    {
        cerr << "Main: Maximum Number of Threads allowed is  " << MAX_THREADS << endl;
        exit(-1);
    }

    if(num_of_threads <= 0)
    {
       cerr << "Main : Invalid Input : Number of thread " << num_of_threads <<  endl;
       exit(-1);
    }
  
    std::getline(std::cin,temp);
   
    while((temp[0] == '#') | (temp[0] == '/'))
    {
        std::getline(std::cin,temp);
    }

    Main_Thread_Args.numOperations = atoi(temp.c_str());
    temp.clear();

    long int i = 0;

    while(std::getline(std::cin,temp))
    {
        if((temp[0] != '#') && (temp[0] != '/'))
	{
	     temp.erase(remove(temp.begin(),temp.end(), '\"'),temp.end());
	     Main_Thread_Args.song_list.push_back(temp);
	}
    }

    Main_Thread_Args.songListLength = Main_Thread_Args.song_list.size();

    if(Main_Thread_Args.songListLength < Main_Thread_Args.numOperations)
    {
         cerr << "Main: Number of songs in the list is less than number of operations per thread...exiting" <<endl;
	 exit(-1);
    }
    
    if(Main_Thread_Args.songListLength < 1)
    {
        cerr << "Main: No Song List specified.......exiting" << endl;
        exit(-1);
    }

    if(Main_Thread_Args.numOperations < 1)
    {
       cerr << "Main: Invalid Input: Num of operations  " << Main_Thread_Args.numOperations << "........exiting" << endl;
       exit(-1);
    }

    pthread_t threads[num_of_threads];

    cout <<endl <<endl;
    table_sz = compute_ht_size(Main_Thread_Args.songListLength);
    threadSafeHash = new hashTable(table_sz);

    if (pthread_mutex_init(&Main_Thread_Args.rw_lock, NULL) != 0)
    {
        cout << "Main: mutex init failed\n";
	exit(-1);
    }

    for(i=0; i< num_of_threads; i++)
    {
        pthread_mutex_lock(&Main_Thread_Args.rw_lock);

	cout << "Main: creating thread " << i << endl;
        rc = pthread_create(&threads[i], NULL, hashOperations, (void *)i);
	if (rc)
	{
	    cout << "Main: ERROR; return code from pthread_create() is " << rc << endl;
	    pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
	    exit(-1);
	}
	pthread_mutex_unlock(&Main_Thread_Args.rw_lock);
    }
	
    for(i=0; i< num_of_threads; i++)
    {
        (void) pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&Main_Thread_Args.rw_lock);
    return 0;
}

