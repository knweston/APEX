import sys
from DQNengine import DQNengine 
from PredictionServer import PredictionServer

# L1: 64KB with associativity 4
# L2: 256KB with associativity 8
# L3: 4MB with associativity 16

# ==================================================================================== #
# MAIN FUNCTION
# ==================================================================================== #
def readPortConfig(filename):
    port_num = 0
    conf_file = open(filename, "r")
    while True:
        line = conf_file.readline()
        if not line:
            break
        else:
            if line.find("port") != -1:
                port_num = int(line.split("=")[1])
                break
    return port_num

def readIPConfig(filename):
    conf_file = open(filename, "r")
    while True:
        line = conf_file.readline()
        if not line:
            break
        else:
            if line.find("ipaddress") != -1:
                return (line.split("=")[1]).rstrip().lstrip()

def main():
    app_name     = sys.argv[1]
    weight_file  = sys.argv[2]
    server_conf  = sys.argv[3]  
    num_features = int(sys.argv[4])
    num_output   = int(sys.argv[5])
    port         = readPortConfig(server_conf)
    ip_address   = readIPConfig(server_conf)

    # Currently num_features = 8 per cache line
    pred_engine = DQNengine(num_features, num_output, 20000, app_name, _checkpt=weight_file)
    server = PredictionServer(pred_engine)
    server.start(port, ip_address)
    
    
if __name__ == "__main__":
    main()