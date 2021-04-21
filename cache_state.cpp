#include "cache_state.h"
#include <iostream>

//========================================================================//
// CLASS SETSTATE
//========================================================================//
SetState::SetState(int num_ways) {
    // initialize way state array
    for (int i=0; i < num_ways; ++i)
        this->way_array.push_back(new WayState());
}

void SetState::updateState(int way, bool is_hit, int access_type) {
    // update way preuse distance
    this->way_array[way]->preuse;

    // update way last access type
    this->way_array[way]->access_type = {0,0,0,0,0};
    this->way_array[way]->access_type[access_type] = 1;

    // update way recency
    this->way_array[way]->recency;

    // update way num_hits
    this->way_array[way]->num_hits;
}

//========================================================================//
// CLASS CACHESTATE
//========================================================================//
CacheState::CacheState(int num_sets, int num_ways) {
    m_nsets = num_sets;
    m_nways = num_ways;
    
    // initialize set state array
    for (int i=0; i < m_nsets; ++i)
        this->set_array.push_back(new SetState(m_nways));
}

void CacheState::updateState(int set, int way, bool is_hit, int access_type) {
    this->set_array[set]->updateState(way, is_hit, access_type);
}