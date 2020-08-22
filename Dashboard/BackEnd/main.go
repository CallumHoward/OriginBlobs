package main

import (
    "fmt"
    "log"
    "net/http"
    socketio "github.com/googollee/go-socket.io"
 )

func main() {
    fmt.Println("Hello World")

    server, err := socketio.NewServer(nil)
    if err != nil {
        log.Fatal(err)
    }

    server.OnConnect("/", func(so socketio.Conn) error {
        fmt.Println("New Connection:", so.ID())
        return nil
    })

    fs := http.FileServer(http.Dir("static"))
    http.Handle("/", fs)

    http.Handle("/socket.io/", server)
    log.Fatal(http.ListenAndServe(":5000", nil))
}
