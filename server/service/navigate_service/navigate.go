package navigate_service

import (
	"encoding/csv"
	"errors"
	"log"
	"math"
	"os"
	"server/algorithm/dijkstra"
	"server/model/entity/common"
	"server/model/entity/system"
	"strconv"
	"strings"
)

type Node struct {
	Index     int     `json:"index"`
	Name      string  `json:"name"`
	Longitude float64 `json:"longitude"`
	Latitude  float64 `json:"latitude"`
}

// 计算航向角
func calculateHeading(coorX [2]float64, coorY [2]float64) int {
	//先求出向量
	vectX := coorY[0] - coorX[0]
	vectY := coorY[1] - coorX[1]
	//随后求出正切值,求反三角函数后转换为弧度制
	//同时由于此为浮点数运算，故不需要担心除数为0的问题
	res := math.Atan(vectX/vectY) * 180.0 / math.Pi
	//四舍五入返回结果
	return (int(res+0.5) + 360) % 360
}

// 计算地球上两点之间直线距离,同时由于计算的两点之间距离较近,故采用优化过的Haversine公式计算
func calculateDistance(coorX [2]float64, coorY [2]float64) float64 {
	//先定义常量
	r := 6367000.0 //WGS-84中长半轴与短半轴的平均值
	toRadians := func(x float64) float64 { return x * math.Pi / 180.0 }
	dx := coorX[0] - coorY[0]
	//纬度差
	dy := coorX[1] - coorY[1]
	//平均纬度
	b := (coorX[1] + coorY[1]) / 2
	//计算东西方向距离和南北方向距离
	Lx := toRadians(dx) * r * math.Cos(toRadians(b))
	Ly := r * toRadians(dy)
	return math.Sqrt(Lx*Lx + Ly*Ly)
}

// 获取节点列表
func getNodeList() []Node {
	//打开文件(只读模式)，创建io.read接口实例
	opencast, err := os.Open("./resources/node.csv")
	if err != nil {
		log.Println("csv文件打开失败！")
		return nil
	}
	defer func(opencast *os.File) {
		err := opencast.Close()
		if err != nil {
			log.Println(err.Error())
		}
	}(opencast)

	//创建csv读取接口实例
	ReadCsv := csv.NewReader(opencast)

	//丢弃首行内容
	_, _ = ReadCsv.Read() //返回切片类型：[chen  hai wei]
	var nodelist []Node
	//读取所有内容
	ReadAll, err := ReadCsv.ReadAll() //返回切片类型：[[s s ds] [a a a]]
	for _, v := range ReadAll {
		id, _ := strconv.Atoi(v[0])
		x, _ := strconv.ParseFloat(v[2], 64)
		y, _ := strconv.ParseFloat(strings.Replace(v[3], "\r", "", -1), 64)
		nodelist = append(nodelist, Node{
			Index:     id,
			Name:      v[1],
			Longitude: x,
			Latitude:  y,
		})
	}
	return nodelist
}

// 获取道路列表
func getPathList() []system.Path {
	//打开文件(只读模式)，创建io.read接口实例
	opencast, err := os.Open("./resources/edge.csv")
	if err != nil {
		log.Println("csv文件打开失败！")
		return nil
	}
	defer func(opencast *os.File) {
		err := opencast.Close()
		if err != nil {
			log.Println(err.Error())
		}
	}(opencast)

	//创建csv读取接口实例
	ReadCsv := csv.NewReader(opencast)

	//丢弃首行内容
	_, _ = ReadCsv.Read() //返回切片类型：[chen  hai wei]
	var pathlist []system.Path
	//读取所有内容
	ReadAll, err := ReadCsv.ReadAll() //返回切片类型：[[s s ds] [a a a]]
	for _, v := range ReadAll {
		var tmp system.Path
		tmp.FromId, _ = strconv.Atoi(v[0])
		tmp.DescId, _ = strconv.Atoi(v[1])
		tmp.Distance, _ = strconv.ParseFloat(strings.Replace(v[2], "\r", "", -1), 64)
		pathlist = append(pathlist, tmp)
	}
	return pathlist
}

// GetCoordinateSet 将id转换为坐标列表
func (s *server) GetCoordinateSet(nodeId []int) [][2]float64 {
	type Coor = [2]float64
	var result []Coor
	for _, v := range nodeId {
		result = append(result, Coor{s.tempNodeList[v].Longitude, s.tempNodeList[v].Latitude})
	}
	return result
}

func (s *server) DoNavigation(navigateRequest common.NavigateRequest) ([][2]float64, error) {
	result, _, err := dijkstra.Dijkstra(navigateRequest.FromId, navigateRequest.DesId, len(s.tempNodeList))
	return s.GetCoordinateSet(result), err
}
func (s *server) DoGetNavigationInfo(nodeList [][2]float64) ([]system.PathGuider, error) {
	nodeCnt := len(nodeList)
	if nodeCnt <= 1 {
		return nil, errors.New("节点个数错误,可能是起点与终点一致")
	}
	var result []system.PathGuider
	for i := 0; i < nodeCnt-1; i++ {
		heading := calculateHeading(nodeList[i], nodeList[i+1])
		distance := calculateDistance(nodeList[i], nodeList[i+1])
		result = append(result, system.PathGuider{
			Heading:  heading,
			Distance: distance,
		})
	}
	return result, nil
}
