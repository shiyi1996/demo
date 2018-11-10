package qsort

import "sync"

type Array []int

func AA() {

}

func Qsort(array Array) {
	qsort(array, 0, len(array)-1)
}

func MultQsort(array Array) {
	var wg sync.WaitGroup
	wg.Add(1)
	multQsort(array, 0, len(array)-1, &wg)
}

func qsort(array Array, left ,right int) {
	if left > right {
		return
	}
	sentry := array[left]
	i,j := left, right
	for i < j {
		for array[j] > sentry && i < j {
			j--
		}
		for array[i] < sentry && i < j {
			i++
		}
		if i < j {
			temp := array[i]
			array[i] = array[j]
			array[j] = temp
		}
	}
	array[i] = sentry
	qsort(array, left, i-1)
	qsort(array, i+1, right)
}

func multQsort(array Array, left ,right int, wg *sync.WaitGroup) {
	defer wg.Done()
	if left > right {
		return
	}
	sentry := array[left]
	i,j := left, right
	for i < j {
		for array[j] > sentry && i < j {
			j--
		}
		for array[i] < sentry && i < j {
			i++
		}
		if i < j {
			temp := array[i]
			array[i] = array[j]
			array[j] = temp
		}
	}
	array[i] = sentry
	wg.Add(2)
	qsort(array, left, i-1)
	qsort(array, i+1, right)
}
