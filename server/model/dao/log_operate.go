package dao

import (
	"encoding/json"
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

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, *logs); err != nil {
		return err
	}

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

	return nil
}
