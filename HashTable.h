#ifndef __HASH_TABLE__
#define __HASH_TABLE__
#include<iostream>
#include<string>

using namespace std;

class hashNodes
{
    unsigned int hash_key;
    string song_name;
    hashNodes *next;
public:
    hashNodes(unsigned int hash_key,string song_name);
    string getValue();
    unsigned int getKey();
    hashNodes* getNext();
    void setValue(string song_name);
    void setNext(hashNodes *next);
};

class hashTable
{
    hashNodes **table;
	int hashTableSz;
	pthread_mutex_t hashlock;
public:
    hashTable(int table_size);
    int get(unsigned int &hash_key,string &song_name);
    int put(unsigned int &hash_key,string &song_name);
    void printContents();
   ~hashTable();
};
#endif
   
  
