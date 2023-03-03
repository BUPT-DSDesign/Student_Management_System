package navigate_service

import (
	"server/model/entity/common"
	"server/model/entity/system"
)

type NavigateServer interface {
	// DoNavigation 开始导航,返回导航路径
	DoNavigation(navigateRequest common.NavigateRequest) ([]system.Coordinate, error)
	// DoGetFacilityList 返回待查询建筑物列表
	DoGetFacilityList(request common.FacilityRequest) (*[]*system.FacilityInfo, error)
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

}
