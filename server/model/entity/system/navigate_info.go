package system

// NavigateRequest 导航请求
type NavigateRequest struct {
	FromId int `json:"from_id"`
	DesId  int `json:"des_id"`
}

// FacilityRequest 建筑物信息查询请求
type FacilityRequest struct {
	Count      int64   `json:"count"`              // 返回计数
	FacID      *int64  `json:"fac_id,omitempty"`   // 设施ID，设施ID
	FacName    *string `json:"fac_name,omitempty"` // 设施名，设施名称
	IsSpecific bool    `json:"is_specific"`        // 是否为精确查找，精确查找为True，使用fac_name或fac_id来精确查询；如果不是，则返回建筑物列表
	Offset     int64   `json:"offset"`             // 偏移量
}

// Path 无向边
type Path struct {
	FromId   int
	DescId   int
	Distance float64
}

// 学校设施信息
type FacilitiyInfo struct {
	FacID   int64  `json:"fac_id"`   // 设施ID，设施ID
	FacName string `json:"fac_name"` // 设施名，设施名称
}
