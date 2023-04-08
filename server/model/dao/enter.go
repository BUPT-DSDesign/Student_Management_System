package dao

import "server/model/entity/system"

// UserDao 用户数据库操作对象
type UserDao interface {
	AddUser(userInfo *system.UserInfo) error
	QueryUserById(userId int64, userInfo **system.UserInfo) error
	QueryUserByName(username string, userInfo **system.UserInfo) error
	UpdateUserAvatar(userId int64, avatarUrl string) error
	UpdateSignature(userId int64, signature string) error
}

// CourseDao 课程数据库操作对象
type CourseDao interface {
}

// ActivityDao 活动数据库操作对象
type ActivityDao interface {
}

// LogDao 日志数据库操作对象
type LogDao interface {
	QueryLogsByUserId(userId int64, logs **[]*system.LogInfo) error
	AddLog(logInfo *system.LogInfo) error
	DeleteLogById(logId int64) error
}

// GroupDao 数据库操作对象集合
type GroupDao struct {
	UserDao     UserDao
	CourseDao   CourseDao
	ActivityDao ActivityDao
	LogDao      LogDao
}

var Group GroupDao

func init() {
	Group = GroupDao{
		UserDao:     &userDao{},
		CourseDao:   &courseDao{},
		ActivityDao: &activityDao{},
		LogDao:      &logDao{},
	}
}

type userDao struct{}
type courseDao struct{}
type activityDao struct{}
type logDao struct{}
