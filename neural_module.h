#ifndef NEURAL_MODULE_H
#define NEURAL_MODULE_H

#include <string>
#include "cache_state.h"
using namespace std;

//========================================================================//
// CLASS NEURALMODULE
//========================================================================//
class NeuralModule {
public:
    NeuralModule(int num_sets, int num_ways, string ip_address, int port, int buffer_size=1024);
    ~NeuralModule();
    
    int    predict(int set_id, int access_type);
    void   retrain();
    void   sendSample(vector<int> sample);
    void   addSampleCP(int set_id, int victim, vector<unsigned long long> tags);
    void   updateState(int set_id, int way_id, bool is_hit, int access_type, unsigned *recency_list, unsigned long long access_tag);
    string sendMessage(string msg);
    string getReply();
    void   connectServer();
    void   disconnectServer();

private:
    // connection config
    int m_port;
    int m_sock;
    int m_buffer_size;
    string m_ipaddr;
    int createSocket();

    // cache profiling data
    CacheState *cache;

    // prediction stats
    unsigned long long num_samples = 0;
    unsigned long long num_infer = 0;
};

#endif // NEURAL_MODULE_H