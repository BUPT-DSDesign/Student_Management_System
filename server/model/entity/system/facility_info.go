package system

// FacilityInfo 学校设施信息
type FacilityInfo struct {
	FacID   int64  `json:"fac_id"`   // 设施ID，设施ID
	FacName string `json:"fac_name"` // 设施名，设施名称
}
