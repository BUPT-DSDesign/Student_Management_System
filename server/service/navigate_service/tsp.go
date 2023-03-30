package navigate_service

import (
	"errors"
	"server/algorithm/tsp"
	"server/model/entity/system"
)

type tspFlow struct {
	// 包含handler层传来的参数等
	startId  int
	passIds  []int
	nodeList []Node
	pathList []system.Path
}

func newTSPFlow(startId int, passIds []int, nodeList []Node, pathList []system.Path) *tspFlow {
	return &tspFlow{startId: startId, passIds: passIds, nodeList: nodeList, pathList: pathList}
}

func (s *server) DoTSP(startId int, passIds []int) (*[][2]float64, error) {
	return newTSPFlow(startId, passIds, s.tempNodeList, s.tempPathList).do()
}

func (f *tspFlow) do() (*[][2]float64, error) {
	var nodeList *[][2]float64

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&nodeList); err != nil {
		return nil, err
	}

	return nodeList, nil
}

// 检验参数
func (f *tspFlow) checkNum() error {
	if f.startId < 0 || f.startId >= len(f.nodeList) {
		return errors.New("开始节点编号错误")
	}
	for _, id := range f.passIds {
		if id < 0 || id >= len(f.nodeList) {
			return errors.New("途径节点编号错误")
		}
	}

	return nil
}

func (f *tspFlow) run(nodeList **[][2]float64) error {
	// 调用tsp算法得到途径节点编号列表
	result, err := tsp.TSP(f.startId, f.passIds)
	if err != nil {
		return err
	}

	// 还需要将途径节点的编号变成经纬度传给前端
	var tmpNodeList [][2]float64
	for _, v := range result {
		tmpNodeList = append(tmpNodeList, [2]float64{f.nodeList[v].Longitude, f.nodeList[v].Latitude})
	}

	*nodeList = &tmpNodeList
	return nil
}
