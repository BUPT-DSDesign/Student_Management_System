package dao

import (
	"fmt"
	"server/model/entity/system"
	"server/utils"
)

// AddUser 添加用户操作
func (s *userDao) AddUser(userInfo *system.UserInfo) error {
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

	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	return nil
}

// QueryUserById 根据用户id查询用户
func (s *userDao) QueryUserById(userId int64, userInfo **system.UserInfo) error {
	/*
		根据id查询用户
	*/
	return nil
}

// QueryUserByName 根据用户名查询用户
func (s *userDao) QueryUserByName(username string, userInfo **system.UserInfo) error {
	/*
		根据用户名查询用户
	*/
	return nil
}

// UpdateUserAvatar 更新用户头像
func (s *userDao) UpdateUserAvatar(userId int64, avatarUrl string) error {
	/*
		更新用户头像
	*/
	return nil
}

// UpdateSignature 更新用户个性签名
func (s *userDao) UpdateSignature(userId int64, signature string) error {
	/*
		更新用户个性签名
	*/
	return nil
}
