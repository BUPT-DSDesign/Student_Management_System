package my_sort

// Sorter 定义一个通用的接口
type Sorter interface {
	Len() int
	Less(i, j int) bool
	Swap(i, j int)
	SubSlice(start, end int) Sorter
}

// QuickSort 快速排序算法
func QuickSort(data Sorter) {
	if data.Len() <= 1 {
		return
	}

	pivot := 0 // 选择第一个元素作为基准
	left := 1
	right := data.Len() - 1
	for left <= right {
		// 从左边开始找到第一个大于基准的元素
		for left <= right && !data.Less(pivot, left) {
			left++
		}
		// 从右边开始找到第一个小于基准的元素
		for left <= right && data.Less(pivot, right) {
			right--
		}
		// 如果左边的下标小于右边的下标，交换两个元素
		if left < right {
			data.Swap(left, right)
		}
	}
	// 将基准元素放到中间
	data.Swap(pivot, right)

	// 递归处理左边的元素
	QuickSort(data.SubSlice(0, right))
	// 递归处理右边的元素
	QuickSort(data.SubSlice(right+1, data.Len()))
}
