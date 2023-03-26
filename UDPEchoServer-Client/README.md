コンパイル UDPEchoClient
```
gcc UDPEchoClient.c DieWithError.c -o UDPEchoClient
```
コンパイル UDPEchoServer.c
```
gcc UDPEchoServer.c DieWithError.c HandleTCPClient.c -o UDPEchoServer
```

サーバ起動
```
./UDPEchoServer 8081
```
クライアントから接続
```
./TCPEchoClient 127.0.0.1 hoge
```
