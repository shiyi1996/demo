package qsort

import (
	"testing"
	"math/rand"
	"time"
)

func TestQsort(t *testing.T) {
	tests := []struct {
		name string
		input Array
		expect Array
	}{
		{
			"正序",
			[]int{1,2,3,4,5},
			[]int{1,2,3,4,5},
		},
		{
			"倒序",
			[]int{5,4,3,2,1},
			[]int{1,2,3,4,5},
		},
		{
			"打乱",
			[]int{3,2,5,4,1},
			[]int{1,2,3,4,5},
		},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			Qsort(test.input)
			if equal := intSliceEqual(test.input, test.expect); !equal {
				t.Errorf("Qsort() = %v, expect = %v", test.input, test.expect)
			}
		})
	}
}

func TestMultQsort(t *testing.T) {
	tests := []struct {
		name string
		input Array
		expect Array
	}{
		{
			"正序",
			[]int{1,2,3,4,5},
			[]int{1,2,3,4,5},
		},
		{
			"倒序",
			[]int{5,4,3,2,1},
			[]int{1,2,3,4,5},
		},
		{
			"打乱",
			[]int{3,2,5,4,1},
			[]int{1,2,3,4,5},
		},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			MultQsort(test.input)
			if equal := intSliceEqual(test.input, test.expect); !equal {
				t.Errorf("MultQsort() = %v, expect = %v", test.input, test.expect)
			}
		})
	}
}

func TestRandomQsort(t *testing.T) {
	seed := time.Now().UTC().UnixNano()
	rng := rand.New(rand.NewSource(seed))
	for i:=0; i<100; i++ {
		array := randomArray(rng, 100, 100000000)
		input := make(Array, 0)
		input = append(input, array...)
		Qsort(input)
		if !isOrder(input) {
			t.Errorf("Qsort()\ninput = %v\nans = %v\n", array, input)
		}
	}
}

func TestRandomMultQsort(t *testing.T) {
	seed := time.Now().UTC().UnixNano()
	rng := rand.New(rand.NewSource(seed))
	for i:=0; i<100; i++ {
		array := randomArray(rng, 100, 100000000)
		input := make(Array, 0)
		input = append(input, array...)
		MultQsort(input)
		if !isOrder(input) {
			t.Errorf("MultQsort()\ninput = %v\nans = %v\n", array, input)
		}
	}
}

func randomArray(rng *rand.Rand, maxLen int, maxItem int) Array {
	n := rng.Intn(maxLen)
	array := make([]int, n)
	for i :=0;i<n;i++{
		array[i]=rng.Intn(maxItem)
	}
	return array
}

func isOrder(array Array) bool {
	for i:=1; i<len(array); i++ {
		if array[i] < array[i-1] {
			return false
		}
	}
	return true
}

func intSliceEqual(a, b []int) bool {
	if len(a) != len(b) {
		return false
	}

	if (a == nil) != (b == nil) {
		return false
	}

	for i, v := range a {
		if v != b[i] {
			return false
		}
	}

	return true
}