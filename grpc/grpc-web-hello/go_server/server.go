package main

import (
  "log"
  "net"

  "golang.org/x/net/context"
  "google.golang.org/grpc"
  "./helloworld"
)

type Server struct {}

func (s *Server) SayHello(ctx context.Context, message *HelloRequest) (*HelloReply, error) {
  log.Printf("Received message body from client: %s", message.Body)
  return &HelloReply{Body: "Hello from the Server!"}, nil
}

func main() {
  lis, err := net.Listen("tcp", ":9090")
  if err != nil {
    log.Fatalf("Failed to listen on port 9090: %v", err)
  }

  grpcServer := grpc.NewServer()

  if err := grpcServer.Serve(lis); err != nil {
    log.Fatalf("Failed to serve gRPC server over port 9090: %v", err)
  }
}
