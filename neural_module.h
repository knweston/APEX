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
    NeuralModule(int num_sets, int num_ways, string ip_address, int buffer_size=1024);
    ~NeuralModule();
    
    int    predict(int set_id, int access_type);
    void   retrain();
    void   addSample();
    void   updateState(int set_id, int way_id, bool is_hit, int access_type, vector<int> recency_list);
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

};

#endif // NEURAL_MODULE_H