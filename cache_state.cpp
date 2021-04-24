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

//========================================================================//
void SetState::updateState(int way, bool is_hit, int access_type, vector<int> recency_list) {
    // update way preuse distance
    for (unsigned i=0; i < this->way_array.size(); ++i)
        this->way_array[way]->preuse++;

    // update way last access type
    this->way_array[way]->access_type = {0,0,0,0,0};
    this->way_array[way]->access_type[access_type] = 1;

    // update way recency
    for (unsigned i=0; i < this->way_array.size(); ++i)
        this->way_array[i]->recency = recency_list[i];

    // update way num_hits
    if (is_hit)
        this->way_array[way]->num_hits++;
}

//========================================================================//
void SetState::resetState(int way, int access_type) {
    // reset num hits since insertion
    this->way_array[way]->num_hits = 0;
    
    // reset preuse distance
    this->way_array[way]->preuse   = 0;

    // reset last access type
    this->way_array[way]->access_type = {0,0,0,0,0};
    this->way_array[way]->access_type[access_type] = 1;

    // reset recency
    this->way_array[way]->recency = 0;
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

//========================================================================//
void CacheState::updateState(int set, int way, bool is_hit, int access_type, vector<int> recency_list) {
    this->set_array[set]->updateState(way, is_hit, access_type, recency_list);
}

//========================================================================//
void CacheState::resetState(int set, int way, int access_type) {
    this->set_array[set]->resetState(way, access_type);
}
