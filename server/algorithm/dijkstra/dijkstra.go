package dijkstra

import (
	"errors"
	"math"
	"server/algorithm/heap"
	"server/algorithm/initialize"
)

// NodeDis Dijkstra的dis数组
type NodeDis struct {
	nodeId int
	dis    float64
}

// PriorityQueue Dijkstra的dis数组优先队列
type PriorityQueue []NodeDis

//以下开始绑定heap.go中的方法

func (pq PriorityQueue) Len() int {
	return len(pq)
}
func (pq PriorityQueue) Cmp(i int, j int) bool {
	return pq[i].dis < pq[j].dis
}
func (pq PriorityQueue) Swap(i int, j int) {
	pq[i], pq[j] = pq[j], pq[i]
}
func (pq *PriorityQueue) Push(x interface{}) {
	*pq = append(*pq, x.(NodeDis))
}
func (pq *PriorityQueue) Pop() interface{} {
	old := *pq
	n := len(old)
	item := old[n-1]
	*pq = old[0 : n-1]
	return item
}

// Dijkstra 最短路算法
func Dijkstra(start int, desc int, nodeCnt int) (nodeList []int, distance float64, err error) {
	// from数组记录每个节点是从哪个节点过来的
	from := make([]int, nodeCnt+5)
	dis := make([]float64, nodeCnt+5)
	//初始化为-1
	for i := range from {
		from[i] = -1
		dis[i] = math.Inf(1)
	}
	//初始化起点距离为0
	dis[start] = 0

	// 创建一个优先队列
	pq := make(PriorityQueue, len(dis))
	//将第一个元素推入
	pq.Push(NodeDis{nodeId: start, dis: 0})
	//新建堆
	heap.Init(&pq)
	for pq.Len() != 0 {
		node := heap.Pop(&pq).(NodeDis)
		id := node.nodeId
		for i := initialize.Head[id]; i != -1; i = initialize.Edges[i].Next {
			e := initialize.Edges[i]
			//Dijkstra算法核心:路径松弛
			if dis[e.To] > dis[id]+node.dis {
				dis[e.To] = dis[id] + node.dis
				heap.Push(&pq, NodeDis{nodeId: e.To, dis: e.Length})
				from[e.To] = id
			}
		}
	}

	// 最后，从终点回头输出路径
	if dis[desc] == math.Inf(1) {
		return nil, 0.0, errors.New("没有这样的路径")
	}
	nodeList = append(nodeList, desc)

	for i := from[desc]; i != -1; i = from[i] {
		nodeList = append(nodeList, i)
	}

	// 将nodeList倒置
	for i, j := 0, len(nodeList)-1; i < j; i, j = i+1, j-1 {
		nodeList[i], nodeList[j] = nodeList[j], nodeList[i]
	}

	return nodeList, dis[desc], nil
}
