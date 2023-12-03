import json
arr = [{"URL": "https://www.office.com/ (170153211)","count": 1,"time": [{"Time": "22:54", "IP": "127.0.0.1"}]},{"URL": "https://www.office.com/ (171153211)","count": 3,"time": [{"Time": "22:55", "IP": "127.0.0.1"}, {"Time": "22:55", "IP": "127.0.0.1"}, {"Time": "22:55", "IP": "127.1.0.1"}]}]





report_logs = [
        {
            "URL": "https://www.office.com/ (170153211)",
            "count": 1,
            "time": [
                {"Time": "22:54", "IP": "127.0.0.1"},
            ],
        },

    ]

























class TCPClient():
    BUFFER_SIZE = 4096
    IP = '127.0.0.1'
    PORT = 6379
    def createConnection(self):
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (self.IP, self.PORT)
        client_socket.connect(server_address)
        client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, self.BUFFER_SIZE)
        return client_socket

    def sendTCP(self, filepath, query):
        client_socket = self.createConnection()
        path_length = len(filepath).to_bytes(4, 'little')
        client_socket.send(path_length)
        path_length = len(query).to_bytes(4, 'little')
        client_socket.send(path_length)
        client_socket.send(filepath.encode())
        client_socket.send(query.encode())
        return client_socket


    def sendQ(self, name, value):
        filepath = 'stats.data'
        query = "QPUSH " + name + " " + str(value) + '\0'
        client_socket = self.sendTCP(filepath, query)
        client_socket.close()

    def recvQ(self, name):
        filepath = 'stats.data'
        query = "QPOP " + name + '\0'

        s = ""
        for i in range(3):
            client_socket = self.sendTCP(filepath, query)
            received_data = b''
            received_data = client_socket.recv(4)
            len = ord(received_data.decode()[0])
            received_data = client_socket.recv(len)

            received_data = received_data.decode("latin-1")
            if "queue is empty" in received_data:
                return -1
            s+= received_data + ", "
            client_socket.close()
        return s

def report_stats(request):
    newTCP = TCPClient()
    s = ""
    arr = []
    while(True):
        test = newTCP.recvQ("logs")
        if test == -1:
            break
        test = test[:len(test)-2]
        s+=test+", "
        arr.append(test)
    s = s[:len(s)-2]
    s = "[" + s + "]"
    print(s)
    data = json.loads(s)
    for i in range(len(arr)):
        newTCP.sendQ("logs", arr[i])
    return JsonResponse(data, safe=False)
