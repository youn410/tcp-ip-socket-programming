#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

void HandleTCPClient(int clientSock) {
  char echoBuffer[RCVBUFSIZE]; /* エコー文字列のバッファ */
  int receiveMessageSize;

  /* クライアントからの受信メッセージ */
  if ((receiveMessageSize = recv(clientSock, echoBuffer, RCVBUFSIZE - 1, 0)) < 0)
    DieWithError("recv() failed");

  /* 受信した文字列を送信し、転送が完了していなければ次を受信する */
  while(receiveMessageSize > 0) {
    /* メッセージをクライアントにエコーバックする */
    if (send(clientSock, echoBuffer, receiveMessageSize, 0) != receiveMessageSize)
      DieWithError("send() failed");

    /* 受信するデータが残っていないか確認する */
    if ((receiveMessageSize = recv(clientSock, echoBuffer, RCVBUFSIZE - 1, 0)) < 0)
      DieWithError("recv() failed");
  }

  close(clientSock);
}
