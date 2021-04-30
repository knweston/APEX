#ifndef REPL_STATE_H
#define REPL_STATE_H
 
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is distributed as part of the Cache Replacement Championship     //
// workshop held in conjunction with ISCA'2010.                               //
//                                                                            //
//                                                                            //
// Everyone is granted permission to copy, modify, and/or re-distribute       //
// this software.                                                             //
//                                                                            //
// Please contact Aamer Jaleel <ajaleel@gmail.com> should you have any        //
// questions                                                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cassert>
#include "utils.h"
#include "crc_cache_defs.h"
#include <iostream>
#include "neural_module.h"

using namespace std;

// Replacement Policies Supported
typedef enum {
    CRC_REPL_LRU        = 0,
    CRC_REPL_RANDOM     = 1,
    CRC_REPL_CONTESTANT = 2
} ReplacemntPolicy;


// Replacement State Per Cache Line
typedef struct {
    UINT32  LRUstackposition;
    // CONTESTANTS: Add extra state per cache line here
} LINE_REPLACEMENT_STATE;


struct sampler; // Jimenez's structures


// The implementation for the cache replacement policy
class CACHE_REPLACEMENT_STATE {
public:
    LINE_REPLACEMENT_STATE   **repl;

private:
    NeuralModule *neural_module;
    UINT32 numsets;
    UINT32 assoc;
    UINT32 replPolicy;
    COUNTER num_accesses;  // tracks # of references to the cache
    string server_conf_file;

    // CONTESTANTS:  Add extra state for cache here

  public:
    ostream & PrintStats(ostream &out);

    // The constructor CAN NOT be changed
    CACHE_REPLACEMENT_STATE( UINT32 _sets, UINT32 _assoc, UINT32 _pol, string _server_conf_file );
    ~CACHE_REPLACEMENT_STATE(void);
    void   IncrementTimer() { num_accesses++; } 
    INT32  GetVictimInSet( UINT32 tid, UINT32 setIndex, const LINE_STATE *vicSet, UINT32 assoc, Addr_t PC, 
                            Addr_t paddr, UINT32 accessType, UINT32 accessSource, vector<unsigned long long> tags);
    void   SetReplacementPolicy( UINT32 _pol ) { replPolicy = _pol; } 
    void   UpdateReplacementState( UINT32 setIndex, INT32 updateWayID, const LINE_STATE *currLine, 
                                   UINT32 tid, Addr_t PC, UINT32 accessType, bool cacheHit, UINT32 accessSource);
    void   finishExecution() { neural_module->disconnectServer(); }
    
  private:  
    void   InitReplacementState();
    INT32  Get_Random_Victim( UINT32 setIndex );
    INT32  Get_LRU_Victim( UINT32 setIndex );
    INT32  Get_My_Victim( UINT32 setIndex, UINT32 accessType, vector<unsigned long long> tags );
    void   UpdateLRU( UINT32 setIndex, INT32 updateWayID );
    void   UpdateMyPolicy( UINT32 setIndex, INT32 updateWayID, bool cacheHit, UINT32 accessType, UINT64 accessTag );
};

#endif
