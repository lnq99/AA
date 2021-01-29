package main

import (
	"os"
	"testing"
)

type TestResult struct {
	key      string
	expected interface{}
}

var D Dict
var testSuite []TestResult

func setup() {
	D.init(DataPath)
	testSuite = []TestResult{
		{D.data[0].k, D.data[0].v},
		{D.data[999].k, D.data[999].v},
		{D.data[499].k, D.data[499].v},
		{D.data[500].k, D.data[500].v},
		{D.data[101].k, D.data[101].v},
		{D.data[777].k, D.data[777].v},
		{"None", nil},
	}
}

func TestMain(m *testing.M) {
	setup()
	code := m.Run()
	// shutdown()
	os.Exit(code)
}

func TestLinearSearch(t *testing.T) {
	for _, test := range testSuite {
		v := D.linearSearch(test.key)
		if test.expected != v {
			t.Errorf("Expected %v, got %v", test.expected, v)
		}
	}
}

func TestBinarySearch(t *testing.T) {
	for _, test := range testSuite {
		v := D.binarySearch(test.key)
		if test.expected != v {
			t.Errorf("Expected %v, got %v", test.expected, v)
		}
	}
}

func TestHybridSearch(t *testing.T) {
	for _, test := range testSuite {
		v := D.hybridSearch(test.key)
		if test.expected != v {
			t.Errorf("Expected %v, got %v", test.expected, v)
		}
	}
}
