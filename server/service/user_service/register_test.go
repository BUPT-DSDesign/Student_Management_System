package user_service

import (
	"fmt"
	"server/model/entity/system"
	"server/utils"
	"testing"
)

func Test(t *testing.T) {
	userInfo := &system.UserInfo{
		Username:  "test",
		Password:  "123456",
		AvatarUrl: "http://127.0.0.1:8080/static/avatar.jpg",
		Signature: "",
	}
	sqlStr := fmt.Sprintf("INSERT INTO user_info VALUES('%v', '%v', '%v', '%v', '%v', '%v', '%v', '%v')",
		userInfo.Username,
		userInfo.Password,
		userInfo.Salt,
		userInfo.UserId,
		userInfo.StudentId,
		utils.BoolToInt8(userInfo.IsAdmin),
		userInfo.AvatarUrl,
		userInfo.Signature,
	)
	t.Log(sqlStr)
}
