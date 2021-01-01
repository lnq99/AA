package main

import (
	"fmt"
	"image"
	"image/png"
	"os"
	"runtime"
	"sync"
	"time"
)

const (
	NCores  = 3
	NTasks  = 10
	InFile  = "data/in.png"
	OutFile = "data/out.png"
)

type Task struct {
	id     int
	in     string
	out    string
	img    image.Image
	start  []time.Time
	finish []time.Time
}

type Stage func(in, out chan Task)

func createTask(id int, in, out string) Task {
	return Task{id: id, in: in, out: out}
}

func genTask(n int, out chan Task) {
	for i := 0; i < n; i++ {
		out <- createTask(i, InFile, OutFile)
	}
	close(out)
}

func Stage1(in, out chan Task) {
	for task := range in {
		task.start = append(task.start, time.Now())
		task.img = readImage(task.in)
		task.finish = append(task.finish, time.Now())
		out <- task
	}
}

func Stage2(in, out chan Task) {
	for task := range in {
		task.start = append(task.start, time.Now())
		task.img = toGrayscale(task.img)
		task.finish = append(task.finish, time.Now())
		out <- task
	}
}

func Stage3(in, out chan Task) {
	for task := range in {
		task.start = append(task.start, time.Now())
		saveImage(task.img, task.out)
		task.finish = append(task.finish, time.Now())
		out <- task
	}
}

func StageLog(in, _ chan Task) {
	println("Id\tStage1 start\tDuration\tStage2 start\tDuration\tStage3 start\tDuration")
	for task := range in {
		print(task.id)
		for i := range task.start {
			fmt.Printf("\t%s\t%s\t",
				task.start[i].Format("15:04:05.999"),
				task.finish[i].Sub(task.start[i]).Round(10*time.Microsecond))
		}
		println()
	}
}

func mainPipeline(nTasks int, stages []Stage, nNodes []int) {
	wg := &sync.WaitGroup{}
	in := make(chan Task, nTasks)

	genTask(nTasks, in)

	for i := range stages {
		out := make(chan Task, nTasks)
		wg.Add(1)
		if nNodes[i] > 1 {
			go func(st Stage, in, out chan Task) {
				wg1 := &sync.WaitGroup{}
				for j := 0; j < nNodes[i]; j++ {
					wg1.Add(1)
					go func(st Stage, in, out chan Task) {
						defer wg1.Done()
						st(in, out)
					}(st, in, out)
				}
				defer wg.Done()
				defer close(out)
				wg1.Wait()
			}(stages[i], in, out)
		} else {
			go func(st Stage, in, out chan Task) {
				defer wg.Done()
				defer close(out)
				st(in, out)
			}(stages[i], in, out)
		}
		in = out
	}

	wg.Wait()
}

func readImage(path string) image.Image {
	infile, err := os.Open(path)
	if err != nil {
		panic(err.Error())
	}
	defer infile.Close()

	src, _, err := image.Decode(infile)
	if err != nil {
		panic(err.Error())
	}
	return src
}

func toGrayscale(src image.Image) image.Image {
	bounds := src.Bounds()
	w, h := bounds.Max.X, bounds.Max.Y
	gray := image.NewGray(image.Rect(0, 0, w, h))
	for x := 0; x < w; x++ {
		for y := 0; y < h; y++ {
			gray.Set(x, y, src.At(x, y))
		}
	}
	return gray
}

func saveImage(img image.Image, path string) {
	outfile, err := os.Create(path)
	if err != nil {
		panic(err.Error())
	}
	defer outfile.Close()
	png.Encode(outfile, img)
}

func serialWorker(in, out chan Task) {
	for t := range in {
		t.start = append(t.start, time.Now())
		t.img = readImage(t.in)
		t.finish = append(t.finish, time.Now())

		t.start = append(t.start, time.Now())
		t.img = toGrayscale(t.img)
		t.finish = append(t.finish, time.Now())

		t.start = append(t.start, time.Now())
		saveImage(t.img, t.out)
		t.finish = append(t.finish, time.Now())

		out <- t
	}
}

func mainSerial(nTasks int) {
	wg := &sync.WaitGroup{}
	in := make(chan Task, nTasks)
	out := make(chan Task, nTasks)

	genTask(nTasks, in)

	// go StageLog(out, out)

	for i := 0; i < NCores; i++ {
		wg.Add(1)
		go func(in, out chan Task) {
			serialWorker(in, out)
			defer wg.Done()
		}(in, out)
	}

	defer close(out)
	wg.Wait()
}

func measureTime(f func()) time.Duration {
	start := time.Now()
	f()
	return time.Since(start)
}

func Benchmark() {
	runtime.GOMAXPROCS(NCores)
	stages := []Stage{Stage1, Stage2, Stage3}
	nNodes1 := []int{1, 1, 1}
	nNodes2 := []int{NCores, NCores, NCores}

	println("n,t0,t1,t2")
	for n := 10; n <= 100; n += 10 {
		print(n)
		print(",", measureTime(func() {
			mainSerial(n)
		}).Milliseconds())
		print(",", measureTime(func() {
			mainPipeline(n, stages, nNodes1)
		}).Milliseconds())
		print(",", measureTime(func() {
			mainPipeline(n, stages, nNodes2)
		}).Milliseconds())
		println()
	}
}

func main() {
	// disable StageLog when benchmarking
	stages := []Stage{Stage1, Stage2, Stage3, StageLog}
	nNodes := []int{2, 3, 4, 1}

	fmt.Printf("Total time (with log): %s\n", measureTime(func() {
		mainPipeline(NTasks, stages, nNodes)
	}))

	// Benchmark()
}
