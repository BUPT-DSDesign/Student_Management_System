package system

// UserInfo 系统用户信息
type UserInfo struct {
	Username  string `json:"username"`
	Password  string `json:"-"` // 加密之后的密码
	Salt      string `json:"-"` // 密码加盐
	UserId    int64  `json:"user_id"`
	StudentId string `json:"student_id"`
	IsAdmin   bool   `json:"is_admin"`
	AvatarUrl string `json:"avatar_url"` // 保存用户头像的路径
	// ......后续应该还要加上一些字段
	Signature string `json:"signature"`
}
