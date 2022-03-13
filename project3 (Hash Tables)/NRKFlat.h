#ifndef NRKFLAT_H
#define NRKFLAT_H

#include "FlatHash.h"
#include <iostream>
#include <stdint.h>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

class NRKFlat : public FlatHash
{
private:
  // Counter array
  unsigned int* counters;
  // Size of NRK filter (the number of counters)
  unsigned int filter_size;
  // Loading factor
  float alpha;
  
  unsigned int Tombstone;


public:
  NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size);

  ~NRKFlat();
  
  unsigned int hashFunction(const unsigned int key) { return key % filter_size; }
  
  // Hash function
  unsigned int murmurHash2(const void* key);

  void getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3);

  bool filter(const unsigned int key);

  void resize_counter();

  // Overwriting
  int insert(const unsigned int key);
  
  // Overwriting
  int remove(const unsigned int key);

  // Overwriting
  int search(const unsigned int key);

};

NRKFlat::NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size) : FlatHash(_flag, _alpha)
{
  filter_size = _filter_size;
  alpha = _alpha;
  Tombstone = 1000001;
  counters = new unsigned int[filter_size];
  for (unsigned int i = 0; i < filter_size; i++)
    counters[i] = 0;
}

NRKFlat::~NRKFlat()
{
  if (counters != NULL){
    delete[] counters;
    counters = NULL;
  }

}

unsigned int NRKFlat::murmurHash2(const void* key){
  int len = 4;
  unsigned int seed = 2019;

  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  unsigned int h = seed ^ len;

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4)
  {
    unsigned int k = *(unsigned int *)data;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h *= m; 
    h ^= k;

    data += 4;
    len -= 4;
  }

  switch(len)
  {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
          h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

void NRKFlat::getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3)
{ 
  h1 = murmurHash2(INT2VOIDP(&key));
  h2 = murmurHash2(INT2VOIDP(&h1));
  h3 = murmurHash2(INT2VOIDP(&h2));
  
}

bool NRKFlat::filter(const unsigned int key)
{
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);

  unsigned int arr[3] = { h1, h2, h3 };

  for (int i = 0; i < 3; i++){
    if (counters[hashFunction(arr[i])] == 0)
      return false;    
  }  

  return true;
}

void NRKFlat::resize_counter(){
    
  unsigned int* hashtable = getTablePointer();
  unsigned int num_keys = getNumofKeys();
  unsigned int temp = 0;
  filter_size = 2 * filter_size;
  unsigned int* temp_counters = new unsigned int[filter_size];
  
  for (unsigned int k = 0; k < filter_size; k++)
    temp_counters[k] = 0;

  for (unsigned int i = 0; i < getTableSize(); i++){
    
    if (hashtable[i] != 0 && hashtable[i] != Tombstone){
    temp++;
    unsigned int key = hashtable[i];
    unsigned int h1, h2, h3;
    getMMHashValue(key, h1, h2, h3);

    unsigned int arr[3] = { h1, h2, h3 };

    for (int i = 0; i < 3; i++)
      temp_counters[hashFunction(arr[i])]++;

    if (temp == num_keys)
      break;
    }

  }

  delete[] counters;
  counters = temp_counters;
}

int NRKFlat::insert(const unsigned int key)
{  
  int time_cost = insert_part(key);

  if (time_cost > 0){

  unsigned int h1, h2, h3;

  getMMHashValue(key, h1, h2, h3);

  unsigned int arr[3] = { h1, h2, h3 };

  for (int i = 0; i < 3; i++)
    counters[hashFunction(arr[i])]++; 
  }

  float temp_alpha = float(getNumofKeys()) / getTableSize();
  if (temp_alpha >= alpha){
    resize_counter();
    resize();
  }

  return time_cost;

}

int NRKFlat::remove(const unsigned int key)
{ 
  if (filter(key) == false)
    return 0;
  
  int time_cost = FlatHash::remove(key);
  if (time_cost > 0){
  
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);
  unsigned int arr[3] = { h1, h2, h3 };

  
  for (int i = 0; i < 3; i++)
      counters[hashFunction(arr[i])]--;
  }

  return time_cost;
}

int NRKFlat::search(const unsigned int key)
{
  if (filter(key) == false)
    return 0;
  
  int time_cost = FlatHash::search(key);

  return time_cost;
}

#endif
