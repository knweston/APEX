#include "neural_module.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

//========================================================================//
// CLASS NEURALMODULE
//========================================================================//
NeuralModule::NeuralModule(int num_sets, int num_ways, string ip_address, int buffer_size) {
    m_buffer_size = buffer_size;
    m_ipaddr      = ip_address;
    m_sock        = createSocket();
    cache         = new CacheState(num_sets, num_ways);
    connectServer();
}

//========================================================================//
NeuralModule::~NeuralModule() { 
    disconnectServer(); 
}

//========================================================================//
int NeuralModule::createSocket() {
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw "Socket creation error";
    return sock;
}

//========================================================================//
void NeuralModule::connectServer() {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, m_ipaddr.c_str(), &serv_addr.sin_addr) <= 0)
        throw "Invalid address/ Address not supported";

    if (connect(m_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        throw "Connection Failed";
}

//========================================================================//
void NeuralModule::disconnectServer() { 
    string reply = sendMessage("disconnect");
    cout << reply << endl;
    close(m_sock); 
}

//========================================================================//
string NeuralModule::sendMessage(string msg) {
    send(m_sock, msg.c_str(), strlen(msg.c_str()), 0);
    return getReply();
}

//========================================================================//
string NeuralModule::getReply() {
    char buffer[m_buffer_size];
    int valread = read(m_sock, buffer, m_buffer_size);

    string reply;
    for (int i=0; i < valread; ++i) 
        reply += buffer[i];
    return reply;
}

//========================================================================//
int NeuralModule::getPrediction(int set_id, int access_type) {
    int victim = 0;
    this->cache->resetState(set_id, victim, access_type);
    return victim;
}

//========================================================================//
void NeuralModule::updateState(int set_id, int way_id, bool is_hit, int access_type, vector<int> recency_list) {
    this->cache->updateState(set_id, way_id, is_hit, access_type, recency_list);
}

//========================================================================//
void NeuralModule::retrain() {

}