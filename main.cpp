#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>

#define PORT 80

    int main(int argc, char const * argv[])
    {
        int sock, inet, conn;
        int valread;
        struct sockaddr_in address;
        char *hello = "hello i'm from client\0";
        char buffer[10000] = {0};

        sock = socket(AF_INET, SOCK_STREAM, 0); //1.소켓생성
        if(sock < 0) //소켓만들기에 실패한다면
        {
            printf("socket creation error\n");
            return -1;
        }

        address.sin_family = AF_INET;
        address.sin_port = htons(PORT);

        // inet = inet_pton(AF_INET, "10.19.225.197", &address.sin_addr); //IP주소를 binary형식으로 변경
        inet = inet_pton(AF_INET, "127.0.0.1", &address.sin_addr); //IP주소를 binary형식으로 변경
        if(inet <= 0)
        {
            printf("inet_pton error\n");
            return -1;
        }

        conn = connect(sock, (struct sockaddr *)&address, sizeof(address)); //2. server와 연결
        if(conn<0)
        {
            printf("connect error\n");
            return -1;
        }

        fcntl(sock, F_SETFL, O_NONBLOCK);


//3. server와 통신
        char buf[1000];
        std::string msg;
        while (1) {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::fgets(buf, 1000, stdin);
            int i = 0;
            while (buf[i])
            {
                if (buf[i] == '*' && buf[i + 1] == '*'){
                    buf[i] = '\r';
                    buf[i + 1] = '\n';
                }
                i++;
            }
            std::cout << buf << std::endl;
            send(sock, buf, strlen(buf), 0);
            valread = read(sock, buffer, 10000);
            msg = buffer;
            int end = msg.find("*");
            if (end != std::string::npos)
                break;
            std::memset(buf, 0, strlen(buf));
            printf("from server : %s\n", buffer);
        }
        return 0;
    }
