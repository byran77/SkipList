#pragma once

#include <cstdint>
#include <vector>

const uint8_t TOP_NUMBER = 10;

template<typename K, typename V>
class IrankList
{
public:
    virtual uint32_t insert(K& key, V& value) = 0;
    virtual uint32_t remove(K& key) = 0;
    virtual uint32_t update(K& key, V& new_value) = 0;

    //query the value of KEY
    virtual V value(K& key) const = 0;

    //query the rank of KEY
    virtual uint32_t find(K& key) const = 0;
    //query the key of DW_RANK
    virtual K rank(uint32_t dw_rank) const = 0;
    //query the keys ranked from DW_START to DW_END
    virtual std::vector<K> range(uint32_t dw_begin, uint32_t dw_end) const = 0;
    
    ////buffer the keys that rank top n
    virtual std::vector<K> top() const = 0;
};

template<typename K, typename V>
IrankList<K, V>* createRankList();

#include "myRankInterface.inl"