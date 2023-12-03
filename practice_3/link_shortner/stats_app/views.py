from django.shortcuts import render
from django.http import JsonResponse
import socket
import json


class TCPClient:
    BUFFER_SIZE = 4096*2
    IP = '127.0.0.1'
    PORT = 6379
    filepath = 'stats.data'
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

    def sendHT(self, name, key, value):
        query = "HSET " + name + " " + str(value) + " " + str(key) + '\0'
        client_socket = self.sendTCP(self.filepath, query)
        client_socket.close()

    def recvHT(self, name, value):
        query = "HGET " + name + " " + str(value) + '\0'
        client_socket = self.sendTCP(self.filepath, query)

        received_data = b''
        received_data = client_socket.recv(4)
        len = ord(received_data.decode("latin-1")[0])
        received_data = client_socket.recv(len*1000)


        received_data = received_data.decode("latin-1")
        client_socket.close()
        return received_data

    def delHT(self, name, key):
        query = "HDEL " + name + " " + str(key) + '\0'
        client_socket = self.sendTCP(self.filepath, query)
        client_socket.close()

    def recvQ(self, name):
        query = "QPOP " + name + '\0'
        s = ""
        for i in range(3):
            client_socket = self.sendTCP(self.filepath, query)

            received_data = b''
            received_data = client_socket.recv(4)
            len = ord(received_data.decode("latin-1")[0])
            received_data = client_socket.recv(len * 1000)

            received_data = received_data.decode("latin-1")
            s+= received_data + ", "
            client_socket.close()
        return s

class report:
    report_logs = []
    ht_name = "logs"
    k = "key"

    def add(self, log):
        id = self.urlInReport(log["url"])
        if id == -1:
            log_form = {
                "URL": log['url'],
                "count": 1,
                "time": [
                    {"Time": log['time'], "IP": log['ip']},
                ],
            }
            self.report_logs.append(log_form)
        else:
            self.report_logs[id]['count'] = self.report_logs[id]['count'] + 1
            self.report_logs[id]['time'].append({"Time": log['time'], "IP": log['ip']})
        self.update()

    def urlInReport(self, url):
        for i in range(len(self.report_logs)):
            if self.report_logs[i]['URL'] == url:
                return i
        return -1

    def create(self):
        newTCP = TCPClient()
        newTCP.sendHT(self.ht_name, json.dumps(self.report_logs), "key")

    def get(self):
        newTCP = TCPClient()
        temp = newTCP.recvHT(self.ht_name, self.k)
        if "No key" not in temp:
            self.report_logs = json.loads(temp)

    def checkUpdates(self):
        newTCP = TCPClient()
        while(True):
            temp = newTCP.recvQ("new")
            if "is empty" in temp:
                print("\tNo new logs")
                break
            print("\t", temp)
            new_log = json.loads(temp[:len(temp)-2])
            self.add(new_log)
        self.update()

    def update(self):
        newTCP = TCPClient()
        newTCP.delHT(self.ht_name, self.k)
        self.create()

    def createReport(self):
        html = ''
        ips = set()
        s = set()
        for i in self.report_logs:
            html += f"<pre>{ i['URL'] } ( { i['count'] } )</pre>"
            for t in i['time']:
                s.add(t['Time'])
            for t in s:
                print(t)
                c = 0
                c_ips = 0
                for k in i['time']:
                    if t == k['Time']:
                        ips.add(k['IP'])
                        c+=1
                        c_ips+=1
                        print(k)
                html += f"<pre>\t{ t } ( {c} )</pre>"
                for k in ips:
                    html += f"<pre>\t\t{k} ( {c_ips} )</pre>"
                ips.clear()
        return html


def report_stats(request):
    reports = report()
    reports.get()
    reports.checkUpdates()

    return render(request, 'report/report.html', {'items': reports.createReport()})
    #return JsonResponse(reports.report_logs, safe=False)
