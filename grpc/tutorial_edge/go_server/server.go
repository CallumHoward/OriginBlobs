package main

import (
    "log"
    "net"

    "google.golang.org/grpc"
    "github.com/tutorialedge/go-grpc-tutorial/chat"
)

func main() {
    lis, err := net.Listen("tcp", ":9090")
    if err != nil {
        log.Fatalf("Failed to listen on port 9090: %v", err)
    } else {
        log.Print("Listening for gRPC on port 9090")
    }

    s := chat.Server{}

    grpcServer := grpc.NewServer()

    chat.RegisterChatServiceServer(grpcServer, &s)

    if err := grpcServer.Serve(lis); err != nil {
        log.Fatalf("Failed to serve gRPC server over port 9090: %v", err)
    } else {
        log.Print("Serving gRPC on port 9090")
    }

}
