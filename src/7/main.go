package main

import (
	"bufio"
	"os"
	"strings"
)

func info() {
	println("Cryptocurrencies")
	println("Search by name")
	println("{ Rank, Symbol, Market Cap($), Price($) }")
}

func menu(d Dict) {
	info()
	reader := bufio.NewReader(os.Stdin)
	run := true

	for run {
		print("\nSearch: ")
		key, _ := reader.ReadString('\n')

		if len(key) > 1 {
			key = strings.TrimRight(key, "\n")
			searchAllMethod(d, key)
		} else {
			run = false
		}
	}
}

func searchAllMethod(d Dict, key string) {
	print("Linear\t")
	d.search(key, d.linearSearch)
	print("Binary\t")
	d.search(key, d.binarySearch)
	print("Hybrid\t")
	d.search(key, d.hybridSearch)
}

func test(d Dict) {
	keys := []string{
		"Bitcoin",
		"Ethereum",
		"Chainlink",
		"0Chain",
		"key",
	}

	info()
	for _, k := range keys {
		println("\nSearch:", k)
		searchAllMethod(d, k)
	}
}

func main() {
	var D Dict
	D.init(DataPath)
	// D.print()
	// D.printFreqTable()

	// menu(D)
	test(D)
	Benchmark(D)
}
