package navigate_service

import (
	"encoding/csv"
	"log"
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
func (s *server) GetCoordinateSet(nodeId []int) []system.Coordinate {
	var result []system.Coordinate
	for _, v := range nodeId {
		result = append(result, system.Coordinate{
			Longitude: s.tempNodeList[v].Longitude,
			Latitude:  s.tempNodeList[v].Latitude,
		})
	}
	return result
}

func (s *server) DoNavigation(navigateRequest common.NavigateRequest) ([]system.Coordinate, error) {
	result, err := dijkstra.Dijkstra(navigateRequest.FromId, navigateRequest.DesId, s.tempPathList, len(s.tempNodeList))
	return s.GetCoordinateSet(result), err
}
