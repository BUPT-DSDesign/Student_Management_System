package navigate_service

import (
	"encoding/csv"
	"log"
	"os"
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

var tempNodeList []Node
var tempPathList []system.Path

// 获取节点列表
func getNodeList() []Node {
	//打开文件(只读模式)，创建io.read接口实例
	opencast, err := os.Open("./resources/node.csv")
	if err != nil {
		log.Println("csv文件打开失败！")
		return nil
	}
	defer opencast.Close()

	//创建csv读取接口实例
	ReadCsv := csv.NewReader(opencast)

	//获取一行内容，一般为第一行内容
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
	defer opencast.Close()

	//创建csv读取接口实例
	ReadCsv := csv.NewReader(opencast)

	//获取一行内容，一般为第一行内容
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
func GetCoordinateSet(nodeId []int) []system.Coordinate {
	var result []system.Coordinate
	for _, v := range nodeId {
		result = append(result, system.Coordinate{
			Longitude: tempNodeList[v].Longitude,
			Latitude:  tempNodeList[v].Latitude,
		})
	}
	return result
}
