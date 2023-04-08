package tsp

import (
	"fmt"
	"math/rand"
	"sort"
	"time"
)

const (
	populationNum  = 1000 // 种群规模
	iterationCount = 3000 // 迭代次数
	crossoverRate  = 0.9  // 交叉率
	variationRate  = 0.1  // 变异率
)

// 路线结构体 (一条路线对应一个个体)
type route struct {
	passList      []int   // 途径节点编号
	totalDistance float64 // 路线总距离
	fitness       float64 // 个体适应度
	survivalRate  float64 // 生存概率
}

var population = make([]route, populationNum) // 种群

// 种群初始化
func populationInit(individualNum int) {
	// 先创建一个全排列
	permutation := make([]int, individualNum-1)
	for i := range permutation {
		permutation[i] = i + 1
	}

	// 随机化得到个体
	for i := 0; i < populationNum; i++ {
		// 随机化permutation
		rand.Shuffle(len(permutation), func(i, j int) { permutation[i], permutation[j] = permutation[j], permutation[i] })
		individual := append(permutation, 0)

		// 翻转individual
		temp := 0 // 临时变量
		for j := 0; j < len(individual)/2; j++ {
			temp = individual[len(individual)-1-j]
			individual[len(individual)-1-j] = individual[j]
			individual[j] = temp
		}

		// 得到passList
		population[i].passList = make([]int, len(individual))
		copy(population[i].passList, individual)

	}
}

// 计算种群中每个个体适应度以及生存概率 (适应度设置为totalDistance的倒数)
func populationCalc(indexMap []int) {
	totalFitness := 0.0 // 种群总适应度

	// 得到每个个体的适应度
	for i := range population {
		// 得到totalDistance
		population[i].totalDistance = 0.0
		for j, v := range population[i].passList {
			population[i].totalDistance += routeMatrix[indexMap[v]][indexMap[population[i].passList[(j+1)%len(population[i].passList)]]].totalDistance
		}
		// 通过totalDistance得到fitness
		population[i].fitness = 1.0 / population[i].totalDistance

		totalFitness += population[i].fitness
	}

	// 得到每个个体的生存概率
	for i := range population {
		population[i].survivalRate = population[i].fitness / totalFitness
	}
}

// 选择
func populationSelect() {
	// 计算累计概率
	accumulateRate := make([]float64, populationNum)
	accumulateRate[0] = population[0].survivalRate
	for i := 1; i < populationNum; i++ {
		accumulateRate[i] = accumulateRate[i-1] + population[i].survivalRate
	}

	// 通过赌轮选择法对个体进行选择
	selectPopulation := make([]route, populationNum)
	for i := 0; i < populationNum; i++ {
		randomRate := rand.Float64()
		for j := 0; j < len(accumulateRate); j++ {
			if randomRate <= accumulateRate[j] {
				selectPopulation[i] = population[j]
				break
			}
		}
	}

	// 种群更新
	for i := 0; i < populationNum; i++ {
		population[i] = selectPopulation[i]
	}
}

// 交叉
func populationCrossover(individualNum int) {
	// 随机生成子代交配时交换染色体的数量
	changeChromosomeNum := rand.Intn(individualNum/2) + 1

	// 子代交配
	for i := 0; i < populationNum; i += 2 {
		randomRate := rand.Float64()

		// 如果randomRate在交叉率以内, 则i个体与i + 1个体交配
		if randomRate < crossoverRate {
			// 生成随机交配点
			randomPoint := rand.Intn(individualNum - changeChromosomeNum + 1)

			// 将二者的交叉片段互换, 并解决基因冲突
			clashMap := make(map[int]int, 0)
			for j := randomPoint; j < changeChromosomeNum+randomPoint; j++ {
				iChromosome := population[i].passList[j]
				ipChromosome := population[i+1].passList[j]

				if v, ok := clashMap[iChromosome]; ok {
					iChromosome = v
				}
				if v, ok := clashMap[ipChromosome]; ok {
					ipChromosome = v
				}
				clashMap[iChromosome] = ipChromosome
				clashMap[ipChromosome] = iChromosome

				// 交换
				tmp := population[i].passList[j]
				population[i].passList[j] = population[i+1].passList[j]
				population[i+1].passList[j] = tmp
			}

			// 解决基因冲突问题（断点前）
			for j := 0; j < randomPoint; j++ {
				if v, ok := clashMap[population[i].passList[j]]; ok {
					population[i].passList[j] = v
				}
				if v, ok := clashMap[population[i+1].passList[j]]; ok {
					population[i+1].passList[j] = v
				}
			}

			// 断点后
			for j := randomPoint + changeChromosomeNum; j < individualNum; j++ {
				if v, ok := clashMap[population[i].passList[j]]; ok {
					population[i].passList[j] = v
				}
				if v, ok := clashMap[population[i+1].passList[j]]; ok {
					population[i+1].passList[j] = v
				}
			}
		}
	}
}

// 变异
func populationVariation(individualNum int) {
	for i := 0; i < populationNum; i++ {
		randomRate := rand.Float64()

		// 如果randomRate在变异率以内, 则i个体变异
		if randomRate < variationRate {
			// 基因交换次数
			exchangeTimes := rand.Intn(individualNum) + 1
			for ; exchangeTimes > 0; exchangeTimes-- {
				a := rand.Intn(individualNum-1) + 1
				b := rand.Intn(individualNum-1) + 1

				// 交换
				tmp := population[i].passList[a]
				population[i].passList[a] = population[i].passList[b]
				population[i].passList[b] = tmp
			}
		}
	}

}

// ga算法
func ga(individualNum int, indexMap []int) []int {
	// 初始化操作
	rand.Seed(time.Now().UnixNano()) // 设置随机种子
	populationInit(individualNum)
	populationCalc(indexMap)

	sort.Slice(population, func(i, j int) bool {
		return population[i].totalDistance > population[j].totalDistance
	})

	// 开始种群迭代
	for i := 0; i < iterationCount; i++ {
		// 计算适应度以及生存概率
		populationCalc(indexMap)
		// 在种群中选择个体
		populationSelect()
		// 种群进行交配
		populationCrossover(individualNum)
		// 种群中的个体产生变异
		populationVariation(individualNum)
	}
	populationCalc(indexMap)

	sort.Slice(population, func(i, j int) bool {
		return population[i].totalDistance > population[j].totalDistance
	})

	for i := 0; i < populationNum; i++ {
		fmt.Printf("%d个体：", i)
		for j := 0; j < individualNum; j++ {
			fmt.Printf("%d ", population[i].passList[j])
		}
		fmt.Printf("\n")
	}

	return population[populationNum-1].passList
}
