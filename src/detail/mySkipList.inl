#pragma once

#include "mySkipList.h"
#include <random>

template <class T>
mySkipList<T>::mySkipList()
{
    m_dw_current_level = 1;
    m_dw_current_length = 0;
    m_p_head = new listNode(m_dw_current_level);
    m_p_tail = m_p_head;

    setCmpFunc( [](const_reference x, const_reference y) -> bool {
        return x COMPARISON_OPR y;
    });
}

template <class T>
mySkipList<T>::~mySkipList()
{
    clear();
    delete m_p_head;
}

template <class T>
void mySkipList<T>::setCmpFunc(cmp_func f)
{
    m_cmp_func = f;
}

template <class T>
uint32_t mySkipList<T>::length() const
{
    return m_dw_current_length;
}

template <class T>
void mySkipList<T>::clear()
{
    node_ptr tmp1 = m_p_head, tmp2;

    while (tmp1 != nullptr) {
        tmp2 = tmp1->arr_level[0].p_next;
        delete tmp1;
        tmp1 = tmp2;
    }

    m_dw_current_length = 0;
    m_dw_current_level = 1;
    m_p_head = new listNode(m_dw_current_level);
    m_p_tail = m_p_head;

    return;
}

template <class T>
uint32_t mySkipList<T>::insert(const_reference data)
{
    uint32_t dw_random_level = getRandomLevel();
    node_ptr p_insert_node = new listNode(data, dw_random_level);

    if (dw_random_level > m_dw_current_level) {
        node_ptr tmp = m_p_head;
        m_p_head = new listNode(dw_random_level);

        for (int i = 0; i < m_dw_current_level; i++) {
            m_p_head->arr_level[i].p_next = tmp->arr_level[i].p_next;
            m_p_head->arr_level[i].dw_span = tmp->arr_level[i].dw_span;
        }

        for (int i = m_dw_current_level; i < dw_random_level; i++) {
            m_p_head->arr_level[i].dw_span = m_dw_current_length + 1;
        }

        m_dw_current_level = dw_random_level;
        delete tmp;
    }

    node_ptr* arr_update = new node_ptr[m_dw_current_level];
    uint32_t* arr_rank = new uint32_t[m_dw_current_level];
    
    node_ptr tmp = m_p_head;
    for (int i = m_dw_current_level - 1; i >= 0; i--) {
        arr_rank[i] = (i == m_dw_current_level - 1)? 0: arr_rank[i + 1];
        while (tmp->arr_level[i].p_next && m_cmp_func(tmp->arr_level[i].p_next->elem_data, data)) {
            arr_rank[i] += tmp->arr_level[i].dw_span;
            tmp = tmp->arr_level[i].p_next;
        }
        arr_update[i] = tmp;
    }

    for (int i = 0; i < dw_random_level; i++) {
        p_insert_node->arr_level[i].p_next = arr_update[i]->arr_level[i].p_next;
        arr_update[i]->arr_level[i].p_next = p_insert_node;

        p_insert_node->arr_level[i].dw_span = arr_update[i]->arr_level[i].dw_span - (arr_rank[0] - arr_rank[i]);
        arr_update[i]->arr_level[i].dw_span = (arr_rank[0] - arr_rank[i]) + 1;
    }

    for (int i = dw_random_level; i < m_dw_current_level; i++) {
        arr_update[i]->arr_level[i].dw_span++;
    }

    if (p_insert_node->arr_level[0].p_next) {
        p_insert_node->arr_level[0].p_next->p_prev = p_insert_node;
    }
    else {
        m_p_tail = p_insert_node;
    }
    p_insert_node->p_prev = arr_update[0];

    m_dw_current_length++;

    uint32_t dw_rank = arr_rank[0] + 1;

    delete[] arr_update;
    delete[] arr_rank;

    return dw_rank;
}

template <class T>
uint32_t mySkipList<T>::remove(const_reference data)
{
    node_ptr* arr_update = new node_ptr[m_dw_current_level];
    uint32_t dw_rank = 0;

    node_ptr tmp = m_p_head;
    for (int i = m_dw_current_level - 1; i >= 0; i--) {
        while (tmp->arr_level[i].p_next && m_cmp_func(tmp->arr_level[i].p_next->elem_data, data)) {
            dw_rank += tmp->arr_level[i].dw_span;
            tmp = tmp->arr_level[i].p_next;
        }
        arr_update[i] = tmp;
    }
    
    dw_rank += 1;
    tmp = tmp->arr_level[0].p_next;
    if (tmp && tmp->elem_data == data) {
        for (int i = 0; i < m_dw_current_level; i++) {
            if (arr_update[i]->arr_level[i].p_next == tmp) {
                arr_update[i]->arr_level[i].p_next = tmp->arr_level[i].p_next;
                arr_update[i]->arr_level[i].dw_span += tmp->arr_level[i].dw_span - 1;
            }
            else {
                arr_update[i]->arr_level[i].dw_span--;
            }
        }

        if (tmp->arr_level[0].p_next) {
            tmp->arr_level[0].p_next->p_prev = tmp->p_prev;
        }
        else {
            m_p_tail = tmp->p_prev;
        }
        
        delete tmp;

        m_dw_current_length--;
        bool b_flag = false;
        while (m_dw_current_level > 1 && m_p_head->arr_level[m_dw_current_level - 1].p_next == nullptr) {
            m_dw_current_level--;
            b_flag = true;
        }
        if (b_flag) {
            tmp = m_p_head;
            m_p_head = new listNode(m_dw_current_level);
            for (int i = 0; i < m_dw_current_level; i++) {
                m_p_head->arr_level[i].p_next = tmp->arr_level[i].p_next;
                m_p_head->arr_level[i].dw_span = tmp->arr_level[i].dw_span;
            }

            delete tmp;
        }

        delete[] arr_update;

        return dw_rank;
    }

    delete[] arr_update;

    return 0;
}

template <class T>
std::pair<uint32_t, typename mySkipList<T>::const_iterator> mySkipList<T>::find(const_reference data) const
{
    uint32_t dw_rank = 0;

    node_ptr tmp = m_p_head;
    for (int i = m_dw_current_level - 1; i >= 0; i--) {
        while (tmp->arr_level[i].p_next && m_cmp_func(tmp->arr_level[i].p_next->elem_data, data)) {
            dw_rank += tmp->arr_level[i].dw_span;
            tmp = tmp->arr_level[i].p_next;
        }

        if (tmp->arr_level[i].p_next && tmp->arr_level[i].p_next->elem_data == data) {
            dw_rank += tmp->arr_level[i].dw_span;
            tmp = tmp->arr_level[i].p_next;
            
            const_iterator o_itr(this, tmp);

            return std::pair<uint32_t, const_iterator>(dw_rank, o_itr);
        }
    }

    return std::pair<uint32_t, const_iterator>(0, const_iterator(nullptr, nullptr));
}

template <class T>
typename mySkipList<T>::const_iterator mySkipList<T>::rank(uint32_t dw_rank) const
{
    if (dw_rank > m_dw_current_length) {
        return const_iterator(nullptr, nullptr);
    }  

    uint32_t dw_cur_rank = 0, dw_next_rank = 0;
    uint32_t dw_cur_level = m_dw_current_level - 1;

    node_ptr tmp = m_p_head;
    while (dw_cur_level >= 0) {
        dw_next_rank = dw_cur_rank + tmp->arr_level[dw_cur_level].dw_span;
        if (dw_next_rank > dw_rank) {
            dw_cur_level--;
        }
        else if (dw_next_rank < dw_rank) {
            dw_cur_rank = dw_next_rank;
            tmp = tmp->arr_level[dw_cur_level].p_next;
        }
        else {
            break;
        }
    }
    tmp = tmp->arr_level[dw_cur_level].p_next;

    return const_iterator(this, tmp);
}

template <class T>
typename mySkipList<T>::const_iterator mySkipList<T>::cbegin() const
{
    return const_iterator(this, m_p_head->arr_level[0].p_next);
}

template <class T>
typename mySkipList<T>::const_iterator mySkipList<T>::cend() const
{
    return const_iterator(this, nullptr);
}

template <class T>
uint32_t mySkipList<T>::getRandomLevel() const
{
    uint32_t dw_level = 1;

    std::random_device generator;
    std::uniform_int_distribution<> distribution(0, RAND_MAX);

    while ((distribution(generator) & 0xFFFF) < (0xFFFF >> 2)) {
        dw_level++;
    }

    return (dw_level < MYSKIPLIST_MAXLEVEL)? dw_level: MYSKIPLIST_MAXLEVEL;
}