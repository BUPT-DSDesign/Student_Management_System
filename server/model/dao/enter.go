package dao

import (
	"server/model/entity/common"
	"server/model/entity/system"
)

// UserDao 用户数据库操作对象
type UserDao interface {
	AddUser(userInfo *system.UserInfo) error
	QueryUserById(userId int64, userInfo **system.UserInfo) error
	QueryUserByName(username string, userInfo **system.UserInfo) error
	UpdateUserAvatar(userId int64, avatarUrl string) error
	UpdateSignature(userId int64, signature string) error
	QueryAllUser(userInfos **[]*system.UserInfo) error
}

// CourseDao 课程数据库操作对象
type CourseDao interface {
	AddCourse(courseInfo *system.CourseInfo) error
	DeleteCourse(courseId int64) error
	UpdateCourse(courseId int64, newCourseInfo *common.AddCourseRequest) error
	QueryCourseByUserId(userId int64, courses *[]*system.CourseInfo) error
	QueryCompulsoryCourse(courses **[]*system.CourseInfo) error
	QueryElectiveCourse(userId int64, courses **[]*system.CourseInfo) error
	QueryCourseByName(courseName string, courses **[]*system.CourseInfo) error
	QueryCourseByClassroom(classroom string, courses **[]*system.CourseInfo) error
	QuerySectionListById(courseId int64, sectionList *[]int) error
	QueryWeekScheduleById(courseId int64, weekSchedule *[]int) error
	QueryAllSelectiveCourse(courses **[]*system.CourseInfo) error
	QueryAllCourse(courses **[]*system.CourseInfo) error
	JudgeIsStudentSelectCourse(userId int64, courseId int64) bool
	SelectCourse(userId int64, courseId int64) error
	QueryCourseByWeek(week int, courseIds *[]int64) error
	QueryCourseById(courseId int64, course *system.CourseInfo) error
	QueryCourseBySection(section int, courseIds *[]int64) error
}

// ActivityDao 活动数据库操作对象
type ActivityDao interface {
	AddActivity(activityInfo *system.ActivityInfo) error
	DeleteActivity(activityId int64) error
	QueryNeedMentionActivity(userId int64, activities **[]*system.ActivityInfo) error
	QueryAllActivityByUserId(userId int64, activities **[]*system.ActivityInfo) error
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
