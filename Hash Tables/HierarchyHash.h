#ifndef HIERARCHYHASH_H
#define HIERARCHYHASH_H

#include <iostream>
#include "FlatHash.h"

class HierarchyHash
{
private:
  unsigned int** hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;  
  // Size(range) of full hash table. Initially 1000
  unsigned int table_size;
  // Size of subhash table. Fixed by 100
  unsigned int sub_table_size;
  // Nums of keys
  unsigned int num_of_keys;
  // Tombstone
  unsigned int Tombstone;
  // Number of sub tables
  unsigned int num_sub_tables;
  // Number of allocated sub tables
  unsigned int num_alloc_sub_tables;


public:
  HierarchyHash(enum overflow_handle _flag, float _alpha);

  ~HierarchyHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  // Return the size of allocated sub hash table
  unsigned int getAllocatedSize() { return sub_table_size * num_alloc_sub_tables; }

  // Return time cost
  int insert(const unsigned int key);

  // Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  // Resize
  void resize();

  // Choose a sub hashtable
  unsigned int choose(unsigned int index, unsigned int** hashtable);

  // Check whether a sub hashtable is empty or not
  bool isEmpty(unsigned int* sub_hashtable);

  // Get number of keys in a subhashtable
  unsigned int key_num_subhash(unsigned int* subhashtable);
  
  // Shift
  void shift(unsigned int index, unsigned int key);

  // Print
  void print();
};

HierarchyHash::HierarchyHash(enum overflow_handle _flag, float _alpha)
{
  // Initial map size is 1000
  table_size = 1000;
  // Table size is fixed to 100
  sub_table_size = 100;
  flag = _flag;
  alpha = _alpha;
  num_sub_tables = 10;

  num_of_keys = 0;
  Tombstone = 1000001;
  num_alloc_sub_tables = 0;
  hashtable = new unsigned int*[num_sub_tables];
  for (unsigned int i = 0; i < num_sub_tables; i++)
    hashtable[i] = NULL;

}

HierarchyHash::~HierarchyHash()
{
  if (hashtable != NULL){
    for (unsigned int i = 0; i < num_sub_tables; i++){
      if (hashtable[i] != NULL)
        delete[] hashtable[i]; 
    }
    hashtable = NULL;
  }
}


unsigned int HierarchyHash::choose(unsigned int index, unsigned int** hashtable){
  unsigned int i = 0;
  while (i < num_sub_tables){
    if (100 * i <= index && index < 100 * (i+1)){
      if (hashtable[i] == NULL){
        hashtable[i] = new unsigned int[sub_table_size];
        num_alloc_sub_tables++;
        for (unsigned int k = 0; k < sub_table_size; k++)
          hashtable[i][k] = 0;
      }    
    
      break;
    }
    i++;
  }

  return i;
}

int HierarchyHash::insert(const unsigned int key)
{
  unsigned int index = hashFunction(key);
  int num_probs = 0;

  if (flag == LINEAR_PROBING){

    for (unsigned int j = 0; j < table_size; j++){
      unsigned int new_index = (index + j) % table_size;
      num_probs++;
      
      unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
      new_index = new_index % sub_table_size;

      if (sub_hashtable[new_index] == 0){
        sub_hashtable[new_index] = key;
        num_of_keys++;
        break;
      }
      else if (sub_hashtable[new_index] == key){
        num_probs = -1 * num_probs;
        break;
      }
    }
  }


  else if (flag == QUADRATIC_PROBING){
    bool check = false;
    for (unsigned int j = 0; j < table_size; j++){
      unsigned int new_index = (index + j*j) % table_size;
      num_probs++;
      
      unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
      new_index = new_index % sub_table_size;

      if (sub_hashtable[new_index] == 0 || sub_hashtable[new_index] == Tombstone){
        sub_hashtable[new_index] = key;
        num_of_keys++;
        check = true;
        break;
      }
      else if (sub_hashtable[new_index] == key){
        check = true;
        num_probs = -1 * num_probs;
        break;
      }
      
    }

    if (check == false){
      for (unsigned int j = 0; j < table_size; j++){
        unsigned int new_index = (index + j) % table_size;
        num_probs++;
        
        unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
        new_index = new_index % sub_table_size;

        if (sub_hashtable[new_index] == 0 || sub_hashtable[new_index] == Tombstone){
          sub_hashtable[new_index] = key;
          num_of_keys ++;
          break;
        }
        else if (sub_hashtable[new_index] == key){
          num_probs = -1 * num_probs;
          break;
        }
      }
    }
  }


  float temp_alpha = float(num_of_keys) / table_size;
  if (temp_alpha >= alpha)
    resize();
  
  return num_probs;
}

void HierarchyHash::resize(){
  
  unsigned int initial_num_sub_tables = num_sub_tables;
  table_size = 2 * table_size;
  num_sub_tables = 2 * num_sub_tables;
  num_alloc_sub_tables = 0;
  unsigned int** temp_hashtable = new unsigned int*[num_sub_tables];

  for (unsigned int i = 0; i < num_sub_tables; i++)
    temp_hashtable[i] = NULL;

  for (unsigned int i = 0; i < initial_num_sub_tables; i++){
    if (hashtable[i] != NULL){
      unsigned int* sub_hashtable = hashtable[i];

      if (flag == LINEAR_PROBING){

        for (unsigned int j = 0; j < sub_table_size; j++){
          if (sub_hashtable[j] != 0){
        
            unsigned int index = hashFunction(sub_hashtable[j]);
          
            for (unsigned int k = 0; k < table_size; k++){
              unsigned int new_index = (index + k) % table_size;

              unsigned int* temp_sub_hashtable = temp_hashtable[choose(new_index, temp_hashtable)];
              new_index = new_index % sub_table_size;
           
              if (temp_sub_hashtable[new_index] == 0){
                temp_sub_hashtable[new_index] = sub_hashtable[j];
                break;
              }
            }
          }
        }
      }
    
    else if (flag == QUADRATIC_PROBING){
      for (unsigned int j = 0; j < sub_table_size; j++){
        if (sub_hashtable[j] != 0 && sub_hashtable[j] != Tombstone){
          bool check = false;
          unsigned int index = hashFunction(sub_hashtable[j]);

          for (unsigned int k = 0; k < table_size; k++){
            unsigned int new_index = (index + k*k) % table_size;

            unsigned int* temp_sub_hashtable = temp_hashtable[choose(new_index, temp_hashtable)];
            new_index = new_index % sub_table_size;

            if (temp_sub_hashtable[new_index] == 0){
              temp_sub_hashtable[new_index] = sub_hashtable[j];
              check = true;
              break;
            }   
          }

          if (check == false){

            for (unsigned int k = 0; k < table_size; k++){

              unsigned int new_index = (index + k) % table_size;
              unsigned int* temp_sub_hashtable = temp_hashtable[choose(new_index,temp_hashtable)];
              new_index = new_index % sub_table_size;
              
              if (temp_sub_hashtable[new_index] == 0){
                temp_sub_hashtable[new_index] = sub_hashtable[j];
                break;
              }
            }
          }
       }
      }
    }
    delete[] hashtable[i];
  }
  }

  hashtable = temp_hashtable;

}

bool HierarchyHash::isEmpty(unsigned int* sub_hashtable){
  for (unsigned int i = 0; i < sub_table_size; i++){
    if (sub_hashtable[i] != 0 && sub_hashtable[i] != Tombstone)
      return false;
  }

  return true;
}

void HierarchyHash::shift(unsigned int index, unsigned int key){

    unsigned int next = (index + 1) % table_size;

    while (true){
    
      for (unsigned int i = 0 ; i < table_size; i++){

        unsigned int temp_index = (next + i) % table_size;
        unsigned int* temp_sub_hashtable = hashtable[choose(temp_index, hashtable)];

        unsigned int sub_index = temp_index % sub_table_size; 
        if (temp_sub_hashtable[sub_index] == 0)
         return;

        unsigned int temp_key = temp_sub_hashtable[sub_index];
        unsigned int location = hashFunction(temp_key);

        for (unsigned int j = 0; j < table_size; j++){
          unsigned int new_index = (location + j) % table_size;

          unsigned int* sub_hash = hashtable[choose(new_index, hashtable)];
          new_index = new_index % sub_table_size;

          if (sub_hash[new_index] == 0){
            sub_hash[new_index] = temp_key;
            break;
          }
          else if (sub_hash[new_index] == temp_key)
            break;
        }
      
       unsigned int* sub_hashtable = hashtable[choose(index, hashtable)]; 
       if (sub_hashtable[index % sub_table_size] != 0){
          temp_sub_hashtable[sub_index] = 0;
          index = temp_index;
          next = (temp_index + 1) % table_size;
          break;
        }
      }
    }
}

int HierarchyHash::remove(const unsigned int key)
{
  unsigned int index = hashFunction(key);
  int num_probs = 0;

  if (flag == LINEAR_PROBING){
    for (unsigned int j = 0; j < table_size; j++){
      unsigned int temp_index = (index + j) % table_size;
      num_probs++;
      
      unsigned int* sub_hashtable = hashtable[choose(temp_index, hashtable)];
      unsigned int new_index = temp_index % sub_table_size;

      if (sub_hashtable[new_index] == key){
        sub_hashtable[new_index] = 0;
        shift(temp_index, key);
        num_of_keys--;
        break;
      }
      else if (sub_hashtable[new_index] == 0){
        num_probs = -1 * num_probs;
        break;
      }
    }
  }


  else if (flag == QUADRATIC_PROBING){
    bool check = false;
    for (unsigned int j = 0; j < table_size; j++){
      unsigned int new_index = (index + j*j) % table_size;
      num_probs++;
      
      unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
      new_index = new_index % sub_table_size;

      if (sub_hashtable[new_index] == key){
        sub_hashtable[new_index] = Tombstone;
        num_of_keys--;
        check = true;
        break;
      }
      else if (sub_hashtable[new_index] == 0){
        check = true;
        num_probs = -1 * num_probs;
        break;
      }
    }

    if (check == false){
      for (unsigned int j = 0; j < table_size; j++){
        unsigned int new_index = (index + j) % table_size;
        num_probs++;
        
        unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
        new_index = new_index % sub_table_size;

        if (sub_hashtable[new_index] == key){
          sub_hashtable[new_index] = Tombstone;
          num_of_keys--;
          break;
        }
        else if (sub_hashtable[new_index] == 0){
          num_probs = -1 * num_probs;
          break;
        }
      }
    }
  }

  for (unsigned int i = 0; i < num_sub_tables; i++){
    if (hashtable[i] != NULL && isEmpty(hashtable[i])){
      delete[] hashtable[i];
      hashtable[i] = NULL;
      num_alloc_sub_tables--;
    }
  }

  return num_probs;
}

int HierarchyHash::search(const unsigned int key)
{
  int index = hashFunction(key);
  int num_probs = 0;

  if (flag == LINEAR_PROBING){

    for (unsigned int j = 0; j < table_size; j++){
      unsigned int new_index = (index + j) % table_size;
      num_probs++;
      
      unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
      new_index = new_index % sub_table_size;

      if (sub_hashtable[new_index] == key)
        break;
      else if (sub_hashtable[new_index] == 0){
        num_probs = -1 * num_probs;
        break;
      }
    }
  }


  else if (flag == QUADRATIC_PROBING){
    bool check = false;
    for (unsigned int j = 0; j < table_size; j++){
      unsigned int new_index = (index + j*j) % table_size;
      num_probs++;
      
      unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
      new_index = new_index % sub_table_size;

      if (sub_hashtable[new_index] == key){
        check = true;
        break;
      }
      else if (sub_hashtable[new_index] == 0){
        check = true;
        num_probs = -1 * num_probs;
        break;
      }
      
    }

    if (check == false){
      for (unsigned int j = 0; j < table_size; j++){
        unsigned int new_index = (index + j) % table_size;
        num_probs++;
        
        unsigned int* sub_hashtable = hashtable[choose(new_index, hashtable)];
        new_index = new_index % sub_table_size;

        if (sub_hashtable[new_index] == key){
          break;
        }
        else if (sub_hashtable[new_index] == 0){
          num_probs = -1 * num_probs;
          break;
        }
      }
    }
  }

  return num_probs;

}

void HierarchyHash::clearTombstones()
{
  if (flag == QUADRATIC_PROBING){

    num_alloc_sub_tables = 0;
    unsigned int** temp_hashtable = new unsigned int*[num_sub_tables];

    for (unsigned int i = 0; i < num_sub_tables; i++)
      temp_hashtable[i] = NULL;

    for (unsigned int i = 0; i < num_sub_tables; i++){
      if (hashtable[i] != NULL){
        unsigned int* sub_hashtable = hashtable[i];


        for (unsigned int j = 0; j < sub_table_size; j++){
          if (sub_hashtable[j] != 0 && sub_hashtable[j] != Tombstone){
            bool check = false;
            unsigned int index = hashFunction(sub_hashtable[j]);

            for (unsigned int k = 0; k < table_size; k++){
              unsigned int new_index = (index + k*k) % table_size;

              unsigned int* temp_sub_hashtable = temp_hashtable[choose(new_index, temp_hashtable)];
              new_index = new_index % sub_table_size;

              if (temp_sub_hashtable[new_index] == 0){
                temp_sub_hashtable[new_index] = sub_hashtable[j];
                check = true;
                break;
              }   
            }

            if (check == false){

              for (unsigned int k = 0; k < table_size; k++){

                unsigned int new_index = (index + k) % table_size;
                unsigned int* temp_sub_hashtable = temp_hashtable[choose(new_index,temp_hashtable)];
                new_index = new_index % sub_table_size;
              
                if (temp_sub_hashtable[new_index] == 0){
                  temp_sub_hashtable[new_index] = sub_hashtable[j];
                  break;
                }
              }
            }
          }
        }
      
      delete[] sub_hashtable;
      }
    }
  hashtable = temp_hashtable;
  }
}

unsigned int HierarchyHash::key_num_subhash(unsigned int* subhashtable){
  unsigned int key_num = 0;
  for (unsigned int i = 0; i < sub_table_size; i++){
    if (subhashtable[i] != 0 && subhashtable[i] != Tombstone)
      key_num++;
  }

  return key_num;
}

void HierarchyHash::print() 
{
  // Print valid key pair for each sub hash table - subtable_id:(index1:key1,index2:key2)
  // e.g., 0:(1:3,3:7,5:1)
  //       1:(101:2,192:10)
  //       9:(902:90,938:82)

  if(getNumofKeys() == 0){
    std::cout << "()" << std::endl;
    return;
  }

  for (unsigned int i = 0; i < num_sub_tables; i++){
    if (hashtable[i] == NULL)
      continue;
  
    unsigned int temp = 0;
    unsigned int key_num = key_num_subhash(hashtable[i]);
    std::cout << i << ":(";
    for (unsigned int j = 0; j < sub_table_size; j++){
      if (hashtable[i][j] != 0 && hashtable[i][j] != Tombstone){
        temp++;
        std::cout << i * 100 + j << ':' << hashtable[i][j];
       
        if (temp != key_num)
          std::cout << ',';
        else
          break;
      }

    }
    std::cout << ')' << std::endl;
  }
}

#endif
