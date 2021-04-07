#include    "unp.h"
#define MAXCLIENTS 10
int main(int argc, char ** argv){
    int                 listenfd;
    struct sockaddr_in  clients[MAXCLIENTS + 1], srv;// 多一个用作缓冲区
    char *              namelist[MAXCLIENTS];
    socklen_t           clilen;
    char                buff[MAXLINE], recvline[MAXLINE];
    int                 n;
    int                 islogin;
    int                 i, j, k;
    listenfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&srv, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    srv.sin_port = htons(9999);

    Bind(listenfd, (SA*)&srv, sizeof(srv));
    i = 0;
    for( ; ; ) {
        clilen = 16;
        islogin = 0;
        bzero(&clients[i], sizeof(clients[i]));
        n = Recvfrom(listenfd, recvline, MAXLINE, 0, (SA*)&clients[i], &clilen);
        recvline[n] = 0;

        for(k = 0; k < i; k++)//判断是否已经登录
            if(clients[k].sin_addr.s_addr == clients[i].sin_addr.s_addr 
               && clients[k].sin_port == clients[i].sin_port ){
                islogin = 1;
                break;
            }
        if(islogin){
            bzero(buff, sizeof(buff));
            strcat(buff, namelist[k]);
            if(strcmp(recvline, "quit\n") == 0){//是否为退出状态
                strcat(buff, " is quit\n");
                clients[k].sin_addr.s_addr = clients[i-1].sin_addr.s_addr;
                clients[k].sin_port = clients[i-1].sin_port;//集合操作
                free(namelist[k]);
                namelist[k] = namelist[i-1];
                k = i--;
            }else{
                strcat(buff, " say:");
                strcat(buff, recvline);
            }
            fprintf(stdout, "%s", buff);
            for(j = 0; j < i ; j++ )
                if(j != k)
                    Sendto(listenfd, buff, strlen(buff), 0, (SA*)&clients[j], sizeof(clients[j]));
        }else{
            if(i == MAXCLIENTS + 1)//缓冲区被占
                err_quit("too many");
            namelist[i] = Malloc(strlen(recvline));
            strcpy(namelist[i], recvline);
            fprintf(stdout, "%s:%d login => [%s]\n", 
                    Inet_ntop(AF_INET, &clients[i].sin_addr, buff, MAXLINE), 
                    ntohs(clients[i].sin_port), recvline);
            bzero(buff, sizeof(buff));
            strcat(buff, namelist[i]);
            strcat(buff, " log in\n");
            for(j = 0; j < i; j++ )
                Sendto(listenfd, buff, strlen(buff), 0, (SA*)&clients[j], sizeof(clients[j]));
            i++;
        }
    }
}
