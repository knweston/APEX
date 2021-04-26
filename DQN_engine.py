import socket
import sys
import random
from collections import deque

import warnings
with warnings.catch_warnings():
    warnings.filterwarnings("ignore",category=FutureWarning)
    import numpy as np
    from numpy import savetxt
    from numpy import loadtxt
    from keras import models
    from keras import layers
    from keras import optimizers
    from keras.callbacks import ModelCheckpoint

import os
import tensorflow as tf
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

try:
    from tensorflow.python.util import module_wrapper as deprecation
except ImportError:
    from tensorflow.python.util import deprecation_wrapper as deprecation
deprecation._PER_MODULE_WARNING_LIMIT = 0


# ==================================================================================== #
# CLASS DQN_ENGINE
# ==================================================================================== #
class DQNengine:
    def __init__(self, _num_features, _num_output, replay_size, _checkpt="none"):
        self.gamma    = 0.95    # discounting factor
        self.alpha    = 0.05    # learning rate
        self.eps      = 0.05    # exploration rate
        self.num_actions   = _num_output
        self.num_features  = _num_features
        self.num_retrains = 0
        self.network = self.createModel(self.num_actions*self.num_features, _num_output, _checkpt)
        self.replay_buffer = deque(maxlen=replay_size)
        if _checkpt == "none":
            self.mcp_save = ModelCheckpoint('weights.hdf5', save_best_only=True, monitor='loss', mode='min')
        else:    
            self.mcp_save = ModelCheckpoint(_checkpt+'.hdf5', save_best_only=True, monitor='loss', mode='min')
        print("New model created\n")
    
    def createModel(self, _num_input, _num_output, _checkpoint):
        model = models.Sequential()
        model.add(layers.Dense(16, activation='tanh', input_shape=(_num_input,)))
        model.add(layers.Dense(16, activation='tanh'))
        model.add(layers.Dense(_num_output))
        if _checkpoint != "none":
            model.load_weights(_checkpoint)
        model.compile(optimizer='adam', loss='mse', metrics=['mae'])
        return model
    
    def predict(self, state):
        if np.random.randint(0,99) < int(self.eps * 100):
            action = np.random.randint(0,self.num_actions)
            return action
        else:
            x_input = [state]
            x_input = np.array(x_input)
            action = np.argmax(self.network.predict(x_input))
            return action 

    def addSample(self, data):
        state      = []
        next_state = []
        action     = data[-2]
        reward     = data[-1]

        state_length = self.num_features*self.num_actions
        for index in range(state_length):
            state.append(data[index])
        for index in range(state_length, 2*state_length):
            next_state.append(data[index])
        
        self.replay_buffer.append((state, int(action), next_state, reward))

    def retrain(self, draw_size=128, train_batch_size=32, num_epochs=2):
        # sanity check: there must be something in the replay buffer
        assert(len(self.replay_buffer))
        
        # sanity check: draw size always > batch size
        assert(draw_size >= train_batch_size)

        if len(self.replay_buffer) >= draw_size:
            batch = random.sample(self.replay_buffer, draw_size)
        else:
            batch = self.replay_buffer
            if len(self.replay_buffer) < train_batch_size:
                train_batch_size = len(self.replay_buffer)

        states_batch, actions_batch, next_states_batch, rewards_batch = map(np.array, zip(*batch))
        current_Q_batches = self.network.predict(states_batch)
        next_Q_batches = self.network.predict(next_states_batch)

        # update new Q-value: Q(s,a) = Q(s,a) + alpha * (reward + gamma * maxQ(s',a') - Q(s,a))
        for index in range(len(current_Q_batches)):
            current_Q_batches[index][actions_batch[index]] += self.alpha * \
                                                              (rewards_batch[index] + \
                                                              self.gamma * np.max(next_Q_batches[index]) - \
                                                              current_Q_batches[index][actions_batch[index]])  
        
        self.network.fit(states_batch, current_Q_batches, batch_size=train_batch_size, \
                        verbose=False, epochs=num_epochs, callbacks=[self.mcp_save])
        self.num_retrains += 1    


# ==================================================================================== #
# CLASS PREDICTION_SERVER
# ==================================================================================== #
class PredictionServer:
    def __init__(self, pred_engine):
        self.prediction_engine = pred_engine
        self.done_episodes     = 0
        self.complete_signal   = 0

    # start connection with client
    def start_server(self, port_no, ipaddress=''):
        # create a socket object
        s = socket.socket()
        print("Socket successfully created")

        # max wait time = 1200s
        s.settimeout(1200)

        # reserve a port on your computer in our
        # case it is 12345 but it can be anything
        port = port_no

        # next bind to the port
        # we have not typed any ip in the ip field
        # instead we have input an empty string
        # this makes the server listen to requests
        # coming from other computers on the network
        s.bind((ipaddress, port))
        print("socket binded to %s" % (port))

        # put the socket into listening mode
        s.listen(5)
        print("socket is listening")
        return s
    
    # process client request
    def process_request(self, request, connection):
        
        if len(request) == 0:
            return
        
        elif request == "make prediction":
            reply = "Request received: make inference"
            connection.send(reply.encode())
            state = []
            while True:
                data = connection.recv(1024).decode()
                if len(data) == 0:
                    break
                elif data == "E":
                    break
                else:
                    state.append(float(data))
                    reply = "Next"
                    connection.send(reply.encode())

            prediction = self.prediction_engine.predict(state)
            reply = str(prediction)
            connection.send(reply.encode())

        elif request == "retrain":
            self.prediction_engine.retrain()
            reply = str(prediction_engine.num_retrains)
            connection.send(reply.encode())

        elif request == "new sample":
            reply = "Request received: add sample"
            connection.send(reply.encode())
            sample_data = []
            while True:
                data = connection.recv(1024).decode()
                if len(data) == 0:
                    break
                elif data == "E":
                    break
                else:
                    sample_data.append(float(data))
                    reply = "Next"
                    connection.send(reply.encode())
            self.prediction_engine.addSample(sample_data)
            reply = "New sample added"
            connection.send(reply.encode())

        elif request == "disconnect":
            self.complete_signal = 1
            reply = "Server disconnected successfully"
            connection.send(reply.encode())

        else:
            reply = "unknown request"
            connection.send(reply.encode())

    def start(self, port_no, ipaddress):
        sock = self.start_server(port_no, ipaddress)
        
        # Establish connection with client 0.
        conn, addr = sock.accept()
        print('Connected to ', addr)
        
        self.complete_signal = 0
        while self.complete_signal < 1:
            request = conn.recv(1024).decode()
            self.process_request(request, conn)

        # Close the connection with the client
        conn.close()
    

# ==================================================================================== #
# MAIN FUNCTION
# ==================================================================================== #
def main():
    application_name = sys.argv[1]
    port       = int(sys.argv[2])
    ipaddress  = sys.argv[3]
    num_features  = int(sys.argv[4])
    num_output = int(sys.argv[5])

    # L1: 64KB with associativity 4
    # L2: 256KB with associativity 8
    # L3: 4MB with associativity 16

    # Currently num_features = 8 per cache line
    pred_engine = DQNengine(num_features, num_output, 10000, application_name)
    server = PredictionServer(pred_engine)
    server.start(port, ipaddress)
    
if __name__ == "__main__":
    main()