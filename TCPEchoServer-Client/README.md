コンパイル TCPEchoClient
```
gcc TCPEchoClient.c DieWithError.c -o TCPEchoClient
```
コンパイル TCPEchoServer.c
```
gcc TCPEchoServer.c DieWithError.c HandleTCPClient.c -o TCPEchoServer
```

サーバ起動
```
./TCPEchoServer 8080
```
クライアントから接続
```
./TCPEchoClient 127.0.0.1 hoge 8080
```
