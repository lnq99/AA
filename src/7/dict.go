package main

import (
	"encoding/csv"
	"os"
	"strconv"
)

const (
	DataPath = "data/data.csv"
)

type Cryptocurrency struct {
	// name   string (key)
	rank      int
	symbol    string
	marketcap int64
	price     float32
}

func (d *Dict) readDictRecord(r []string) {
	rank, _ := strconv.ParseInt(r[0], 10, 32)
	marketcap, _ := strconv.ParseInt(r[3], 10, 64)
	price, _ := strconv.ParseFloat(r[4], 32)
	d.data = append(d.data, DictRecord{
		r[1],
		Cryptocurrency{
			int(rank),
			r[2],
			marketcap,
			float32(price),
		},
	})
}

func (d *Dict) readDict(f *os.File) {
	records, err := csv.NewReader(f).ReadAll()
	if err != nil {
		panic(err.Error())
	}

	for _, r := range records[1:] {
		d.readDictRecord(r)
	}
}
