#ifndef FLATHASH_H
#define FLATHASH_H

#include <iostream>

// Flag(hint) for overflow handling
enum overflow_handle {
  LINEAR_PROBING = 0,
  QUADRATIC_PROBING
};

class FlatHash
{
private:
  unsigned int* hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size of hash table
  unsigned int table_size;
  // Nums of keys
  unsigned int num_of_keys;
  // Tobmstone
  unsigned int Tombstone;

public:
  FlatHash(enum overflow_handle _flag, float _alpha);

  ~FlatHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  unsigned int* getTablePointer() { return hashtable; }

  // Return time cost
  int insert(const unsigned int key);

  int insert_part(const unsigned int key);

  // Remove function in lecture. Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();

  void shift(unsigned int index, unsigned int key);

  void resize();
};

FlatHash::FlatHash(enum overflow_handle _flag, float _alpha)
{
  // Initial table size is 1000 
  table_size = 1000;
  num_of_keys = 0;
  flag = _flag;
  alpha = _alpha;
  
  Tombstone = 1000001;
  hashtable = new unsigned int[table_size];
  for (unsigned int i = 0; i < table_size; i++)
    hashtable[i] = 0;

}

FlatHash::~FlatHash()
{
  if (hashtable != NULL){
  delete[] hashtable;
  hashtable = NULL;
  }
}

int FlatHash::insert_part(const unsigned int key){
  
  int num_probs = 0;
  unsigned int index = hashFunction(key);

  if (flag == LINEAR_PROBING){  
    for (unsigned int i = 0; i < table_size; i++){
      unsigned int new_index = (index + i) % table_size;
      num_probs++;
      
      if (hashtable[new_index] == 0){
        hashtable[new_index] = key;
        num_of_keys++;
        break;
      }
      else if (hashtable[new_index] == key){
        num_probs = -1 * num_probs;
        break;
      }
    }
  }

  else if (flag == QUADRATIC_PROBING){
    bool check = false;
    for (unsigned int i = 0; i < table_size; i++){
      unsigned int new_index = (index + i*i) % table_size;
      num_probs++;
      if (hashtable[new_index] == 0 || hashtable[new_index] == Tombstone){
        hashtable[new_index] = key;
        num_of_keys ++;
        check = true;
        break;
      }
      else if (hashtable[new_index] == key){
        check = true;
        num_probs = -1 * num_probs;
        break;
      }
      
    }

    if (check == false){
      for (unsigned int i = 0; i < table_size; i++){
        num_probs++;
        unsigned int new_index = (index + i) % table_size;
        if (hashtable[new_index] == 0 || hashtable[new_index] == Tombstone){
          hashtable[new_index] = key;
          num_of_keys ++;
          break;
        }
        else if (hashtable[new_index] == key){
          num_probs = -1 * num_probs;
          break;
        }
      }
    }
  }

  return num_probs;
}

int FlatHash::insert(const unsigned int key)
{
  int time_cost = insert_part(key);
  
  float temp_alpha = float(num_of_keys) / table_size;
  if (temp_alpha >= alpha)
    resize();

  return time_cost;

}

void FlatHash::resize(){

    unsigned int initial_table_size = table_size;
    table_size = 2 * table_size;
    int unsigned* temp_hashtable = new unsigned int[table_size];
    
    for (unsigned int i = 0; i < table_size; i++){
      temp_hashtable[i] = 0;
    }

    if (flag == LINEAR_PROBING){

      for (unsigned int i = 0; i < initial_table_size; i++){
        if (hashtable[i] != 0){
        
          unsigned int index = hashFunction(hashtable[i]);
  
          for (unsigned int j = 0; j < table_size; j++){
            unsigned int new_index = (index + j) % table_size;
            if (temp_hashtable[new_index] == 0){
              temp_hashtable[new_index] = hashtable[i];
              break;
            }   
          }
       }
      }
    }
    
    else if (flag == QUADRATIC_PROBING){
      for (unsigned int i = 0; i < initial_table_size; i++){
        if (hashtable[i] != 0 && hashtable[i] != Tombstone){
          bool check = false;
          unsigned int index = hashFunction(hashtable[i]);
  
          for (unsigned int j = 0; j < table_size; j++){
            unsigned int new_index = (index + j*j) % table_size;
            if (temp_hashtable[new_index] == 0){
              temp_hashtable[new_index] = hashtable[i];
              check = true;
              break;
            }   
          }

          if (check == false){

            for (unsigned int j = 0; j < table_size; j++){
              unsigned int new_index = (index + j) % table_size;
              if (temp_hashtable[new_index] == 0){
                temp_hashtable[new_index] = hashtable[i];
                break;
              }
            }
          }
       }
      }
    }

  delete[] hashtable;
  hashtable = temp_hashtable;
}

void FlatHash::shift(unsigned int index, unsigned int key){

    unsigned int next = (index + 1) % table_size;
    while (hashtable[next] != 0){

      for (unsigned int i = 0 ; i < table_size; i++){

        unsigned int temp_index = (next + i) % table_size;
      
        if (hashtable[temp_index] == 0)
          return;

        unsigned int temp_key = hashtable[temp_index];
        unsigned int location = hashFunction(temp_key);
        
        for (unsigned int i = 0; i < table_size; i++){
          unsigned int new_index = (location + i) % table_size;
      
          if (hashtable[new_index] == 0){
            hashtable[new_index] = temp_key;
            break;
          }
          else if (hashtable[new_index] == temp_key)
            break;
        }

        if (hashtable[index] != 0){
          hashtable[temp_index] = 0;
          index = temp_index;
          next = (temp_index + 1) % table_size;
          break;
        }
      }
    }
}

int FlatHash::remove(const unsigned int key)
{
  unsigned int index = hashFunction(key);
  int num_probs = 0;
  if (flag == LINEAR_PROBING){
      for (unsigned int i = 0; i < table_size; i++){
        num_probs++;
        unsigned int new_index = (index + i) % table_size;
        if (hashtable[new_index] == key){
          hashtable[new_index] = 0;
          shift(new_index, key);
          num_of_keys--;
          break;
        }
        else if (hashtable[new_index] == 0){
          num_probs = -1 * num_probs;
          break;
        }
      }
  }


  else if (flag == QUADRATIC_PROBING){
    
      bool check = false;
      for (unsigned int i = 0; i < table_size; i++){
        num_probs++;
        unsigned int new_index = (index + i*i) % table_size;
        if (hashtable[new_index] == key){
          hashtable[new_index] = Tombstone;
          num_of_keys--;
          check = true;
          break;
        }
        else if (hashtable[new_index] == 0){
          check = true;
          num_probs = -1 * num_probs;
          break;
        }
      }

      if (check == false){
        for (unsigned int i = 0; i < table_size; i++){
          num_probs++;
          unsigned int new_index = (index + i*i) % table_size;
          if (hashtable[new_index] == key){
            hashtable[new_index] = Tombstone;
            num_of_keys--;
            break;
          }
          else if (hashtable[new_index] == 0){  
            num_probs = -1 * num_probs;
            break;
          }
        }
      }
  }

  return num_probs;

}

int FlatHash::search(const unsigned int key)
{ 
  unsigned int index = hashFunction(key);
  int num_probs = 0;

  if (flag == LINEAR_PROBING){
    for (unsigned int i = 0; i < table_size; i++){
      num_probs++;
      unsigned int new_index = (index + i) % table_size;
      if (hashtable[new_index] == key)
        break;
      else if (hashtable[new_index] == 0){
        num_probs = -1 * num_probs;
        break;
      }
    }
      
  }

  else if (flag == QUADRATIC_PROBING){
    bool check = false;
    for (unsigned int i = 0; i < table_size; i++){
      num_probs++;
      unsigned int new_index = (index + i*i) % table_size;
      if (hashtable[new_index] == key){
        check = true;
        break;
      }
      else if (hashtable[new_index] == 0){
        check = true;
        num_probs = -1 * num_probs;
        break;
      }
    }

    if (check == false){
      for (unsigned int i = 0; i < table_size; i++){
        num_probs++;
        unsigned int new_index = (index + i*i) % table_size;
        if (hashtable[new_index] == key)
          break;
        else if (hashtable[new_index] == 0){
          num_probs = -1 * num_probs;
          break;
        }
      }
    }

  }
  
  return num_probs;

}

void FlatHash::clearTombstones()
{
  if (flag == QUADRATIC_PROBING){
    int unsigned* temp_hashtable = new unsigned int[table_size];
    
    for (unsigned int i = 0; i < table_size; i++){
      temp_hashtable[i] = 0;
    }

    for (unsigned int i = 0; i < table_size; i++){
        if (hashtable[i] != 0 && hashtable[i] != Tombstone){
          bool check = false;
          unsigned int index = hashFunction(hashtable[i]);
  
          for (unsigned int j = 0; j < table_size; j++){
            unsigned int new_index = (index + j*j) % table_size;
            if (temp_hashtable[new_index] == 0){
              temp_hashtable[new_index] = hashtable[i];
              check = true;
              break;
            }   
          }

          if (check == false){
            
            for (unsigned int j = 0; j < table_size; j++){
              unsigned int new_index = (index + j) % table_size;
              if (temp_hashtable[new_index] == 0){
                temp_hashtable[new_index] = hashtable[i];
                break;
              }
            }
          }
        }
      }

    delete[] hashtable;
    hashtable = temp_hashtable;
  }
}

void FlatHash::print()
{
  // Print valid key pair - (index1:key1,index2:key2)
  // e.g., (1:3,3:7,5:1)
  std::cout << "(";
   
  if (num_of_keys != 0){
    unsigned int temp = 0;
    for (unsigned int i = 0; i < table_size; i++){
      if (hashtable[i] != 0 && hashtable[i] != Tombstone){
        temp++;
        std::cout << i << ':' <<hashtable[i];

        if (temp != num_of_keys)
          std::cout << ',';
        else
          break;
      }
    }
  }

  std::cout << ")" << std::endl;
}

#endif
