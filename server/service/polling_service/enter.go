package polling_service

import "server/model/entity/system"

type PollingServer interface {
	DoIsCourseArrive(userId int64, curTime string) (*system.CourseInfo, error)
	DoIsActivityArrive(userId int64, curTime string) (*system.ActivityInfo, error)
}
type server struct {
}

var Server PollingServer

func init() {
	Server = &server{}
}
