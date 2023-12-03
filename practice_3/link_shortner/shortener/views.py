from django.shortcuts import render, redirect, HttpResponse
from django.views import View
from .forms import URLForm
import time
import socket
import json
from datetime import datetime

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

    def sendHT(self, name, key, value):
        filepath = 'file.data'
        query = "HSET " + name + " " + str(value) + " " + str(key) + '\0'
        client_socket = self.sendTCP(filepath, query)
        client_socket.close()

    def recvHT(self, name, value):
        filepath = 'file.data'
        query = "HGET " + name + " " + str(value) + '\0'
        client_socket = self.sendTCP(filepath, query)

        received_data = b''
        received_data = client_socket.recv(4)
        len = ord(received_data.decode()[0])
        received_data = client_socket.recv(len)


        received_data = received_data.decode("latin-1")
        client_socket.close()
        return received_data


    def sendQ(self, name, value):
        filepath = 'stats.data'
        query = "QPUSH " + name + " " + str(value) + '\0'
        client_socket = self.sendTCP(filepath, query)
        client_socket.close()

def get_client_ip(request):
    x_forwarded_for = request.META.get('HTTP_X_FORWARDED_FOR')
    if x_forwarded_for:
        ip = x_forwarded_for.split(',')[0]
    else:
        ip = request.META.get('REMOTE_ADDR')
    return ip

class RedirectView(View):
    def get(self, request, short_url):
        if request.path == '/favicon.ico':
            return HttpResponse('')

        if(short_url == "report"):
            return redirect("report/")
        else:
            newTCP = TCPClient()
            url = newTCP.recvHT("urls", short_url)
            self.req(get_client_ip(request), f"{url} ({short_url})")
            return redirect(url)

    def req(self, ip, url):
        now = datetime.now()
        time = f"{now.hour:02d}:{now.minute:02d}"
        report = {
            "ip": ip,
            "url": url,
            "time": time
        }
        report = json.dumps(report)
        newTCP = TCPClient()
        newTCP.sendQ("new", report)
        print(report)


def shorten_url(request):
    if request.method == 'POST':
        form = URLForm(request.POST)
        if form.is_valid():
            long_url = form.cleaned_data['long_url']
            id = str(time.time())
            short_url = id[0:7] + id[15:17]

            newTCP = TCPClient()
            newTCP.sendHT("urls", long_url, short_url)
            return render(request, 'shortener/index.html', {'form': form, 'short_url': "http://127.0.0.1:8000/"+short_url})
    else:
        form = URLForm()
    return render(request, 'shortener/index.html', {'form': form})