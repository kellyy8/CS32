//
//  Map.h
//  Map 2.0
//
//  Created by Kelly Yu on 1/24/22.
//

#ifndef Map_h
#define Map_h

#include <iostream>
using KeyType = std::string;
using ValueType = double;

class Map{
    public:
        Map();
        Map(const Map& other);
        Map& operator=(const Map& other);
        ~Map();
        
        bool empty() const;
        int size() const;
        bool insert(const KeyType& key, const ValueType& value);
        bool update(const KeyType& key, const ValueType& value);
        bool insertOrUpdate(const KeyType& key, const ValueType& value);
        bool erase(const KeyType& key);
        bool contains(const KeyType& key) const;
        bool get(const KeyType& key, ValueType& value) const;
        bool get(int i, KeyType& key, ValueType& value) const;
        void swap(Map& other);
        void dump() const;
    
    private:
        struct Pair{
            KeyType m_key;
            ValueType m_value;
            Pair* prev;
            Pair* next;
        };
      
      Pair dummy;
      int numKeys;

};

bool merge(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

#endif /* Map_h */
