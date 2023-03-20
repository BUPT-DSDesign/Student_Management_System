package tsp

import (
	"server/model/entity/system"
)

// Edge 前向星存图的边信息
type Edge struct {
	next   int
	to     int
	length float64
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

func TSP(startId int, passIds []int, paths []system.Path, nodeCnt int) (nodeList []int, err error) {
	// 使用前向星存图
	head := make([]int, nodeCnt+5)

	//初始化为-1
	for i := range head {
		head[i] = -1
	}

	//获取前向星存图
	head, edges := addEdge(paths, head)
	println(edges)

	return append(passIds, startId), nil
}
