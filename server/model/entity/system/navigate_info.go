package system

import "server/model/entity/common"

// NavigateRequest 导航请求
type NavigateRequest struct {
	FromId int `json:"from_id"`
	DesId  int `json:"des_id"`
}

// 导航响应
type NavigateResponse struct {
	common.StatusResponse
	NodeList []Coordinate `json:"node_list"`
}

// Path 无向边
type Path struct {
	FromId   int
	DescId   int
	Distance float64
}
