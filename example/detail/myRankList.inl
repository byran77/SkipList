#pragma once

#include "myRankList.h"

template<typename K, typename V>
std::unordered_map<K, V> myRankList<K, V>::ms_hash;

template<typename K, typename V>
myRankList<K, V>::myRankList()
{
    m_dw_length = 0;
    m_top = std::vector<K>(TOP_NUMBER);

    m_skiplist.setCmpFunc( [](const K& x, const K& y) -> bool {
        return (ms_hash[x] > ms_hash[y])
            || ((ms_hash[x] == ms_hash[y]) && (x > y));
    });
}

template<typename K, typename V>
myRankList<K, V>::~myRankList()
{

}

template<typename K, typename V>
uint32_t myRankList<K, V>::insert(K& key, V& value)
{   
    if (ms_hash.count(key)) {
        return 0;
    }

    ms_hash[key] = value;
    uint32_t dw_rank = m_skiplist.insert(key);
    m_dw_length = m_skiplist.length();

    if (dw_rank <= TOP_NUMBER) {
        topUpload();
    }

    return dw_rank;
}

template<typename K, typename V>
uint32_t myRankList<K, V>::remove(K& key)
{
    if (ms_hash.count(key)) {
        uint32_t dw_rank = m_skiplist.remove(key);
        m_dw_length = m_skiplist.length();

        ms_hash.erase(key);

        if (dw_rank <= TOP_NUMBER) {
            topUpload();
        }

        return dw_rank;
    }
    
    return 0;
}

template<typename K, typename V>
uint32_t myRankList<K, V>::update(K& key, V& new_value)
{
    if (ms_hash.count(key)) {
        uint32_t dw_rank = remove(key);
        
        dw_rank = insert(key, new_value);

        return dw_rank;
    }
    
    return 0;
}

template<typename K, typename V>
V myRankList<K, V>::value(K& key) const
{
    if (ms_hash.count(key)) {
        return ms_hash[key];
    }

    return V();
}

template<typename K, typename V>
uint32_t myRankList<K, V>::find(K& key) const
{
    if (ms_hash.count(key)) {
        std::pair<uint32_t,typename mySkipList<K>::const_iterator> result = m_skiplist.find(key);

        return result.first;
    }
    
    return 0;
}

template<typename K, typename V>
K myRankList<K, V>::rank(uint32_t dw_rank) const
{
    typename mySkipList<K>::const_iterator itr = m_skiplist.rank(dw_rank);

    return *itr;
}

template<typename K, typename V>
std::vector<K> myRankList<K, V>::range(uint32_t dw_begin, uint32_t dw_end) const
{
    typename mySkipList<K>::const_iterator itr = m_skiplist.rank(dw_begin);
    std::vector<K> result;

    for (int i = dw_begin; i <= dw_end && i <= m_dw_length; i++)
    {
        result.push_back(*itr);
        itr++;
    }

    return result;
}

template<typename K, typename V>
std::vector<K> myRankList<K, V>::top() const
{
    return m_top;
}

template<typename K, typename V>
void myRankList<K, V>::topUpload() 
{
    typename mySkipList<K>::const_iterator itr = m_skiplist.cbegin();
    for (int i = 0; i < TOP_NUMBER && i < m_dw_length; i++) {
        m_top[i] = *(itr++);
    }
    
    return ;
}