#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 255

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoServerAddress;
  struct sockaddr_in echoClientAddress;
  unsigned int clientAddressLength;
  char echoBuffer[ECHOMAX];
  unsigned short echoServerPort;
  int receiveMessageSize;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
    exit(1);
  }

  echoServerPort = atoi(argv[1]);

  /* データグラムの送受信に使うソケットを作成 */
  if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    DieWithError("socket() failed");

  /* ローカルのアドレス構造体を作成 */
  memset(&echoServerAddress, 0, sizeof(echoServerAddress));
  echoServerAddress.sin_family = AF_INET;
  echoServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  echoServerAddress.sin_port = htons(echoServerPort);

  /* ローカルアドレスへバインド */
  if (bind(sock, (struct sockaddr *) &echoServerAddress, sizeof(echoServerAddress)) < 0)
    DieWithError("bind() failed");

  for(;;) {
    clientAddressLength = sizeof(echoClientAddress);

    /* クライアントからのメッセージを受診するまでブロック */
    if ((receiveMessageSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClientAddress, &clientAddressLength)) < 0)
      DieWithError("recvfrom() failed");

    printf("Handling client %s\n", inet_ntoa(echoClientAddress.sin_addr));

    /* 受信したデータグラムをクライアントに返信する */
    if (sendto(sock, echoBuffer, receiveMessageSize, 0, (struct sockaddr *) &echoClientAddress, sizeof(echoClientAddress)) != receiveMessageSize)
      DieWithError("sendto() sent a differenct number of bytes than expected");
  }
}
