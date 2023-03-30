package tsp

import (
	"server/algorithm/dijkstra"
	"server/model/entity/system"
)

type bestRoute struct {
	passList      []int
	totalDistance float64
}

var routeMatrix [][]bestRoute

func InitRouteMatrix(path []system.Path, nodeCnt int) {
	// 创建路线矩阵
	println(nodeCnt)
	routeMatrix = make([][]bestRoute, nodeCnt)
	for i := 0; i < nodeCnt; i++ {
		routeMatrix[i] = make([]bestRoute, nodeCnt)
	}

	// 利用dij算法求出routeMatrix
	for i := 0; i < nodeCnt; i++ {
		for j := 0; j < nodeCnt; j++ {
			if i == j {
				continue
			}
			routeMatrix[i][j].passList, routeMatrix[i][j].totalDistance, _ = dijkstra.Dijkstra(i, j, path, nodeCnt)
		}
	}
}
