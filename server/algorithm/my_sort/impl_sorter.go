package my_sort

import (
	"server/model/entity/system"
	"server/utils"
)

// 实现活动按照时间排序
// 1. 实现Sorter接口
// 2. 实现Len() int方法
// 3. 实现Less(i, j int) bool方法
// 4. 实现Swap(i, j int)方法
// 5. 实现SubSlice(start, end int) Sorter方法

type ActivitySlice []*system.ActivityInfo

func (a ActivitySlice) Len() int {
	return len(a)
}

func (a ActivitySlice) Less(i, j int) bool {
	return utils.IsLessThan(a[i].StartTime, a[j].StartTime)
}

func (a ActivitySlice) Swap(i, j int) {
	a[i], a[j] = a[j], a[i]
}

func (a ActivitySlice) SubSlice(start, end int) Sorter {
	return a[start:end]
}
