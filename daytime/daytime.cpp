#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0); //Создания сокетв
    if (clientSocket == -1) {
        perror("Ошибка создания сокета");
        return -1;
    }
    
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(13);
    
    if (inet_pton(AF_INET, "172.16.40.1", &(serverAddress.sin_addr)) <= 0) {
        perror("Ошибка преобразования IP-сервера");
        close(clientSocket);
        return -1;
    }

    const char* requestMessage = "Сколько время?";
    if (sendto(clientSocket, requestMessage, strlen(requestMessage), 0,
               (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Ошибка отправик запроса");
        close(clientSocket);
        return -1;
    }

    char responseMessage[256];
    socklen_t serverAddressLen = sizeof(serverAddress);
    ssize_t bytesRead = recvfrom(clientSocket, responseMessage, sizeof(responseMessage), 0,
                                  (struct sockaddr*)&serverAddress, &serverAddressLen);
    if (bytesRead == -1) {
        perror("Ошибка получения ответа");
        close(clientSocket);
        return -1;
    }

    responseMessage[bytesRead] = '\0';  // Добавляем нулевой символ, чтобы преобразовать в строку
    cout << "Response from server: " << responseMessage << endl;

    close(clientSocket);

    return 0;
}
