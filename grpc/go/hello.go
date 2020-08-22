package main

import (
	"fmt"
	"math/rand"
	"time"
)

func main() {
    rand.Seed(time.Now().UnixNano())
    time.Sleep(100 * time.Millisecond)
    fmt.Println("Hello World! %.3f", rand.NormFloat64())
}
