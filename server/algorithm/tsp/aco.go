package tsp

import (
	"math"
	"math/rand"
	"time"
)

const (
	antCnt  = 30   // 蚂蚁数量
	loopCnt = 200  // 循环次数
	alpha   = 2    // 信息启发因子
	beta    = 3    // 期望启发式因子
	rou     = 0.1  // 全局信息素挥发参数
	alpha1  = 0.1  // 局部信息素挥发参数
	qzero   = 0.01 // 状态转移公式中的q0
)

var Lnn float64 // 由最近邻近方法得到的一个长度

// 获得经过n个城市的路径长度
func calcSumDistance(path [][2]int, indexMap []int) float64 {
	sum := 0.0
	for i := 0; i < len(indexMap); i++ {
		sum += routeMatrix[indexMap[path[i][0]]][indexMap[path[i][1]]].totalDistance
	}
	return sum
}

// 蚁群系统
type antColonySystem struct {
	nodeCnt   int         // 节点个数
	infoPower [][]float64 // 节点之间的信息素强度
	visible   [][]float64 // 节点之间的能见度
}

// 初始化系统
func (s *antColonySystem) initPara(nodeCnt int, value float64, indexMap []int) {
	// 初始化变量
	s.nodeCnt = nodeCnt
	s.infoPower = make([][]float64, nodeCnt)
	for i := 0; i < nodeCnt; i++ {
		s.infoPower[i] = make([]float64, nodeCnt)
	}
	s.visible = make([][]float64, nodeCnt)
	for i := 0; i < nodeCnt; i++ {
		s.visible[i] = make([]float64, nodeCnt)
	}

	// 初始化路径上的信息素强度
	for i := 0; i < nodeCnt; i++ {
		for j := 0; j < nodeCnt; j++ {
			s.infoPower[i][j] = value
			s.infoPower[j][i] = value
			if i != j {
				s.visible[i][j] = 1.0 / routeMatrix[indexMap[i]][indexMap[j]].totalDistance
				s.visible[j][i] = s.visible[i][j]
			}
		}
	}
}

// 计算节点i -> j的概率
func (s *antColonySystem) transition(i int, j int) float64 {
	if i != j {
		return math.Pow(s.infoPower[i][j], alpha) * math.Pow(s.visible[i][j], beta)
	}
	return 0.0
}

// 局部更新 i -> j 的信息素
func (s *antColonySystem) updateLocal(i int, j int) {
	s.infoPower[i][j] = (1.0-alpha1)*s.infoPower[i][j] + alpha1*(1.0/(float64(s.nodeCnt)*Lnn))
	s.infoPower[j][i] = s.infoPower[i][j]
}

// 全局更新信息素
func (s *antColonySystem) updateGlobal(bestPath [][2]int, bestPathLength float64) {
	for i := 0; i < s.nodeCnt; i++ {
		st := bestPath[i][0]
		ed := bestPath[i][1]
		s.infoPower[st][ed] = (1.0-rou)*s.infoPower[st][ed] + rou*(1.0/bestPathLength)
		s.infoPower[ed][st] = s.infoPower[st][ed]
	}
}

// 蚂蚁
type ant struct {
	antColony        *antColonySystem
	startNode        int
	currentNode      int
	allowed          []int    // 禁忌表
	path             [][2]int // 当前路径
	currentPathIndex int      // 当前路径索引
}

// 开始搜索
func (s *ant) search() [][2]int {
	s.currentNode = s.startNode
	s.currentPathIndex = 0
	for i := 0; i < s.antColony.nodeCnt; i++ {
		s.allowed[i] = 1
	}
	s.allowed[s.currentNode] = 0

	var endNode int
	toNode := 0
	count := 0

	for toNode >= 0 {
		count++
		endNode = s.currentNode
		toNode = s.choose()
		if toNode >= 0 {
			s.moveToNextNode(toNode)
			s.antColony.updateLocal(endNode, toNode)
			s.currentNode = toNode
		}
	}

	s.moveToNextNode(s.startNode)
	s.antColony.updateLocal(endNode, s.startNode)

	return s.path
}

// 选择下一节点
func (s *ant) choose() int {
	nextNode := -1
	rand.Seed(time.Now().UnixNano())
	q := rand.Float64()

	// 如果q <= q0, 按先验知识, 否则按概率转移
	if q <= qzero {
		probability := -1.0
		for i := 0; i < s.antColony.nodeCnt; i++ {
			if s.allowed[i] == 1 {
				prob := s.antColony.transition(s.currentNode, i)
				if prob > probability {
					nextNode = i
					probability = prob
				}
			}
		}
	} else {
		rand.Seed(time.Now().UnixNano())
		p := rand.Float64() // 生成一个随机数, 用来判断落在哪个区间
		sum := 0.0
		probability := 0.0 // 概率的区间点
		// 计算概率公式的分母的值
		for i := 0; i < s.antColony.nodeCnt; i++ {
			if s.allowed[i] == 1 {
				sum += s.antColony.transition(s.currentNode, i)
			}
		}
		for i := 0; i < s.antColony.nodeCnt; i++ {
			if s.allowed[i] == 1 && sum > 0 {
				probability += s.antColony.transition(s.currentNode, i) / sum
				if probability >= p || (p > 0.9999 && probability > 0.9999) {
					nextNode = i
					break
				}
			}
		}
	}

	return nextNode
}

// 移动到下一节点
func (s *ant) moveToNextNode(nextNode int) {
	s.allowed[nextNode] = 0
	s.path[s.currentPathIndex][0] = s.currentNode
	s.path[s.currentPathIndex][1] = nextNode
	s.currentPathIndex++
	s.currentNode = nextNode
}

// 选择下一节点
func chooseNextNode(currentNode int, visitedNode []int, nodeCnt int, indexMap []int) int {
	nextNode := -1
	shortDistance := 0.0
	for i := 0; i < nodeCnt; i++ {
		// 去掉已经走过的节点, 从剩下的节点中选择距离最近的节点
		if visitedNode[i] == 1 {
			if shortDistance == 0.0 {
				shortDistance = routeMatrix[indexMap[currentNode]][indexMap[i]].totalDistance
				nextNode = i
			}
			if shortDistance < routeMatrix[indexMap[currentNode]][indexMap[i]].totalDistance {
				nextNode = i
			}

		}
	}
	return nextNode
}

// 给一个节点由最近邻距离方法计算长度
func calcAdjacentDistance(node int, nodeCnt int, indexMap []int) float64 {
	sum := 0.0
	visitedNode := make([]int, nodeCnt)
	for i := 0; i < nodeCnt; i++ {
		visitedNode[i] = 1
	}
	visitedNode[node] = 0
	currentNode := node
	nextNode := 0

	for nextNode >= 0 {
		nextNode = chooseNextNode(currentNode, visitedNode, nodeCnt, indexMap)
		if nextNode >= 0 {
			sum += routeMatrix[indexMap[currentNode]][indexMap[nextNode]].totalDistance
			currentNode = nextNode
			visitedNode[currentNode] = 0
		}
	}

	sum += routeMatrix[indexMap[currentNode]][indexMap[node]].totalDistance // 回到起点
	return sum
}

func aco(nodeCnt int, indexMap []int) []int {
	// 蚁群系统
	Lnn = calcAdjacentDistance(0, nodeCnt, indexMap)
	acs := new(antColonySystem)
	initInfo := 1.0 / (float64(nodeCnt) * Lnn)
	acs.initPara(nodeCnt, initInfo, indexMap)

	// 蚂蚁
	ants := make([]*ant, antCnt)
	for i := 0; i < antCnt; i++ {
		ants[i] = new(ant)
		ants[i].antColony = acs
		ants[i].startNode = 0
		ants[i].allowed = make([]int, nodeCnt)
		ants[i].path = make([][2]int, nodeCnt)
	}

	globalPath := make([][2]int, nodeCnt) // 全局最优路径
	globalPathLength := 1e9               // 全局最优路径长度

	for cnt := 0; cnt < loopCnt; cnt++ {
		localPath := make([][2]int, nodeCnt) // 局部最优路径
		localPathLength := 1e9               // 局部最优路径长度

		var pathLength float64
		for i := 0; i < antCnt; i++ {
			path := ants[i].search()
			pathLength = calcSumDistance(path, indexMap)
			if pathLength < localPathLength {
				for j := 0; j < nodeCnt; j++ {
					localPath[j][0] = path[j][0]
					localPath[j][1] = path[j][1]
				}
				localPathLength = pathLength
			}
		}

		// 全局比较
		if localPathLength < globalPathLength {
			for j := 0; j < nodeCnt; j++ {
				globalPath[j][0] = localPath[j][0]
				globalPath[j][1] = localPath[j][1]
			}
			globalPathLength = localPathLength
		}
		acs.updateGlobal(globalPath, globalPathLength)
	}

	passList := make([]int, nodeCnt)
	for i := 0; i < nodeCnt; i++ {
		passList[i] = globalPath[i][0]
	}

	return passList
}
