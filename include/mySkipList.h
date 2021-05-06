#pragma once

#include <cstdint>
#include <utility>

//When the opration is "<", the list is sorted in non-decreasing order
//and vice versa.
#define COMPARISON_OPR    <

const static uint8_t MYSKIPLIST_MAXLEVEL = 32;

template <typename T>
class mySkipList
{
public:
    using value_type            = T;
    using pointer               = T*;
    using const_pointer         = const T*;
    using reference             = T&;
    using const_reference       = const T&;

    using cmp_func = bool (*)(const_reference, const_reference);

    mySkipList();
    ~mySkipList();

    uint32_t length() const;
    void clear();

    //Using special function to campare elements
    void setCmpFunc(cmp_func f);

    uint32_t insert(const_reference data);
    uint32_t remove(const_reference data);

    class const_iterator;

    //Rank starts from 1
    std::pair<uint32_t, const_iterator> find(const_reference data) const;
    const_iterator rank(uint32_t dw_rank) const;

    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    uint32_t getRandomLevel() const;

private:
    struct listNode;
    using node_ptr = listNode*;

    struct nodeLevel {
        node_ptr p_next = nullptr;
        uint32_t dw_span = 1;
    };

    struct listNode {
        value_type elem_data;
        uint32_t n_level;
        nodeLevel* arr_level;
        node_ptr p_prev;
        
        listNode(const_reference data, uint32_t level = 1) {
            elem_data = data;
            n_level = level; 
            if (n_level == 1) {
                arr_level = new nodeLevel;
            }
            else {
                arr_level = new nodeLevel[n_level];
            }
            p_prev = nullptr;
        }
        listNode(uint32_t level = 1) {
            n_level = level;
             if (n_level == 1) {
                arr_level = new nodeLevel;
            }
            else {
                arr_level = new nodeLevel[n_level];
            }
            p_prev = nullptr;
        }
        ~listNode() {
            if (n_level == 1) {
                delete arr_level;
            }
            else {
                delete[] arr_level;
            }
        }
    };

public:
    //Segmentation will fault when access overflows.
    class const_iterator {
    public:
        //If m_p_skiplist is not nullptr but m_p_listnode is, it points to m_p_skiplist->cend().
        //When m_p_skiplist is nullptr, it points to nill.
        const_iterator(const mySkipList* p_skiplist = nullptr, const listNode* p_listnode = nullptr) {
            m_p_skiplist = p_skiplist;
            m_p_listnode = nullptr;

            if (m_p_skiplist) {
                m_p_listnode = p_listnode;
            }
        }

        const_iterator& operator++ () {
            if (m_p_listnode && m_p_listnode->arr_level[0].p_next) {
                m_p_listnode = m_p_listnode->arr_level[0].p_next;
            }
            else {
                m_p_listnode = nullptr;
            }
            return *this;
        }

        const_iterator operator++ (int) {
            const_iterator tmp = *this;
            if (m_p_listnode && m_p_listnode->arr_level[0].p_next) {
                m_p_listnode = m_p_listnode->arr_level[0].p_next;
            }
            else {
                m_p_listnode = nullptr;
            }
            return tmp;
        }

        const_iterator& operator-- () {
            if (m_p_listnode && m_p_listnode->p_prev) {
                m_p_listnode = m_p_listnode->p_prev;
            }
            else if (m_p_skiplist && m_p_listnode == nullptr) {
                m_p_listnode = m_p_skiplist->m_p_tail;
            }
            else {
                m_p_listnode = nullptr;
            }
            return *this;
        }

        const_iterator operator-- (int) {
            const_iterator tmp = *this;
            if (m_p_listnode && m_p_listnode->p_prev) {
                m_p_listnode = m_p_listnode->p_prev;
            }
            else if (m_p_skiplist && m_p_listnode == nullptr) {
                m_p_listnode = m_p_skiplist->m_p_tail;
            }
            else {
                m_p_listnode = nullptr;
            }
            return tmp;
        }

        //only for read
        const_reference operator* () const {
            return m_p_listnode->elem_data;
        }

        const_pointer operator-> () const {
            return &( operator*() );
        }

        bool operator== (const const_iterator& o_itr) const {
            if (m_p_skiplist == o_itr.m_p_skiplist && m_p_listnode == o_itr.m_p_listnode) {
                return true;
            } 

            return false;
        }

        bool operator!= (const const_iterator& o_itr) const {
            return !(*this == o_itr);
        }

    private:
        const mySkipList* m_p_skiplist;
        const listNode* m_p_listnode;
    };

private:
    node_ptr m_p_head;
    node_ptr m_p_tail;

    uint32_t m_dw_current_level;
    uint32_t m_dw_current_length;

    cmp_func m_cmp_func;
};


#include "mySkipList.inl"