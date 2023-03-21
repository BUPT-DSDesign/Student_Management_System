package tsp

import "server/model/entity/system"

type bestRoute struct {
	passList      []int
	totalDistance float64
}

var routeMatrix [][]bestRoute

func InitRouteMatrix(path []system.Path, nodeCnt int) {
	// 创建路线矩阵
	routeMatrix = make([][]bestRoute, nodeCnt)
	for i := 0; i < nodeCnt; i++ {
		routeMatrix[i] = make([]bestRoute, nodeCnt)
	}

	// 利用
}

