import socket

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 54000        # The port used by the server

# Create the socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while True:
        # Take a message from the user
        # Send it to the server
        # Print the response to the console
        message = input()
        s.send(message.encode())
        data = s.recv(1024)
        print(data.decode())

