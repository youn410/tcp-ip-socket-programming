#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING 5 /* 同時にキュー可能な接続要求の最大数 */

void DieWithError(char *errorMessage);
void HandleTCPClient(int clientSocket);

int main(int argc, char *argv[]) {
  int serverSock; /* サーバのソケットディスクリプタ */
  int clientSock; /* クライアントとの接続に使われるソケットディスクリプタ */
  unsigned short echoServerPort; /* サーバポート */
  struct sockaddr_in echoServerAddress; /* ローカルアドレス */
  struct sockaddr_in echoClientAddress; /* クライアントアドレス */
  unsigned int clientLen; /* クライアントのアドレス構造体の長さ */

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
    exit(1);
  }

  echoServerPort = atoi(argv[1]);

  /* 着信接続用のソケットを作成 */
  if ((serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  /* ローカルのアドレス構造体を作成 */
  memset(&echoServerAddress, 0, sizeof(echoServerAddress));
  echoServerAddress.sin_family = AF_INET;
  echoServerAddress.sin_addr.s_addr = htonl(INADDR_ANY); /* ワイルドカードを使用 */
  echoServerAddress.sin_port = htons(echoServerPort);

  /* ローカルアドレスへバインド */
  if (bind(serverSock, (struct sockaddr *) &echoServerAddress, sizeof(echoServerAddress)) < 0)
    DieWithError("bind() failed");

  /* 「接続要求をリッスン中」というマークをソケットにつける */
  if (listen(serverSock, MAXPENDING) < 0)
    DieWithError("listen() failed");

  for (;;) {
    clientLen = sizeof(echoClientAddress);

    /* クライアントからの接続要求を待機 */
    if ((clientSock = accept(serverSock, (struct sockaddr *) &echoClientAddress, &clientLen)) < 0)
      DieWithError("accept() failed");

    /* clientSockはクライアントに接続済み */

    printf("Handling client %s\n", inet_ntoa(echoClientAddress.sin_addr));

    HandleTCPClient(clientSock);
  }
}
