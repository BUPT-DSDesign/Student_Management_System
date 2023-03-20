package dao

import "server/model/entity/system"

// UserInfo 这里先把userInfo放在内存中
var UserInfo *system.UserInfo

func init() {
	UserInfo = &system.UserInfo{
		Username:  "admin",
		UserId:    123456789,
		StudentId: "2021211201",
		IsAdmin:   true,
		Signature: "",
		AvatarUrl: "http://127.0.0.1:8080/static/avatar.jpg",
	}
}
