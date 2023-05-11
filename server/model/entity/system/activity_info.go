package system

type ActivityInfo struct {
	ActivityId   int64  `json:"activity_id"`
	ActivityName string `json:"activity_name"`
	UserId       int64  `json:"user_id"`
	StartTime    string `json:"start_time"`
	Type         int    `json:"type"`
	Location     string `json:"location"`
	Tag          string `json:"tag"`
	Frequency    int    `json:"frequency"`
	IsMention    bool   `json:"is_mention"` // 是否需要提醒
}
