#include    "unp.h"
void Str_cli(FILE *fp, int socketfd, SA* serv){
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
    int n;

    Connect(socketfd, serv, sizeof(*serv));
    FD_ZERO(&rset);
    for( ; ; ){
        FD_SET(fileno(fp), &rset);
        FD_SET(socketfd, &rset);
        maxfdp1 = max(fileno(fp), socketfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);//可读可写
        if(FD_ISSET(socketfd, &rset)){
            if( (n = Read(socketfd, recvline, MAXLINE) ) == 0)
                err_quit("server die");
            recvline[n] = 0;
            fprintf(stdout, "%s", recvline);
        }
         if(FD_ISSET(fileno(fp), &rset)){
            if(Fgets(sendline, MAXLINE, fp) == NULL)
                return;
            Write(socketfd, sendline, strlen(sendline));
            if(strcmp(sendline, "quit\n") == 0)
                return;
        } 
    }
}
int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in serv;
    if(argc != 3)
        err_quit("two arg need");

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv, sizeof(serv));
    
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[1]));
    Inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);

    Sendto(sockfd, argv[2], strlen(argv[2]), 0, (SA*)&serv, sizeof(serv));
    Str_cli(stdin, sockfd, (SA*)&serv);
}
