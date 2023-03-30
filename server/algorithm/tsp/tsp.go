package tsp

import (
	"fmt"
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

func TSP(startId int, passIds []int, paths []system.Path, nodeCnt int) ([]int, error) {
	// 使用前向星存图
	head := make([]int, nodeCnt+5)

	//初始化为-1
	for i := range head {
		head[i] = -1
	}

	//获取前向星存图
	head, edges := addEdge(paths, head)
	println(edges)

	indexMap := make([]int, len(passIds)+1)
	indexMap[0] = startId

	//println(len(indexMap))
	rawNodeList := ga(len(indexMap), indexMap)

	nodeList := make([]int, 0)
	total := 0.0
	for i := 0; i < len(rawNodeList); i++ {
		for j := 0; j < len(routeMatrix[indexMap[rawNodeList[i]]][indexMap[rawNodeList[(i+1)%len(rawNodeList)]]].passList)-1; j++ {
			nodeList = append(nodeList, routeMatrix[indexMap[rawNodeList[i]]][indexMap[rawNodeList[(i+1)%len(rawNodeList)]]].passList[j])
			total += routeMatrix[indexMap[rawNodeList[i]]][indexMap[rawNodeList[(i+1)%len(rawNodeList)]]].totalDistance
		}
	}
	nodeList = append(nodeList, startId)

	println(nodeList[0])
	fmt.Printf("总距离: %d\n", int64(total))

	return nodeList, nil
}
