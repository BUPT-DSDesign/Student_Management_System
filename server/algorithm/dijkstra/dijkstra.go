package dijkstra

import "math"

type Edge struct {
	next   int
	to     int
	length float64
}
type Path struct {
	fromId   int
	descId   int
	distance float64
}

// Dijkstra的dis数组
type NodeDis struct {
	nodeId int
	dis    float64
}

// 前向星存图加边
func addEdge(path []Path, head []int) ([]int, []Edge) {
	edges := make([]Edge, len(path)*2+5)
	edgeCnt := 0
	for _, i := range path {
		//无向图加边
		edges[edgeCnt].to = i.descId
		edges[edgeCnt].length = i.distance
		edges[edgeCnt].next = head[i.fromId]
		head[i.fromId] = edgeCnt
		edgeCnt++
		edges[edgeCnt].to = i.fromId
		edges[edgeCnt].length = i.distance
		edges[edgeCnt].next = head[i.descId]
		head[i.descId] = edgeCnt
		edgeCnt++
	}
	return head, edges
}
func Dijkstra(start int, desc int, path []Path, node_cnt int) (pathList []int, err error) {
	//使用前向星存图
	head := make([]int, node_cnt+5)
	dis := make([]float64, node_cnt+5)
	//初始化为-1
	for i, _ := range head {
		head[i] = -1
		dis[i] = math.Inf(1)
	}
	//初始化起点距离为0
	dis[start] = 0
	//获取前向星存图
	head, edges := addEdge(path, head)

	return
}
