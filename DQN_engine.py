import socket
import sys

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
    def __init__(self, _network_arch, _num_inputs, _save_file_name):
        self.network = models.Sequential()
        print("New model created\n")
    
# ==================================================================================== #
# CLASS PREDICTION_SERVER
# ==================================================================================== #
class PredictionServer:
    def __init__(self):
        network_config = [384, 384, 256, 256, 128]
        num_inputs = 14
        self.prediction_engine = DQNengine(network_config, num_inputs, application_name)
        self.done_episodes = 0
        self.complete_signal = 0

    # start connection with client
    def start_server(self, port_no, ipaddress=''):
        # create a socket object
        s = socket.socket()
        print("Socket successfully created")

        # max wait time = 1200s
        s.settimeout(3000)

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

            prediction = self.prediction_engine.getPrediction(state)
            print("prediction: ", prediction)
            reply = str(prediction[0])
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
    port = int(sys.argv[2])
    ipaddress = sys.argv[3]
    server = PredictionServer(application_name)
    server.start(port, ipaddress)
    
if __name__ == "__main__":
    main()