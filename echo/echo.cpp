#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(7);  // Порт сервера
    serverAddress.sin_addr.s_addr = inet_addr("172.16.40.1");  // IP-адрес сервера

    // Установка соединения с сервером
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Ошибка при установлении соединения" << endl;
        return 1;
    }

    cout << "Подключение к серверу..." << endl;

    cout << "Введите сообщение: ";
    fgets(buffer, BUFFER_SIZE, stdin);
    send(clientSocket, buffer, strlen(buffer), 0);

    // Получение ответа от сервера
    memset(buffer, 0, BUFFER_SIZE);
    recv(clientSocket, buffer, BUFFER_SIZE, 0);

    cout << "Ответ от сервера: " << buffer;
    close(clientSocket);

    return 0;
}
