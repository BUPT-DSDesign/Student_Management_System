package initialize

import (
	"server/model/entity/system"
)

// Edge 前向星存图的边信息
type Edge struct {
	Next   int
	To     int
	Length float64
}

// 前向星存图加边
func addEdge(path []system.Path, head []int) ([]int, []Edge) {
	edges := make([]Edge, len(path)*2+5)
	edgeCnt := 0
	for _, i := range path {
		//无向图加边
		edges[edgeCnt].To = i.DescId
		edges[edgeCnt].Length = i.Distance
		edges[edgeCnt].Next = head[i.FromId]
		head[i.FromId] = edgeCnt
		edgeCnt++
		edges[edgeCnt].To = i.FromId
		edges[edgeCnt].Length = i.Distance
		edges[edgeCnt].Next = head[i.DescId]
		head[i.DescId] = edgeCnt
		edgeCnt++
	}
	return head, edges
}

var Head []int
var Edges []Edge

func InitMap(nodeCnt int, path []system.Path) {
	// 使用前向星存图
	Head = make([]int, nodeCnt+5)
	for i := range Head {
		Head[i] = -1
	}

	// 建边
	Head, Edges = addEdge(path, Head)
}
