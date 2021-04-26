#ifndef CACHE_STATE_H
#define CACHE_STATE_H

#include <vector>
using namespace std;

//========================================================================//
// CLASS WAYSTATE
//========================================================================//
struct WayState {
    WayState() {}

    ~WayState() {}

    // preuse distance
    int preuse = 0;

    // last access type
    vector<int> access_type = {0,0,0,0,0};

    // line recency
    unsigned recency = 0;

    // hits since insertion
    int num_hits = 0;
};


//========================================================================//
// CLASS SETSTATE
//========================================================================//
class SetState {
public:
    SetState() {};
    SetState(int num_ways);
    ~SetState();
    SetState(const SetState& src);
    vector<WayState*> getWayArray() { return way_array; }
    void updateState(int way, bool is_hit, int access_type, unsigned *recency_list);
    void resetState(int way, int access_type);
    vector<int> flatten();

private:
    vector<WayState*> way_array;
};


//========================================================================//
// CLASS SAMPLE CHECKPOINT
//========================================================================//
class SampleCP {
public:
    SampleCP(vector<unsigned long long> _tags, int _num_ways, SetState& _st, SetState& _nxt_st, int _v);
    ~SampleCP() {}
    bool updateSample(unsigned long long access_tag);    // return true if the sample is ready to send
    vector<int> flatten();
    bool isReady() { return ready; }

private:
    vector<unsigned long long> tags;
    vector<char> reused;
    SetState *state;
    SetState *next_state;
    int victim;
    int reward;
    int num_ways;
    bool ready;
};


//========================================================================//
// CLASS CACHESTATE
//========================================================================//
class CacheState {
public:
    CacheState(int num_sets, int num_ways);
    ~CacheState();
    SetState* getSetState(int set) { return this->set_array[set]; }
    vector<SampleCP*> getSetSampleList(int set) { return samples[set]; }
    void updateState(int set, int way, bool is_hit, int access_type, unsigned *recency_list);
    void resetState(int set, int way, int access_type);
    void createNewSample(int set, int victim, vector<unsigned long long> tags);
    void cleanSampleBuffer(int set);
    int  totalSamples();

private:
    int m_nsets;
    int m_nways;
    vector<SetState*> set_array;
    vector<vector<SampleCP*>> samples;  // one sample list per set
};

#endif // CACHE_STATE_H