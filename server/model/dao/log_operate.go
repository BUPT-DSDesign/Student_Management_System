package dao

import "server/model/entity/system"

// QueryLogsByUserId 根据用户id查询日志
func (s *logDao) QueryLogsByUserId(userId int64, logs **[]*system.LogInfo) error {
	/*
		根据用户id查询日志
	*/
	return nil
}

// AddLog 添加日志
func (s *logDao) AddLog(logInfo *system.LogInfo) error {
	/*
		添加日志
	*/
	return nil
}

// DeleteLogById 根据id删除日志
func (s *logDao) DeleteLogById(logId int64) error {
	/*
		删除日志
	*/
	return nil
}
