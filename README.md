# Introduction: APEX Simulator
APEX simulator is used to test APEX replacement policy (Adaptive Replacement Policy for Exclusive Cache using Reinforcement Learning). It is based on exclusiu, made by Professor Daniel Jimenez at Texas A&M University. Exclusiu is deeply modified to host a Q-learning engine, which is the core of APEX. There are two separate parts of APEX simulator: the prediction engine and cache simulator.
- Q-learning engine: written in Python, it is setup as a server. 
- Cache simulator: simulate the normal behavior of an exclusive cache system.

When there is a replacement request, the cache simulator will send a message to the Q-earning server. Included in this message is the state of the cache at that time. The server will take the cache state as the input, and return the optimal replacement candidate to the cache simulator. 

After a while, the cache simulator computes the reward and sends it back to the Q-learning server. The server saves the reward along with the previous cache state as a training sample in the replay buffer. For every 5000 training sample collected, the Q-learning neural network is retrained.

# Setup Instruction
- Clone this github repo
- Locate the APEX folder
- Enter: make all

# How to run
- Download the memory trace files: http://faculty.cse.tamu.edu/djimenez/614/traces.tar
- Export the desired replacement policy by entering: export DAN_POLICY=[#]; with [#] ranges from 0-2. 0 is LRU, 1 is random, 2 is APEX. For example, if you want to run APEX policy, type: export DAN_POLICY=2; At default, the simulator will use the LRU replacement policy.
- Export the server configuration file location: export SERVER_CONF=server/config/filename; there is one config for each trace. All config files are stored in server/config
- Two terminal windows is required to run this simulator: one is for the Q-learning server, one for the cache simulator. Note that the Q-learning server must be run before the cache simulator.

To start the server, enter: python3 server/main.py trace_name weight_file 8 16
To start the cache simulator, enter: ./exclusiu path_to_the_trace_file

Please refer to exclusiu's GitHub page for more information: https://github.tamu.edu/djimenez/exclusiu

