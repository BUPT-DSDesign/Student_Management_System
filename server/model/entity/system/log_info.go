package system

type LogInfo struct {
	LogId      int64  `json:"log_id"`
	CreateTime string `json:"create_time"`
	Content    string `json:"content"`
	UserId     int64  `json:"user_id"` // 对应的用户id, 对应某个用户的日志
}
