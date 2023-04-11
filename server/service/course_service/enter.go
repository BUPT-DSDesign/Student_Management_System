package course_service

import (
	"server/model/entity/common"
	"server/model/entity/system"
)

type CourseServer interface {
	DoTable(userId int64) (*[]*system.CourseInfo, error)
	DoAdd(userId int64, addCourseRequest common.AddCourseRequest) error
}

type server struct {
}

var Server CourseServer

func init() {
	Server = &server{}
}
