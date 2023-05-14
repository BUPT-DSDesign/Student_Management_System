package polling_service

import "server/model/entity/system"

type PollingServer interface {
	DoIsCourseArrive(userId int64, curTime string) (*system.CourseInfo, error)
}
type server struct {
}

var Server PollingServer

func init() {
	Server = &server{}
}
