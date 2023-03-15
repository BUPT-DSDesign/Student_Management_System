package dao

import "server/model/entity/system"

// UserInfo 这里先把userInfo放在内存中
var UserInfo *system.UserInfo

func init() {
	UserInfo = &system.UserInfo{
		Username:  "admin",
		UserId:    123123213213,
		StudentId: "2021211201",
		IsAdmin:   true,
		Signature: "",
	}
}
