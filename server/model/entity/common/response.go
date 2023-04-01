package common

// 状态响应
type StatusResponse struct {
	StatusCode int    `json:"status_code"`
	StatusMsg  string `json:"status_msg"`
}

// 权限响应 (userId, token)
type AccessResponse struct {
	UserId  int64  `json:"user_id"`
	Token   string `json:"token"`
	IsAdmin bool   `json:"is_admin"`
}
