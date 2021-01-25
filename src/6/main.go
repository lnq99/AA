package main

import (
	"math/rand"
	"time"
)

func main() {
	rand.Seed(int64(time.Now().Nanosecond()))
	// logOutput(10)
	nList := []int{2, 3, 4, 5, 6, 7, 8, 9, 10}
	Benchmark(nList)
}
