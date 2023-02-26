package common

// 登录请求
type LoginRequest struct {
	Username string `json:"username" form:"username"`
	Password string `json:"password" form:"password"`
}

// 注册请求
type RegisterRequest struct {
	StudentId string `json:"student_id" form:"student_id"`
	LoginRequest
}
