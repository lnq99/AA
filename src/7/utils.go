package main

import (
	"fmt"
	"time"
)

const (
	NLOOP = 10
)

func measureTime(f func()) time.Duration {
	start := time.Now()
	f()
	return time.Since(start)
}

func Benchmark(d Dict) {
	searchFunc := [](func(string) interface{}){
		d.linearSearch,
		d.binarySearch,
		d.hybridSearch,
	}

	times := [][]time.Duration{
		[]time.Duration{0, 0, 0},
		[]time.Duration{0, 0, 0},
		[]time.Duration{0, 0, 0},
	}

	last := len(d.data) - 1

	bestKeys := []string{
		d.data[0].k,
		d.data[last/2].k,
		d.data[last/2].k,
	}

	worstKeys := []string{
		d.data[last].k,
		d.data[last].k,
		d.data[d.freq[2].end].k, // 3 linear + 7 binary (log_2(70) > 6)
	}

	for i, f := range searchFunc {
		times[i][0] = measureTime(func() {
			for _, r := range d.data {
				for k := 0; k < NLOOP; k++ {
					f(r.k)
				}
			}
		})
		times[i][1] = measureTime(func() {
			for j := 0; j <= last; j++ {
				for k := 0; k < NLOOP; k++ {
					f(bestKeys[i])
				}
			}
		})
		times[i][2] = measureTime(func() {
			for j := 0; j <= last; j++ {
				for k := 0; k < NLOOP; k++ {
					f(worstKeys[i])
				}
			}
		})
	}

	println("\n#### BENCHMARK ####")
	println("\tAverage\t\tBest\t\tWorst")
	for i, f := range []string{"Linear", "Binary", "Hybrid"} {
		fmt.Printf("%v\t%v\t%v\t%v\n", f, times[i][0], times[i][1], times[i][2])
	}

	// to CSV format
	println("\nAverage,Best,Worst")
	for _, t := range times {
		fmt.Printf("%d,%d,%d\n",
			t[0].Round(time.Microsecond).Microseconds(),
			t[1].Round(time.Microsecond).Microseconds(),
			t[2].Round(time.Microsecond).Microseconds())
	}
}
