package dijkstra

import (
	"errors"
	"math"
	"server/algorithm/heap"
	"server/model/entity/system"
)

// Edge 前向星存图的边信息
type Edge struct {
	next   int
	to     int
	length float64
}

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

// 前向星存图加边
func addEdge(path []system.Path, head []int) ([]int, []Edge) {
	edges := make([]Edge, len(path)*2+5)
	edgeCnt := 0
	for _, i := range path {
		//无向图加边
		edges[edgeCnt].to = i.DescId
		edges[edgeCnt].length = i.Distance
		edges[edgeCnt].next = head[i.FromId]
		head[i.FromId] = edgeCnt
		edgeCnt++
		edges[edgeCnt].to = i.FromId
		edges[edgeCnt].length = i.Distance
		edges[edgeCnt].next = head[i.DescId]
		head[i.DescId] = edgeCnt
		edgeCnt++
	}
	return head, edges
}
func Dijkstra(start int, desc int, path []system.Path, nodeCnt int) (nodeList []int, distance float64, err error) {
	// 判断请求是否合法
	if start < 0 || desc < 0 || start >= nodeCnt || desc >= nodeCnt {
		return nil, 0.0, errors.New("请求节点id错误")
	}
	//使用前向星存图
	head := make([]int, nodeCnt+5)
	//from数组记录每个节点是从哪个节点过来的
	from := make([]int, nodeCnt+5)
	dis := make([]float64, nodeCnt+5)
	//初始化为-1
	for i := range head {
		from[i] = -1
		head[i] = -1
		dis[i] = math.Inf(1)
	}
	//初始化起点距离为0
	dis[start] = 0
	//获取前向星存图
	head, edges := addEdge(path, head)
	pq := make(PriorityQueue, len(dis))
	//将第一个元素推入
	pq.Push(NodeDis{nodeId: start, dis: 0})
	//新建堆
	heap.Init(&pq)
	for pq.Len() != 0 {
		node := heap.Pop(&pq).(NodeDis)
		id := node.nodeId
		for i := head[id]; i != -1; i = edges[i].next {
			e := edges[i]
			//Dijkstra算法核心:路径松弛
			if dis[e.to] > dis[id]+node.dis {
				dis[e.to] = dis[id] + node.dis
				heap.Push(&pq, NodeDis{nodeId: e.to, dis: e.length})
				from[e.to] = id
			}
		}
	}
	//最后，从终点回头输出路径
	if dis[desc] == math.Inf(1) {
		return nil, 0.0, errors.New("没有这样的路径")
	}
	nodeList = append(nodeList, desc)

	for i := from[desc]; i != -1; i = from[i] {
		nodeList = append(nodeList, i)
	}

	//将nodeList倒置
	for i, j := 0, len(nodeList)-1; i < j; i, j = i+1, j-1 {
		nodeList[i], nodeList[j] = nodeList[j], nodeList[i]
	}

	return nodeList, dis[desc], nil
}
