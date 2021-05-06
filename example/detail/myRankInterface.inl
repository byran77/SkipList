#include "myRankList.h"

template<typename K, typename V>
IrankList<K, V>* createRankList()
{
    return new myRankList<K, V>;
}