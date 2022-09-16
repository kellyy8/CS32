//
//  main.cpp
//  Map 2.0
//
//  Created by Kelly Yu on 1/24/22.
//

#include "Map.h"
#include <iostream>
//#include <cstdlib>

Map::Map(){
    numKeys = 0;
    dummy.prev = nullptr;
    dummy.next = nullptr;
}

Map::Map(const Map& other){
    if(other.size() == 0){
        numKeys = 0;
        dummy.prev = nullptr;
        dummy.next = nullptr;
        return;
    }

    //copy over the size of linkedlist
    numKeys = other.numKeys;

    //create a new list starting at the same points (starting at dummy node)
    Pair* p = &dummy;
    Pair* otherP = other.dummy.next;            //otherP wouldn't be nullptr for filled lists

    while(otherP != &other.dummy){
        Pair* temp = new Pair;
        temp->m_key = otherP->m_key;
        temp->m_value = otherP->m_value;
        temp->next = nullptr;
        temp->prev = p;

        p->next = temp;
        p=p->next;
        otherP = otherP->next;
    }

    //storing address of last item to dummy's previous pointer
    //completing the circularly linked list
    p->next = &dummy;
    dummy.prev = p;
}

Map& Map::operator=(const Map& other){
    if(this != &other){
        Map temp = other;         //construct a copy of other
        this->swap(temp);      //Map is replaced by copy of other; what temp becomes doesn't matter
    }
    return *this;
};

Map::~Map(){
    if(numKeys == 0)
        return;
    
    //need array to traverse through the linkedlist and delete every Key object
    Pair* p = dummy.next;           //p points to first Pair
    Pair* temp;
    while(p!=&dummy){
        temp = p;                   //hold onto the address of current Pair
        p = p->next;                //move pointer to the next Pair
        delete temp;                //delete the current Pair
    }
}

bool Map::empty() const{
    return numKeys == 0;
}

int Map::size() const{
    return numKeys;
}

bool Map::insert(const KeyType& key, const ValueType& value){
    if(contains(key))
        return false;
    
    //adding the first Pair
    if(size()==0){
        Pair* temp = new Pair;
        temp->m_key = key;
        temp->m_value = value;
        temp->prev = &dummy;     //only Pair is the first pair
        temp->next = &dummy;     //only Pair is the last pair; creates circularly linkedlist
        
        dummy.next = temp;          //dummy has a pointer to the first & last Pair
        dummy.prev = temp;
        numKeys++;
        return true;
    }
    
    Pair* p = dummy.next;        //dummy won't have nullptrs if there's at least 1 Pair

    //adding to the front/middle; find right space to insert key in linkedlist
    //traverse through linkedlist; compare key names of each Pair
    while(p != &dummy){
        if(p->m_key > key){
            Pair* temp = new Pair;          //allocate new Pair & link it into the list
            temp->m_key = key;
            temp->m_value = value;
            temp->next = p;                 //new Pair points to seceding Pair
            temp->prev = p->prev;           //new Pair points to preceding Pair
            
            p->prev->next = temp;           //preceding Pair points to new Pair (next)
            p->prev = temp;                 //seceding pair points to new Pair (prev)
            
            numKeys++;
            return true;
        }
        p=p->next;
    }
    
    //p now points to &dummy
    //if key is not added already, key should be added to the end
    Pair* temp = new Pair;          //allocate new Pair & link it into the list
    temp->m_key = key;
    temp->m_value = value;
    temp->next = p;                 //new Pair points to seceding Pair
    temp->prev = p->prev;           //new Pair points to preceding Pair
    
    p->prev->next = temp;           //preceding Pair points to new Pair (next)
    p->prev = temp;                 //seceding pair points to new Pair (prev)
    
    numKeys++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value){
    //traverse through list; adjust value when key is found & return true
    if(size() == 0)                   //since dummy.next == nullptr
        return false;
    
    Pair* p = dummy.next;
    while(p!=&dummy){
        if(p->m_key == key){
            p->m_value = value;
            return true;
        }
        p=p->next;
    }
    
    return false;           //key was not found
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    //if key is contained; insert == false, update == true
    //if key is not contained; insert == true, update == false
    return (insert(key, value) || update(key, value));
}

bool Map::erase(const KeyType& key){
    if(size() == 0)
        return false;
    
    //find the Pair with matching key
    Pair* p = dummy.next;
    while(p!=&dummy){
        if(p->m_key == key)
            break;
        p=p->next;
    }
    
    //if no Pair has matching key
    if(p == &dummy)
        return false;
    
    //doesn't matter if there is 1 or many Pairs
    //if it's 1 pair, dummy's pointers will point to itself
    //if Pair with matching key is found in the front, middle, or end of list, then:
    p->next->prev = p->prev;            //seceding Pair should point to Pair preceding curr Pair
    p->prev->next = p->next;            //preceding Pair should point to Pair seceding curr Pair
    delete p;
    numKeys--;
    return true;
}

bool Map::contains(const KeyType& key) const{
    //traverse through list; return true once Pair with matching key is found
    if(size() == 0)         //since dummy.next == nullptr
        return false;
    
    Pair* p = dummy.next;
    while(p!=&dummy){
        if(p->m_key == key){
            return true;
        }
        p=p->next;
    }
        
    return false;
}
 
bool Map::get(const KeyType& key, ValueType& value) const{
    if(numKeys == 0)            //since dummy.next == nullptr; empty list means no Pairs
        return false;
    
    //Get pointer to point to the Pair with matching key
    Pair* p = dummy.next;
    while(p!=&dummy){
        if(p->m_key == key){
            value = p->m_value;
            return true;
        }
        p=p->next;
    }

    //no Pairs found with matching key
    return false;
}
 
bool Map::get(int i, KeyType& key, ValueType& value) const{
    if(i<0 || i >= numKeys || numKeys == 0)     //since dummy.next == nullptr
        return false;

    //Get the pointer to point to the i-th Pair
    Pair* p = dummy.next;
    for(int k=0; k < i ; k++){
        p = p->next;
    }
    
    //Reassign key and value parameters with data from the i-th Pair
    key = p->m_key;
    value = p->m_value;
    return true;
}

void Map::swap(Map& other){
    //swap dummy Pair that mark the start and end of each list
    Pair temp2 = this->dummy;
    this->dummy = other.dummy;
    other.dummy = temp2;

    //make first and last Pairs of Other's linkedlist connected to This linkedlist's dummy
    //but set this's dummy's pointers to point to nullptrs if other's list was empty
    //avoids accessing nullptrs
    if(other.numKeys != 0){
        this->dummy.next->prev = &dummy;
        this->dummy.prev->next = &dummy;
    }
    else{
        this->dummy.next = nullptr;
        this->dummy.prev = nullptr;
    }

    //make first and last Pairs of This linkedlist's dummy connected to Other's dummy
    //only necessary of this's list is not empty
    //but set other's dummy's pointers to point to nullptrs if this's list was empty
    if(numKeys != 0){
        other.dummy.next->prev = &other.dummy;
        other.dummy.prev->next = &other.dummy;
    }
    else{
        other.dummy.next = nullptr;
        other.dummy.prev = nullptr;
    }
    
    //swap the sizes of the linkedlists
    int temp1 = this->numKeys;
    this->numKeys = other.numKeys;
    other.numKeys = temp1;

}

void Map::dump() const{
    if(dummy.next == nullptr)
        std::cerr << "empty list" << std::endl;
    Pair* p = dummy.next;
    for(int i=0; i<numKeys ; i++){
        std::cerr << "KEY " << p->m_key << " VAL " << p->m_value << std::endl;
        p=p->next;
    }
}

bool merge(const Map& m1, const Map& m2, Map& result){
    //if all three maps are referring to the same map, nothing needs to be done
    if(&m1 == &m2 && &m2 == &result)
        return true;
    
    //if m1 and m2 are referring to the same map, then result can be copy of either m1 or m2
    if(&m1 == &m2){
        result = m1;
        return true;
    }
    
    //if m1 and m2 are empty, result is empty
    if(m1.size() == 0 && m2.size() == 0){
        Map temp;
        temp.swap(result);
    }
    
    //if m2 is not empty, but m1 is; result == m2
    if(m1.size() == 0){
        result = m2;
        return true;
    }
    
    //if m1 is not empty, but m2 is; result == m1
    if(m2.size() == 0){
        result = m1;
        return true;
    }
    
    //if m1 and m2 are both filled, use temporary Map to collect valid Pairs to merge
    Map temp;
    KeyType m1k;
    ValueType m1v;
    KeyType m2k;
    ValueType m2v;
    bool sameKsameV = true;                 //all Pairs with same keys are full matches
    
    //searching for valid Pairs in m1 to merge
    for(int i=0; i<m1.size(); i++){
        m1.get(i, m1k, m1v);                //retrieve Pair from m1
        
        for(int k=0; k<m2.size(); k++){
            m2.get(k, m2k, m2v);            //retrieve Pair from m2
            
            //if we find m1's key in m2, m1's key is not unique to m1
            //decide to insert or not, then search for m1's next key in m2
            if(m1k == m2k){
                //same key same value; insert to result
                if(m1v == m2v){
                    temp.insert(m1k, m1v);
                    break;
                }
                //same key but different value; move on, no insert
                else{
                    sameKsameV = false;
                    break;
                }
            }
        }
        
        //won't insert if m1k == m2k when we break out of loop,
        //or if m2's last Pair has key that matches
        //only inserts if m1k is not found at all, when m2's last Pair's key still doesn't match
        if(m2.size()!=0 && m1k != m2k){
            temp.insert(m1k, m1v);
        }
    }
    
    //Searching for valid Pairs in m2 to merge
    for(int i=0; i<m2.size(); i++){
        m2.get(i, m2k, m2v);                //retrieve Pair from m1

        for(int k=0; k<m1.size(); k++){
            m1.get(k, m1k, m1v);            //retrieve Pair from m2

            //decision to insert or not insert any Pairs in m1 and m2 w/ same keys = alrdy handled
            if(m2k == m1k){
                break;
            }
        }

        //won't insert if m2k == m1k when we break out of loop,
        //or if m1's last Pair has key that matches
        //only inserts if m2k is not found at all, when m1's last Pair's key still doesn't match
        if(m1.size()!=0 && m2k != m1k){
            temp.insert(m2k, m2v);
        }
    }

    //replace result with the merged map
    result.swap(temp);
    return sameKsameV;
}

void reassign(const Map& m, Map& result){
    //if m is empty; result is empty OR if m only has one Pair, no reassignment can be made
    if(m.size() == 0 || m.size() == 1){
        result = m;
        return;
    }
    
    //store the current and next Pair's keys and values
    KeyType currK;
    ValueType currV;
    KeyType nextK;
    ValueType nextV;
    
    Map temp;

    //insert current Pair's key who will map to next Pair's value
    for(int i=0; i+1 < m.size(); i++){
        m.get(i, currK, currV);
        m.get(i+1, nextK, nextV);
        temp.insert(currK, nextV);
    }
    
    //temp's last Pair's key will map to m1's first Pair's value
    m.get(m.size()-1, currK, currV);
    m.get(0, nextK, nextV);
    temp.insert(currK, nextV);
    
    result.swap(temp);
}
