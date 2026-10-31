# Hamming Code -
data = input("Enter 4 data bits: ")
if len(data) != 4 or any(bit not in "01" for bit in data):
    print("Please enter exactly 4 binary bits")
    raise SystemExit
h = [0] * 8
h[3] = int(data[0])
h[5] = int(data[1])
h[6] = int(data[2])
h[7] = int(data[3])
h[1] = h[3] ^ h[5] ^ h[7]
h[2] = h[3] ^ h[6] ^ h[7]
h[4] = h[5] ^ h[6] ^ h[7]
code = ''.join(str(h[i]) for i in range(1, 8))
print("Hamming Code:", code)
pos = int(input("Enter error position (1-7, 0 for no error): "))
if pos < 0 or pos > 7:
    print("Invalid position")
    raise SystemExit
if pos != 0:
    h[pos] = 1 - h[pos]
received = ''.join(str(h[i]) for i in range(1, 8))
print("Received Code:", received)
s1 = h[1] ^ h[3] ^ h[5] ^ h[7]
s2 = h[2] ^ h[3] ^ h[6] ^ h[7]
s4 = h[4] ^ h[5] ^ h[6] ^ h[7]
error = s1 + (s2 * 2) + (s4 * 4)
print("Error Position:", error)
if error != 0:
    h[error] = 1 - h[error]
    print("Error detected and corrected")
else:
    print("No error detected")
corrected = ''.join(str(h[i]) for i in range(1, 8))
print("Corrected Code:", corrected)
original = str(h[3]) + str(h[5]) + str(h[6]) + str(h[7])
print("Original Data:", original)



#subnet mask
import ipaddress
network = ipaddress.ip_network(input("Enter network address: "), strict=False)
new_prefix = network.prefixlen + 1
subnets = list(network.subnets(new_prefix=new_prefix))
for i, subnet in enumerate(subnets, start=1):
    hosts = list(subnet.hosts())
    print("\nSubnet", i)
    print("Network Address:", subnet.network_address)
    print("Subnet Mask:", subnet.netmask)
    print("First Usable IP:", hosts[0])
    print("Last Usable IP:", hosts[-1])
    print("Broadcast Address:", subnet.broadcast_address)
    print("Usable Hosts:", len(hosts))



#DVR
n = int(input("Enter number of routers: "))
cost = []
print("Enter cost matrix (999 for no connection):")
for i in range(n):
    cost.append(list(map(int, input().split())))
dist = [row[:] for row in cost]
for k in range(n):
    for i in range(n):
        for j in range(n):
            if dist[i][j] > dist[i][k] + dist[k][j]:
                dist[i][j] = dist[i][k] + dist[k][j]
print("Shortest distance matrix:")
for row in dist:
    print(*row)



#TCP msg sending
server.py- 
import socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("localhost", 12345))
server.listen(1)
print("Waiting for client...")
conn, addr = server.accept()
msg = conn.recv(1024).decode()
print("Received message:", msg)
conn.close()
server.close()
 client.py- 
import socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("localhost", 12345))
msg = input("Enter message: ")
client.send(msg.encode())
client.close()



#TCP arithmetic operations
server.py:- 
import socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("localhost", 12345))
server.listen(1)
print("Waiting for client...")
conn, addr = server.accept()
data = conn.recv(1024).decode()
a, op, b = data.split()
a, b = float(a), float(b)
if op == '+':
    result = a + b
elif op == '-':
    result = a - b
elif op == '*':
    result = a * b
elif op == '/':
    result = a / b
else:
    result = "Invalid operator"
conn.send(str(result).encode())
conn.close()
server.close()                                                                                                                                                                                                                                                        
client.py:-
import socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("localhost", 12345))
a = input("Enter first number: ")
op = input("Enter operator (+, -, *, /): ")
b = input("Enter second number: ")
client.send(f"{a} {op} {b}".encode())
result = client.recv(1024).decode()
print("Result:", result)
client.close()



#UDP
server.py:-                                                                                    
 import socket
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.bind(("localhost", 12345))
print("Waiting for client...")
msg, addr = server.recvfrom(1024)
msg = msg.decode().upper()
print("Uppercase:", msg)
server.sendto(msg.encode(), addr)
server.close()                                                                                                           
client:- 
import socket
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
msg = input("Enter lowercase string: ")
client.sendto(msg.encode(), ("localhost", 12345))
data, addr = client.recvfrom(1024)
print("Converted string:", data.decode())
client.close()
