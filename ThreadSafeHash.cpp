#include<stdio.h>
#include<string>

using namespace std;

class hashNodes
{
    unsigned int hash_key;
    string song_name;
    hashNodes *next;
public:
        hashNodes(unsigned int hash_key,string song_name)
        {
           this->hash_key = hash_key;
           this->song_name = song_name;
           this->next = NULL;
        }
    
        string getValue()
        {
           return song_name;
        }
    
        unsigned int getKey()
        {
           return hash_key;
        }

        hashNodes* getNext()
        {
           return next;
        }

        void setValue(string song_name)
        {
           this->song_name = song_name;
        }

        void setNext(hashNodes *next)
        {
           this->next = next;
        }
};

class hashTable
{
    hashNodes **table;
	int hashTableSz;

public:
    hashTable(int table_size) 
    {
	    this->hashTableSz = table_size;
        table = new hashNodes*[table_size];
        for(int i = 0; i < table_size; i++)
        {
            table[i] = NULL;
        }
   }

   int get(unsigned int &hash_key,string &song_name)
   {
       int hash_index;
	   hash_index = hash_key % hashTableSz;
       hashNodes *entry = table[hash_index];
       while(entry != NULL)
       {
           if(song_name.compare(entry->getValue())  == 0)
           { 
               song_name = entry->getValue();
               return hash_index;
           }
           entry = entry->getNext();
       } 
       return -1;   
    }

    void put(unsigned int &hash_key,string &song_name)
    {
        int hash_index = hash_key % hashTableSz;
        hashNodes *prev = NULL;
        hashNodes *entry = table[hash_index];
 
        while(entry != NULL && entry->getKey() != hash_key)
        {
            prev = entry;
            entry = entry->getNext();
        }

        if(entry == NULL)
        {
            entry = new hashNodes(hash_key,song_name);
            if(prev == NULL)
            {
                table[hash_index] = entry;
            }
            else
            {
                prev->setNext(entry);
            } 
        }
    }
   ~hashTable()
   {
       for(int i = 0; i < 128; i++)
       {
           if(table[i] != NULL)
           {
               hashNodes *prev = NULL;
               hashNodes *entry = table[i];
               while(entry != NULL)
               {
                   prev = entry;
                   entry = entry->getNext();
                   delete prev;
               }
           }
       }
       delete[] table;
    }
};

   
  
