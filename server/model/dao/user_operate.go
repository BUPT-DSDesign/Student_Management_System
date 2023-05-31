package dao

import (
	"encoding/json"
	"errors"
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

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(int) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	return nil
}

// QueryUserById 根据用户id查询用户
func (s *userDao) QueryUserById(userId int64, userInfo **system.UserInfo) error {
	/*
		根据id查询用户
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM user_info WHERE user_id = '%v'", userId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(int) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]int64
	_ = json.Unmarshal([]byte(result["data"].(string)), *userInfo)

	return nil
}

// QueryUserByName 根据用户名查询用户
func (s *userDao) QueryUserByName(username string, userInfo **system.UserInfo) error {
	/*
		根据用户名查询用户
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM user_info WHERE username = '%v'", username)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()

	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(int) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]int64

	_ = json.Unmarshal([]byte(result["data"].(string)), *userInfo)

	return nil
}

// UpdateUserAvatar 更新用户头像
func (s *userDao) UpdateUserAvatar(userId int64, avatarUrl string) error {
	/*
		更新用户头像
	*/
	sqlStr := fmt.Sprintf("UPDATE user_info SET avatar_url = '%v' WHERE user_id = '%v'", avatarUrl, userId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(int) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	return nil
}

// UpdateSignature 更新用户个性签名
func (s *userDao) UpdateSignature(userId int64, signature string) error {
	/*
		更新用户个性签名
	*/
	sqlStr := fmt.Sprintf("UPDATE user_info SET signature = '%v' WHERE user_id = '%v'", signature, userId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(int) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	return nil
}

// QueryAllUser 查询所有用户的信息
func (s *userDao) QueryAllUser(userInfos **[]*system.UserInfo) error {
	/*
		查询所有用户的信息
	*/
	sqlStr := "SELECT * FROM user_info"
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(int) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]*systemUserInfo
	_ = json.Unmarshal([]byte(result["data"].(string)), *userInfos)

	return nil

}
