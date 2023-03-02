package heap

type Interface interface {
	// Len 查询元素数量
	Len() int
	// Cmp 比较函数,若返回True,i会在j的顶上
	Cmp(i, j int) bool
	// Swap 元素交换
	Swap(i, j int)
	// Push 将元素压入堆
	Push(x interface{})
	// Pop 弹出堆顶元素
	Pop() interface{}
}

// Init 堆的初始化函数
func Init(h Interface) {
	n := h.Len() //获取数据的长度
	for i := n/2 - 1; i >= 0; i-- {
		down(h, i, n)
	}
}

// 向下调整,返回值表示是否有调整
func down(h Interface, elem, n int) bool {
	i := elem //临时变量
	for {     //死循环,不断向下调
		left := 2*i + 1            //由于首元素为0，则左孩子是2*i+1
		if left >= n || left < 0 { //下标越界
			break
		}
		j := left                                                //记录根节点应该保存的值
		if right := left + 1; right < n && !h.Cmp(left, right) { //如果right没越界且right应该在顶上
			j = right
		}
		if h.Cmp(i, j) { // 如果i就应该是他们俩的父亲
			break
		}
		h.Swap(i, j)
		i = j //继续调整,保证子树也是堆结构
	}
	return i > elem
}

// 向上调整
func up(h Interface, j int) {
	for {
		i := (j - 1) / 2           //j的父亲
		if i == j || h.Cmp(i, j) { //如果i确实应该当j的父亲或j没父亲了
			break
		}
		h.Swap(i, j)
		j = i //继续找父亲
	}
}

// Push 压入数据
func Push(h Interface, x interface{}) {
	h.Push(x)        //将新插入的节点放到最后
	up(h, h.Len()-1) //向上调整堆
}

// Pop 弹出堆顶数据
func Pop(h Interface) interface{} {
	n := h.Len() - 1
	h.Swap(0, n)  //将最后一个节点和第一个节点交换
	down(h, 0, n) //从顶开始将元素向下调整
	return h.Pop()
}

// Remove 删除某个节点数据
func Remove(h Interface, i int) interface{} {
	n := h.Len() - 1
	if n != i { // 同样调到队尾后删除
		h.Swap(i, n)
		down(h, i, n)
		up(h, i)
	}
	return h.Pop()
}

// Fix 保持当前节点的位置正确
func Fix(h Interface, i int) {
	if !down(h, i, h.Len()) {
		//如果有向下交换,则向上交换,否则不需要操作
		up(h, i)
	}
}
