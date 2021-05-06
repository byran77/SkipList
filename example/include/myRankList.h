#pragma once

#include "myRankInterface.h"
#include "mySkipList.h"
#include <unordered_map>

template<typename K, typename V>
class myRankList : public IrankList<K, V>
{
public:
    myRankList();
    ~myRankList();

    virtual uint32_t insert(K& key, V& value) override;
    virtual uint32_t remove(K& key) override;
    virtual uint32_t update(K& key, V& new_value) override;

    //query the value of KEY
    virtual V value(K& key) const override;

    //query the rank of KEY
    virtual uint32_t find(K& key) const override;
    //query the key of DW_RANK
    virtual K rank(uint32_t dw_rank) const override;
    //query the keys ranked from DW_START to DW_END
    virtual std::vector<K> range(uint32_t dw_begin, uint32_t dw_end) const override;
    
    virtual std::vector<K> top() const override;

private:
    void topUpload();

private:
    uint32_t m_dw_length;
    mySkipList<K> m_skiplist;

    //buffer the keys that rank top n
    std::vector<K> m_top;

    static std::unordered_map<K, V> ms_hash;
};

#include "myRankList.inl"