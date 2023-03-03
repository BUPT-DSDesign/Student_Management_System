package system

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
