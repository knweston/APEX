#ifndef CACHE_STATE_H
#define CACHE_STATE_H

#include <vector>
using namespace std;

//========================================================================//
// CLASS WAYSTATE
//========================================================================//
struct WayState {
    // preuse distance
    int preuse = 0;

    // last access type
    vector<int> access_type = {0, 0, 0, 0, 0};

    // line recency
    int recency = 0;

    // hits since insertion
    int num_hits = 0;
};


//========================================================================//
// CLASS SETSTATE
//========================================================================//
class SetState {
public:
    SetState(int num_ways);
    void updateState(int way, bool is_hit, int access_type);

private:
    vector<WayState*> way_array;
};


//========================================================================//
// CLASS CACHESTATE
//========================================================================//
class CacheState {
public:
    CacheState(int num_sets, int num_ways);
    SetState getState(int set, int way);
    void updateState(int set, int way, bool is_hit, int access_type);

private:
    int m_nsets;
    int m_nways;
    vector<SetState*> set_array;
};

#endif // CACHE_STATE_H