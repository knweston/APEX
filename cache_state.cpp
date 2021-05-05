#include "cache_state.h"
#include <iostream>
#include <cstring>

using namespace std;
//========================================================================//
// HELPER FUNCTIONS
//========================================================================//
int argmax(vector<int> _input) {
    int max_value = _input[0];
    int max_index = 0;
    for (unsigned i=1; i < _input.size(); ++i) {
        if (_input[i] > max_value) {
            max_value = _input[i];
            max_index = i;
        }
    }
    return max_index;
}

int max(vector<int> _input) {
    int max_value = _input[0];
    for (unsigned i=1; i < _input.size(); ++i) {
        if (_input[i] > max_value)
            max_value = _input[i];
    }
    return max_value;
}

//========================================================================//
// CLASS WAYSTATE
//========================================================================//
WayState::WayState(const WayState& src) {
        this->preuse      = src.preuse;
        this->access_type = src.access_type;
        this->recency     = src.recency;
        this->num_hits    = src.num_hits;
    }

void WayState::print() {
    cout << "preuse      = " << preuse << endl;
    cout << "recency     = " << recency << endl;
    cout << "num_hits    = " << num_hits << endl;
    for (unsigned i=0; i < access_type.size(); ++i) {
        if (access_type[i] == 1)
            cout << "access_type = " << i << endl;
    }
}
//========================================================================//
// CLASS SETSTATE
//========================================================================//
SetState::SetState(int num_ways) {
    // initialize way state array
    for (int i=0; i < num_ways; ++i)
        this->way_array.push_back(new WayState());
}

SetState::~SetState() {
    for (unsigned i=0; i < way_array.size(); ++i) {
        delete way_array[i];
    }
    way_array.clear();
}

SetState::SetState(const SetState& src) {
    for (unsigned i=0; i < src.way_array.size(); ++i)
        this->way_array.push_back(new WayState(*src.way_array[i]));
}

void SetState::updateState(int way, bool is_hit, int access_type, unsigned *recency_list) {
    // update way preuse distance
    for (unsigned i=0; i < this->way_array.size(); ++i)
        this->way_array[way]->preuse++;

    // update way last access type
    this->way_array[way]->access_type = {0,0,0,0,0};
    this->way_array[way]->access_type[access_type] = 1;

    // // update way recency
    for (unsigned i=0; i < this->way_array.size(); ++i)
        this->way_array[i]->recency = recency_list[i];

    // update way num_hits
    if (is_hit) {
        this->way_array[way]->num_hits++;
        this->way_array[way]->preuse = 0;
    }
}

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

vector<double> SetState::flatten(bool normalize) {
    vector<double> state;
    vector<int> numhit_arr;
    vector<int> preuse_arr;
    for (unsigned way=0; way < way_array.size(); ++way) {
        numhit_arr.push_back(way_array[way]->num_hits);
        preuse_arr.push_back(way_array[way]->preuse);
    }
    int max_preuse = max(preuse_arr);
    int max_numhit = max(numhit_arr);

    for (unsigned way=0; way < way_array.size(); ++way) {
        if (normalize) {
            state.push_back((double)way_array[way]->preuse/max_preuse);
            for (unsigned i=0; i < way_array[way]->access_type.size(); ++i)
                state.push_back(way_array[way]->access_type[i]);
            state.push_back((double)way_array[way]->recency/way_array.size());
            state.push_back((double)way_array[way]->num_hits/max_numhit);
        }
        else {
            state.push_back((double)way_array[way]->preuse/max_preuse);
            for (unsigned i=0; i < way_array[way]->access_type.size(); ++i)
                state.push_back(way_array[way]->access_type[i]);
            state.push_back((double)way_array[way]->recency/way_array.size());
            state.push_back((double)way_array[way]->num_hits/max_numhit);
        }
    }
    return state;
}


//========================================================================//
// CLASS SAMPLE CHECKPOINT
//========================================================================//
SampleCP::SampleCP(vector<unsigned long long> _tags, int _num_ways, SetState& _st, SetState& _nxt_st, int _v) {
    state      = new SetState(_st);
    next_state = new SetState(_nxt_st);
    reused     = {'n','n','n','n','n','n','n','n'};
    num_ways = _num_ways;
    victim     = _v;
    reward     = 0;
    ready      = false;
    tags       = _tags;
}

bool SampleCP::updateSample(unsigned long long access_tag) {
    int other_reused = 0;

    // Check if this is a hit
    for (int way=0; way < num_ways; ++way) {
        if (access_tag == tags[way]) {
            reused[way] = 'y';
            break;
        }
    }

    // Check how many other ways have been reaccessed
    for (int way=0; way < num_ways; ++way) {
        if (way != victim && reused[way] == 'y')
            ++other_reused;
    }

    // update reward
    if (reused[victim] == 'n' && other_reused == num_ways-1) {
        reward = 1;
        ready = true;
        return true;
    }
    else if (reused[victim] == 'y' && other_reused < num_ways-1) {
        reward = -1;
        ready = true;
        return true;
    }
    else
        return false;
}

vector<double> SampleCP::flatten(bool normalize) {
    vector<double> sample = state->flatten(normalize);
    vector<double> next_state_flat = next_state->flatten(normalize);
    for (unsigned i=0; i < next_state_flat.size(); ++i) {
        sample.push_back(next_state_flat[i]);
    }
    if (normalize) {
        sample.push_back((double)victim/num_ways);   // also the selected action
    }
    else {
        sample.push_back(victim);
    }
    sample.push_back(reward);
    return sample;
}


//========================================================================//
// CLASS CACHESTATE
//========================================================================//
CacheState::CacheState(int num_sets, int num_ways) {
    m_nsets = num_sets;
    m_nways = num_ways;
    
    // initialize set state array
    for (int i=0; i < m_nsets; ++i) {
        this->set_array.push_back(new SetState(m_nways));
        vector<SampleCP*> set_samples;
        this->samples.push_back(set_samples);
    }
}

CacheState::~CacheState() {
    for (unsigned i=0; i < set_array.size(); ++i) {
        delete set_array[i];
    }
    set_array.clear();
}

void CacheState::updateState(int set, int way, bool is_hit, int access_type, unsigned *recency_list) {
    this->set_array[set]->updateState(way, is_hit, access_type, recency_list);
    
}

void CacheState::resetState(int set, int way, int access_type) {
    this->set_array[set]->resetState(way, access_type);
}

void CacheState::createNewSample(int set, int victim, SetState &curr_st, SetState &next_st, vector<unsigned long long> tags) {
    SampleCP *new_sample = new SampleCP(tags, m_nways, curr_st, next_st, victim);
    samples[set].push_back(new_sample);
}

void CacheState::cleanSampleBuffer(int set) {
    for (unsigned i=0; i < samples[set].size(); ++i) {
        if (samples[set][i]->isReady()) {
            delete samples[set][i];
            samples[set].erase(samples[set].begin()+i);
            i=0;
        }
    }
}

int CacheState::totalSamples() {
    int size = 0;
    for (unsigned i=0; i < samples.size(); ++i) {
        size += samples[i].size();
    }
    return size;
}