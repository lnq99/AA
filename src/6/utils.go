package main

import (
	"fmt"
	"math/rand"
	"strings"
	"time"
)

const (
	MaxEdgeLen = 9
	NDays      = 50
)

func genGraph(n int) [][]int {
	graph := make([][]int, n)
	for i := 0; i < n; i++ {
		graph[i] = make([]int, n)
		for j := 0; j < n; j++ {
			if i != j {
				graph[i][j] = rand.Intn(MaxEdgeLen) + 1
			}
		}
	}
	return graph
}

func printGraph(g [][]int) {
	for i := range g {
		fmt.Println(g[i])
	}
	println()
}

func measureTime(f func()) time.Duration {
	start := time.Now()
	f()
	return time.Since(start)
}

func printResult(cost int, route []int) {

	fmt.Printf("%4d|%v\n", cost, route)
}

func logOutput(n int) {
	g := genGraph(n)
	env := createEnv(g)
	printGraph(g)

	println("BRUTE")
	cost, route := tspBrute(env.g)
	fmt.Printf("%d\t%v\n", cost, route)
	println("==========================\n")

	fmt.Printf("ACO\tn ants * %d days\n", NDays)
	println("\t{alpha, beta, rho, tau0, taumin, q}\n")

	env.a = 1
	env.b = 0
	env.rho = 0.1
	for i := 0; i < 6; i++ {
		for j := 0; j < 3; j++ {
			env.rho += 0.3
			initPheromone(env.ph)
			cost, route = tspACO(env, NDays)
			fmt.Printf("%d\t%v\t", cost, route)
			fmt.Printf("{%.1f, %.1f, %.1f, %.1f, %.1f, %.1f}\n",
				env.a, env.b, env.rho, Tau0, TauMin, Q)
		}
		env.a -= 0.2
		env.b += 0.2
	}
}

func Benchmark(nList []int) {
	aco := []time.Duration{}
	brute := []time.Duration{}
	for _, n := range nList {
		g := genGraph(n)
		env := createEnv(g)

		aco = append(aco, measureTime(func() {
			tspACO(env, 100)
		}))

		brute = append(brute, measureTime(func() {
			tspBrute(g)
		}))
	}

	outBenchmark(nList, aco, brute)
}

func outBenchmark(nList []int, a []time.Duration, b []time.Duration) {
	fmt.Printf(" N|%13v|%13v\n", "ACO", "BRUTE")
	fmt.Printf("%30v\n", strings.Repeat("-", 30))
	for i, v := range nList {
		fmt.Printf("%2v|%13v|%13v\n", v, a[i], b[i])
	}
	fmt.Printf("%30v\n\n", strings.Repeat("-", 30))

	// to CSV format
	println("n,aco,brute")
	for i, v := range nList {
		fmt.Printf("%d,%d,%d\n", v, a[i].Round(time.Microsecond).Microseconds(), b[i].Round(time.Microsecond).Microseconds())
	}
}
