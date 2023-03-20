package system

// Path 无向边
type Path struct {
	FromId   int
	DescId   int
	Distance float64
}

// FacilityInfo 学校设施信息
type FacilitiyInfo struct {
	FacID   int64  `json:"fac_id"`   // 设施ID，设施ID
	FacName string `json:"fac_name"` // 设施名，设施名称
}

// PathGuider 导航路径展示
type PathGuider struct {
	Heading  int     // 向量方向,用航向角来计算(正北为0,360等分)
	Distance float64 // 向量长度,单位为米
}
