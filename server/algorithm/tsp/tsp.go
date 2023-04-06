package tsp

import (
	"fmt"
)

func TSP(startId int, passIds []int) ([]int, error) {
	// 将id号映射成0, 1, 2...这种序列, 0对应着出发点
	indexMap := make([]int, len(passIds)+1)
	indexMap[0] = startId
	for i := 1; i < len(indexMap); i++ {
		indexMap[i] = passIds[i-1]
	}

	//ga算法
	//rawNodeList := ga(len(indexMap), indexMap)

	// 蚁群算法
	rawNodeList := aco(len(indexMap), indexMap)

	// 得到路径, 并计算路径长度
	nodeList := make([]int, 0)
	total := 0.0
	for i := 0; i < len(rawNodeList); i++ {
		for j := 0; j < len(routeMatrix[indexMap[rawNodeList[i]]][indexMap[rawNodeList[(i+1)%len(rawNodeList)]]].passList)-1; j++ {
			nodeList = append(nodeList, routeMatrix[indexMap[rawNodeList[i]]][indexMap[rawNodeList[(i+1)%len(rawNodeList)]]].passList[j])
		}
		total += routeMatrix[indexMap[rawNodeList[i]]][indexMap[rawNodeList[(i+1)%len(rawNodeList)]]].totalDistance
	}
	nodeList = append(nodeList, startId)

	fmt.Printf("总距离: %d\n", int64(total))

	return nodeList, nil
}
