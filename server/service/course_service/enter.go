package course_service

import "server/model/entity/system"

type CourseServer interface {
	DoTable(userId int64) (*[]*system.CourseInfo, error)
}

type server struct {
}

var Server CourseServer

func init() {
	Server = &server{}
}
