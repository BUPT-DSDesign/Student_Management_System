package heap

type Interface interface {
	// Len 查询元素数量
	Len() int
	// Cmp 比较函数,优先度正确返回True
	Cmp(i, j any) bool
	// Swap 元素交换
	Swap(i, j any)
	// Push 将元素压入堆
	Push(x interface{})
	// Pop 弹出堆顶元素
	Pop() interface{}
}
