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
    NeuralModule(string conf_file, int num_sets, int num_ways, int buffer_size=1024);
    ~NeuralModule();
    
    int    predict(int set_id, int access_type, vector<unsigned long long> tags);
    void   retrain();
    void   sendSample(vector<double> sample);
    void   updateState( int set_id, int way_id, bool is_hit, int access_type, 
                        unsigned *recency_list, unsigned long long access_tag );
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