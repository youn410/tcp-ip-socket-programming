#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

int main(int argc, char * argv[]) {
  int sock;
  struct sockaddr_in echoServerAddress;
  char *serverIP;
  unsigned short serverPort;
  char *echoString;
  char echoBuffer[RCVBUFSIZE];
  unsigned int echoStringLength;
  int bytesReceived, totalBytesReceived;

  if ((argc < 3) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
    exit(1);
  }

  serverIP = argv[1];
  echoString = argv[2];

  if (argc == 4)
    serverPort = atoi(argv[3]);
  else
    serverPort = 8080;
  
  /**
   * int socket(int protocolFamily, int type, int protocol)
   * 
   * PF_INET: TCP/IPプロトコルファミリ
   *
   * type: ソケットの種類
   *  SOCK_STREAM: 信頼性の高いストリームソケット
   *  SOCK_DGRAM: ベストエフォート型のデータグラムソケット
   *
   * protocol: エンドツーエンドプロトコル
   *  IPPROTO_TCP: TCP
   *  IPPROTO_UDP: UDP
   */
  /* TCPによる信頼性の高いストリームソケットを作成 */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  /* サーバのアドレス構造体を作成 */
  memset(&echoServerAddress, 0, sizeof(echoServerAddress)); // 余分なバイト部分を0で埋めるため
  echoServerAddress.sin_family = AF_INET; // TCP/IP
  echoServerAddress.sin_addr.s_addr = inet_addr(serverIP);
  echoServerAddress.sin_port = htons(serverPort);

  /* エコーサーバへの接続を確立 */
  if (connect(sock, (struct sockaddr *) &echoServerAddress, sizeof(echoServerAddress)) < 0)
    DieWithError("connect() failed");

  echoStringLength = strlen(echoString);

  /* 文字列をエラーサーバに送信  */
  if (send(sock, echoString, echoStringLength, 0) != echoStringLength)
    DieWithError("send() sent a differenct number of bytes than expected");

  /* 同じ文字列をサーバから受信  */
  totalBytesReceived = 0;
  printf("Recieved: ");

  while (totalBytesReceived < echoStringLength) {
    /* バッファサイズに到達するまで（NULL文字分の1byteを除く）サーバからデータを受信する  */
    if ((bytesReceived = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
      DieWithError("recv() failed or connection closed prematurely");
    totalBytesReceived += bytesReceived;
    echoBuffer[bytesReceived] = '\0';
    printf("%s", echoBuffer);
  }

  printf("\n");

  close(sock);
  exit(0);
}
