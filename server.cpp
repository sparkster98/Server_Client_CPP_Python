#include <iostream>
#include <WS2tcpip.h>
#include <string>

using namespace std;

int main()
{
    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0) {
        cerr << "Can't initialize winsock! Quiting." << endl;
        return 1;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        cerr << "Can't create a socket! Quitting" << endl;
        return 1;
    }

    // Bind an IP address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // Host to network short
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Bind to any address

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell winsock the socket is for listening
    listen(listening, SOMAXCONN); // Max number of connections is 0x7fffffff

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Clients remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connected on

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    // Get the name of the client if possible
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << " connected on port " << service << endl;
    }

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo massage back to client
    char buf[4096];

    while(true) {
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesRecieved = recv(clientSocket, buf, 4096, 0);
        if (bytesRecieved == SOCKET_ERROR) {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
        if (bytesRecieved == 0) {
            cout << "Client dissconnected " << endl;
            break;
        }

        string message  = buf;

        // If the client sends a 1, do option 1, else just print message and return message
        if (buf[0] == '1') {
            cout << "You have sent a 1" << endl;
            string returnMessage = "Option 1 selected";
            const char* rm = returnMessage.c_str();

            send(clientSocket, rm, strlen(rm), 0);
        }
        else{
            // Echo message back to the client
            send(clientSocket, buf, bytesRecieved, 0);
            cout << message << endl;
        }
    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();
}
