package system

// NavigateRequest 导航请求
type NavigateRequest struct {
	FromId int `json:"from_id"`
	DesId  int `json:"des_id"`
}

// Path 无向边
type Path struct {
	FromId   int
	DescId   int
	Distance float64
}
