#include "hashTable.h"

using namespace std;

/*New Node Creation*/
hashNodes::hashNodes(unsigned int hash_key,string song_name)
{
   this->hash_key = hash_key;
   this->song_name = song_name;
   this->next = NULL;
}

/*returns song name*/    
string hashNodes::getValue()
{
   return song_name;
}
 
/*returns the hash key*/   
unsigned int hashNodes::getKey()
{
   return hash_key;
}

/*returns the next node pointer*/
hashNodes* hashNodes::getNext()
{
   return next;
}

/*sets the song anme*/
void hashNodes::setValue(string song_name)
{
   this->song_name = song_name;
}

/*sets the next pointer*/
void hashNodes::setNext(hashNodes *next)
{
   this->next = next;
}

/*constructor - hash table*/
hashTable::hashTable(int table_size) 
{
    pthread_mutex_t newmutex[table_size]; 
    this->hashlock = new pthread_mutex_t[table_size];

    for(int i = 0; i < table_size; i++)
    {
	 pthread_mutex_init(&newmutex[i], NULL);
	 this->hashlock[i] = newmutex[i]; 
    }

    this->hashTableSz = table_size;
    table = new hashNodes*[table_size];
    for(int i = 0; i < table_size; i++)
    {
        table[i] = NULL;
    }
}

/*finds if the song name exists in hash table and returns if song is found else returns -1*/
int hashTable::get(unsigned int &hash_key,string &song_name)
{
    int hash_index;
    hash_index = hash_key % hashTableSz;
    pthread_mutex_lock(&this->hashlock[hash_index]);
    hashNodes *entry = table[hash_index];
    while(entry != NULL)
    {
        if(song_name.compare(entry->getValue())  == 0)
        { 
            song_name = entry->getValue();
            pthread_mutex_unlock(&this->hashlock[hash_index]);
            return hash_index;
         }
         entry = entry->getNext();
    } 
    pthread_mutex_unlock(&this->hashlock[hash_index]);
    return -1;   
}

/*adds song name to hash tale at the index computed by hash key*/
int hashTable::put(unsigned int &hash_key,string &song_name)
{
    int hash_index = hash_key % hashTableSz;
    int ret_index = hash_index;
    pthread_mutex_lock(&this->hashlock[hash_index]);

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
    else if(entry->getValue().compare(song_name) == 0)
    {
	 ret_index = -1;
    }/*checks if the song name already exists in the hash table*/

    pthread_mutex_unlock(&this->hashlock[hash_index]);
    return ret_index;
}

/*deallocate memory */
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

/*prints the content of hash table*/
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

  
