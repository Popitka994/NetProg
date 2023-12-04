#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // Создание сокета
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Настройка адреса сервера
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(13);  // Порт 13 для службы daytime

    // Преобразование IP-адреса из строки в структуру
    if (inet_pton(AF_INET, "172.16.40.1", &(serverAddress.sin_addr)) <= 0) {
        perror("Error converting IP address");
        close(clientSocket);
        return -1;
    }

    // Отправка запроса на сервер
    const char* requestMessage = "What's the time?";
    if (sendto(clientSocket, requestMessage, strlen(requestMessage), 0,
               (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error sending request");
        close(clientSocket);
        return -1;
    }

    // Получение ответа от сервера
    char responseMessage[256];
    socklen_t serverAddressLen = sizeof(serverAddress);
    ssize_t bytesRead = recvfrom(clientSocket, responseMessage, sizeof(responseMessage), 0,
                                  (struct sockaddr*)&serverAddress, &serverAddressLen);
    if (bytesRead == -1) {
        perror("Error receiving response");
        close(clientSocket);
        return -1;
    }

    // Вывод ответа
    responseMessage[bytesRead] = '\0';  // Добавляем нулевой символ, чтобы преобразовать в строку
    std::cout << "Response from server: " << responseMessage << std::endl;

    // Закрытие сокета
    close(clientSocket);

    return 0;
}
