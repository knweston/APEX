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
# CLASS SAMPLE
# ==================================================================================== #
class Sample:
    def _init__(self, _state, _action, _next_state, _reward):
        self.state      = _state
        self.action     = _action
        self.next_state = _next_state
        self.reward     = _reward


# ==================================================================================== #
# CLASS DQN_ENGINE
# ==================================================================================== #
class DQNengine:
    def __init__(self, _num_input, _num_output, replay_size, _checkpt="none"):
        self.gamma    = 0.95    # discounting factor
        self.alpha    = 0.05    # learning rate
        self.eps      = 0.05    # exploration rate
        self.network  = createModel(_num_input, _num_output, _checkpt)
        self.num_actions   = _num_output
        self.replay_buffer = deque(maxlen=replay_size)
        if _checkpt == "none":
            self.mcp_save = ModelCheckpoint('weights.hdf5', save_best_only=True, monitor='loss', mode='min')
        else:    
            self.mcp_save = ModelCheckpoint(_checkpt+'.hdf5', save_best_only=True, monitor='loss', mode='min')
        print("New model created\n")
    
    def createModel(self, _num_input, _num_output, _checkpoint):
        model = Sequential()
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
            self.num_random_taken += 1
            print("Random action: " + str(action))
            return action
        else:
            x_input = [state]
            x_input = np.array(x_input)
            action = np.argmax(self.network.predict(x_input))
            print("Learned action: " + str(action))
            return action 

    def addSample(data):
        state      = []
        action     = data[15]
        next_state = []
        reward     = data[-1]

        for index in range(14):
            state.append(data[index])
        for index in range(15, 29)
            next_state.append(data[index])
        
        new_sample = Sample(state, action, next_state, reward)
        self.replay_buffer(new_sample)

    def retrain(self, draw_size=64, train_batch_size=32, num_epochs=5):
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
        for index in range(len(Q_batches)):
            current_Q_batches[index][actions_batch[index]] += self.alpha * \
                                                              (rewards_batch[index] + \
                                                              self.gamma * np.max(next_Q_batches[index]) - \
                                                              current_Q_batches[index][actions_batch[index]])  
        
        self.model.fit(states_batch, Q_batches, batch_size=train_batch_size, verbose=False, epochs=num_epochs)


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
            reply = "Connected"
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
            print("state: ", state)

            prediction = self.prediction_engine.predict(state)
            print("prediction: ", prediction)
            reply = str(prediction[0])
            connection.send(reply.encode())

        elif request == "retrain":
            self.prediction_engine.retrain()
            reply = "Retrained successfully"
            connection.send(reply.encode())

        elif request == "new sample":
            reply = "Connected"
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
    num_input  = int(sys.argv[4])
    num_output = int(sys.argv[5])

    # Currently num_input = 8 per cache line
    pred_engine = DQNengine(application_name, num_input, num_output, 5000)
    server = PredictionServer(pred_engine)
    server.start(port, ipaddress)
    
if __name__ == "__main__":
    main()