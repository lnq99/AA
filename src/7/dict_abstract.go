package main

import (
	"fmt"
	"os"
	"sort"
)

// # - symbol, number, ... (< A)
// * - lowercase, ... (> Z)
const (
	Groups = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ*"
	L      = len(Groups)
)

type DictRecord struct {
	k string
	v interface{}
}

type FreqRecord struct {
	char  rune
	count int
	start int
	end   int
}

type Dict struct {
	data []DictRecord
	freq []FreqRecord
}

func (d *Dict) put(records []DictRecord) {
	for _, r := range records {
		d.data = append(d.data, r)
	}
	d.update()
}

func (d *Dict) print() {
	for _, r := range d.data {
		fmt.Printf("%v :\t%v\n", r.k, r.v)
	}
}

func (d *Dict) printFreqTable() {
	for _, r := range d.freq {
		fmt.Printf("%c : %v\t(%v-%v)\n", r.char, r.count, r.start, r.end)
	}
}

func (d *Dict) init(filename string) {
	d.freq = make([]FreqRecord, L)
	d.loadDictFromFile(filename)
	d.update()
}

func (d *Dict) update() {
	sort.Slice(d.data, func(i, j int) bool {
		return d.data[i].k < d.data[j].k
	})

	d.updateFreqTable()

	sort.Slice(d.freq, func(i, j int) bool {
		return d.freq[i].count > d.freq[j].count
	})
}

func (d *Dict) loadDictFromFile(filename string) {
	f, err := os.Open(filename)
	if err != nil {
		panic(err.Error())
	}
	defer f.Close()

	// depending on specific dictionary and file format
	d.readDict(f)
}

func (d *Dict) updateFreqTable() {
	lData := len(d.data)

	for i, c := range Groups {
		d.freq[i].char = c
	}

	start, end := 0, 0
	for end < lData && d.data[end].k[0] < Groups[1] {
		end++
	}
	d.freq[0].setFreqRecord(start, end)
	start = end

	for i := 1; i < L-1; i++ {
		for end < lData && d.data[end].k[0] == Groups[i] {
			end++
		}
		d.freq[i].setFreqRecord(start, end)
		start = end
	}

	d.freq[L-1].setFreqRecord(end, lData)
}

func (r *FreqRecord) setFreqRecord(start, end int) {
	r.start = start
	r.end = end - 1
	r.count = end - start
}

func (d *Dict) linearSearch(key string) interface{} {
	for _, r := range d.data {
		if r.k == key {
			return r.v
		}
	}
	return nil
}

func (d *Dict) binarySearch(key string) interface{} {
	return d._binarySearch(key, 0, len(d.data)-1)
}

func (d *Dict) _binarySearch(key string, start, end int) interface{} {
	if start > end {
		return nil
	}

	mid := (start + end) / 2
	cur := d.data[mid]

	if key < cur.k {
		return d._binarySearch(key, start, mid-1)
	} else if key > cur.k {
		return d._binarySearch(key, mid+1, end)
	} else {
		return cur.v
	}
}

func (d *Dict) hybridSearch(key string) interface{} {
	segment := rune(key[0])
	if segment > 'Z' {
		segment = '*'
	} else if segment < 'A' {
		segment = '#'
	}

	for _, v := range d.freq {
		if segment == v.char {
			return d._binarySearch(key, v.start, v.end)
		}
	}

	return nil
}

func (d *Dict) search(key string, searchFunc func(string) interface{}) {
	var r interface{}
	t := measureTime(func() {
		r = searchFunc(key)
	})

	if r != nil {
		fmt.Println(key, ":\t", r, "\t", t)
	} else {
		fmt.Println("Not found!\t", t)
	}
}
