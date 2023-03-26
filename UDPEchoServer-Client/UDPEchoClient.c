#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 255 /* エコー文字列の最大長 */
#define DEFAULT_ECHO_SERVER_PORT 8081 /* デフォルトのエコーサーバのポート */

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
  int sock; /* ソケットディスクリプタ */
  struct sockaddr_in echoServerAddress;
  struct sockaddr_in fromAddress; /* エコー送信元アドレス */
  unsigned short echoServerPort;
  unsigned int fromAddressSize;
  char *serverIP; /* サーバのIPアドレス xxx.xxx.xxx.xxx */
  char *echoString;
  char echoBuffer[ECHOMAX+1]; /* エコー文字列の受信用バッファ(終端文字が入る) */
  int echoStringLength;
  int responseStringLength;

  if ((argc < 3) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
    exit(1);
  }

  serverIP = argv[1];
  echoString = argv[2];

  if ((echoStringLength = strlen(echoString)) > ECHOMAX)
    DieWithError("Echo word too long");

  if (argc == 4)
    echoServerPort = atoi(argv[3]);
  else
    echoServerPort = DEFAULT_ECHO_SERVER_PORT;

  /* UDPデータグラムソケットの作成 */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    DieWithError("socket() failed");

  /* サーバのアドレス構造体の作成 */
  memset(&echoServerAddress, 0, sizeof(echoServerAddress)); /* 余分なバイト部分を0で埋める */
  echoServerAddress.sin_family = AF_INET;
  echoServerAddress.sin_addr.s_addr = inet_addr(serverIP);
  echoServerAddress.sin_port = htons(echoServerPort);

  /* TCPと違いconnectしない */

  /* 文字列をサーバに送信する */
  if (sendto(sock, echoString, echoStringLength, 0, (struct sockaddr *) &echoServerAddress, sizeof(echoServerAddress)) != echoStringLength)
    DieWithError("sendto() sent a differenct number of bytes than expected");

  /* 応答を受信 */
  fromAddressSize = sizeof(fromAddress);
  if ((responseStringLength = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddress, &fromAddressSize)) != echoStringLength)
    DieWithError("recvfrom() failed");

  if (echoServerAddress.sin_addr.s_addr != fromAddress.sin_addr.s_addr){
    fprintf(stderr, "Error: received a packet from unknown source.\n");
    exit(1);
  }

  /* 受信データをNULL文字で終端させる */
  echoBuffer[responseStringLength] = '\0';
  printf("Received: %s\n", echoBuffer);

  close(sock);
  exit(0);
}
