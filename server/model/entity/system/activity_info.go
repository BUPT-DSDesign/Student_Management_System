package system

type ActivityInfo struct {
	ActivityName string `json:"activity_name"`
	ActivityId   int64  `json:"activity_id"`
	UserId       int64  `json:"user_id"`
	StartWeek    int    `json:"start_week"`
	StartDay     int    `json:"start_day"`
	StartTime    string `json:"start_time"`
	DurationTime int    `json:"duration_time"`
	Type         int    `json:"type"`
	Tag          int    `json:"tag"`
	Location     string `json:"location"`
	IsOnline     bool   `json:"is_online"`   // 是否线上
	LocationNode int    `json:"location_id"` // 线下地点的节点编号
	IsMention    bool   `json:"is_mention"`  // 是否需要提醒
}
