package course_service

import (
	"server/model/entity/common"
	"server/model/entity/system"
)

type CourseServer interface {
	DoTable(userId int64) (*[]*system.CourseInfo, error)
	DoAdd(userId int64, addCourseRequest common.AddCourseRequest) error
	DoDelete(userId int64, courseId int64) error
	DoUpdate(userId int64, courseId int64, updateCourseRequest common.AddCourseRequest) error
	DoSearch(userId int64, isByCourseName uint8, keyWord string) (*[]*system.CourseInfo, error)
	DoAll(userId int64) (*[]*system.CourseInfo, error)
	DoSelective(userId int64) (*[]*system.CourseInfo, error)
	DoSelect(userId int64, courseId int64) error
}

type server struct {
}

var Server CourseServer

func init() {
	Server = &server{}
}
