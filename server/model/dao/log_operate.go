package dao

import (
	"encoding/json"
	"errors"
	"fmt"
	"server/model/entity/system"
)

// QueryLogsByUserId 根据用户id查询日志
func (s *logDao) QueryLogsByUserId(UserId int64, logs **[]*system.LogInfo) error {
	/*
		根据用户id查询日志
		需要把数据放到logs中
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM log_info WHERE user_id = '%v'", UserId)
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
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]int64
	_ = json.Unmarshal([]byte(result["data"].(string)), *logs)

	return nil
}

// AddLog 添加日志
func (s *logDao) AddLog(logInfo *system.LogInfo) error {
	sqlStr := fmt.Sprintf("INSERT INTO log_info VALUES('%v', '%v', '%v', '%v')",
		logInfo.LogId,
		logInfo.CreateTime,
		logInfo.Content,
		logInfo.UserId,
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
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	return nil
}

// DeleteLogById 根据id删除日志
func (s *logDao) DeleteLogById(logId int64) error {
	/*
		删除日志
	*/
	sqlStr := fmt.Sprintf("DELETE FROM log_info WHERE log_id = '%v'", logId)
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
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	return nil
}
