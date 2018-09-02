/*
 * extendible_hash.h : implementation of in-memory hash table using extendible
 * hashing
 *
 * Functionality: The buffer pool manager must maintain a page table to be able
 * to quickly map a PageId to its corresponding memory location; or alternately
 * report that the PageId does not match any currently-buffered page.
 */

#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <map>

// Below fix the error: ‘shared_ptr’ is not a member of ‘std’
#include <memory> 

#include "hash/hash_table.h"

namespace cmudb {

template <typename K, typename V>
class ExtendibleHash : public HashTable<K, V> {
public:
  // constructor
  ExtendibleHash(size_t size);
  // helper function to generate hash addressing
  size_t HashKey(const K &key);
  // helper function to get global & local depth
  int GetGlobalDepth() const;
  int GetLocalDepth(int bucket_id) const;
  int GetNumBuckets() const;
  // lookup and modifier
  bool Find(const K &key, V &value) override;
  bool Remove(const K &key) override;
  void Insert(const K &key, const V &value) override;

  class Bucket {
    public:
    Bucket(size_t size, int depth, int id) 
    : mCapacity(size), mLocalDepth(depth), mId(id)
    {
    }

    bool isFull() { return (dataMap.size() >= mCapacity); }

    size_t mCapacity; // fixed array size
    int mLocalDepth;
    int mId;
    // std::array must be a compile-time constant. We use vector instead
    //std::vector<std::pair<K,V>> list;
    std::map<K,V> dataMap;
  };

private:
  // add your own member variables here
  size_t GetBucketIndexFromHash(size_t hash);

  // total num of bits needed to express the total num of buckets
  int mDepth; // gloabl depth

  int mTotalBucketSize;  // should be 2^mDepth
  int mBucketCapacity;
  std::vector<std::shared_ptr<Bucket>> mDirectory;


};
} // namespace cmudb
