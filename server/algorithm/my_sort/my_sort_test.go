package my_sort

import "testing"

type IntSlice []int

func (s IntSlice) Len() int {
	return len(s)
}

// 实现Sorter接口
func (s IntSlice) Less(i, j int) bool {
	return s[i] > s[j]
}

// 实现Sorter接口
func (s IntSlice) Swap(i, j int) {
	s[i], s[j] = s[j], s[i]
}

// 实现Sorter接口
func (s IntSlice) SubSlice(start, end int) Sorter {
	return s[start:end]
}

func TestQuickSort(t *testing.T) {
	// 测试用例
	data := []int{3, 4, 5, 1, 12, -1, -3}
	QuickSort(IntSlice(data))
	t.Log(data)
}
