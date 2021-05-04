# APEX
Adaptive Replacement Policy for Exclusive Cache using Reinforcement Learning

Please download the memory trace files at this link: http://faculty.cse.tamu.edu/djimenez/614/traces.tar

To compile, using the terminal to locate the APEX folder, then enter: make all

At default, the simulator will use the LRU replacement policy. To use APEX policy, enter
export DAN_POLICY=2;

Two terminal windows is required to run this simulator: one is for the DQN engine server, one for the cache simulator. Note that the DQN engine server must be run before the cache simulator.

To start the server, enter: python3 server/main.py app_name weight_file 8 16

The cache simulator used in this project is based on the "exclusiu" simulator of Professor Daniel Jimenez at Texas A&M University.
Please refer to exclusiu's README for more information: https://github.tamu.edu/djimenez/exclusiu

