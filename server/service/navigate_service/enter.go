package navigate_service

import (
	"server/handler/navigate_handler"
	"server/model/entity/system"
)

type NavigateServer interface {
	// DoNavigation 开始导航,返回导航路径
	DoNavigation(navigateRequest common.NavigateRequest) ([]system.Coordinate, error)
	// DoGetFacilityList 返回待查询建筑物列表
	DoGetFacilityList(request system.FacilityRequest) ([]navigate_handler.FacilitiyInfo, error)
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
