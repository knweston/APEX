#ifndef NEURAL_MODULE_H
#define NEURAL_MODULE_H

#include <string>
#include "cache_state.h"
using namespace std;

class NeuralModule {
public:
    NeuralModule(int num_sets, int num_ways, string ip_address, int buffer_size=1024);
    ~NeuralModule();
    
    int    getPrediction(vector<double>& counters);
    void   retrain(vector<double>& counters);
    void   updateState(int set_id, int way_id, bool is_hit, int access_type);
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

    // runtime counters
    CacheState *cache;

    // prediction stats

};

#endif // NEURAL_MODULE_H