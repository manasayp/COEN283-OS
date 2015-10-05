#include "hashTable.h"

using namespace std;

hashNodes::hashNodes(unsigned int hash_key,string song_name)
{
     this->hash_key = hash_key;
     this->song_name = song_name;
     this->next = NULL;
}
    
string hashNodes::getValue()
{
     return song_name;
}
    
unsigned int hashNodes::getKey()
{
     return hash_key;
}

hashNodes* hashNodes::getNext()
{
    return next;
}

void hashNodes::setValue(string song_name)
{
    this->song_name = song_name;
}

void hashNodes::setNext(hashNodes *next)
{
    this->next = next;
}

hashTable::hashTable(int table_size) 
{
	    pthread_mutex_t newmutex;
	    pthread_mutex_init(&newmutex, NULL);
	    this->hashlock = newmutex;
	    this->hashTableSz = table_size;
        table = new hashNodes*[table_size];
        for(int i = 0; i < table_size; i++)
        {
            table[i] = NULL;
        }
}

int hashTable::get(unsigned int &hash_key,string &song_name)
   {
       int hash_index;
	   hash_index = hash_key % hashTableSz;
	   pthread_mutex_lock(&this->hashlock);
       hashNodes *entry = table[hash_index];
       while(entry != NULL)
       {
           if(song_name.compare(entry->getValue())  == 0)
           { 
               song_name = entry->getValue();
               pthread_mutex_unlock(&this->hashlock);
               return hash_index;
           }
           entry = entry->getNext();
       } 
       pthread_mutex_unlock(&this->hashlock);
       return -1;   
    }

int hashTable::put(unsigned int &hash_key,string &song_name)
    {
        int hash_index = hash_key % hashTableSz;
        pthread_mutex_lock(&this->hashlock);

        hashNodes *prev = NULL;
        hashNodes *entry = table[hash_index];
 
        while((entry != NULL) && (entry->getValue().compare(song_name) != 0))
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
		else if(entry->getValue().compare(song_name) != 0)
		{
			hash_index = -1;
		}
        pthread_mutex_unlock(&this->hashlock);
		return hash_index;
    }


hashTable::~hashTable()
   {
       for(int i = 0; i < hashTableSz; i++)
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

void hashTable::printContents()
 {
	 for(int i = 0; i < hashTableSz; i++)
     {
		 if(table[i] != NULL)
		 {
			 hashNodes *entry = table[i];
			 cout << "Contents in Hash Table index : " << i << endl;
			 while(entry != NULL)
			 {
				 cout << "Song name : " << entry->getValue() << "\t Hash Key : " << entry->getKey() << endl;
				 entry = entry->getNext();
			 }
			 cout << endl;
		 }
	 }
 }

  
