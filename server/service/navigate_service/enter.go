package navigate_service

import (
	"server/algorithm/tsp"
	"server/model/entity/common"
	"server/model/entity/system"
)

type NavigateServer interface {
	// DoNavigation 开始导航,返回导航路径
	DoNavigation(navigateRequest common.NavigateRequest) ([][2]float64, error)
	// DoGetFacilityList 返回待查询建筑物列表
	DoGetFacilityList(request common.FacilityRequest) (*[]*system.FacilityInfo, error)
	// DoGetNavigationInfo 获取导航的路径信息
	DoGetNavigationInfo(nodeList [][2]float64) ([]system.PathGuider, error)
	// DoTSP 旅行商问题
	DoTSP(startId int, passIds []int) (*[][2]float64, error)
}
type server struct {
	tempNodeList []Node
	tempPathList []system.Path
}

var Server NavigateServer

func init() {
	Server = &server{
		tempNodeList: getNodeList(),
		tempPathList: getPathList(),
	}

	// 两点之间的路线矩阵
	tsp.InitRouteMatrix(Server.(*server).tempPathList, len(Server.(*server).tempNodeList))
}
