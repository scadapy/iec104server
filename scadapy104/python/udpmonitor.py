import socket
import sys
import time
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('localhost', 64000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)
while True:
    data, address = sock.recvfrom(4096)
    print(data)
