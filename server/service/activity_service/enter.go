package activity_service

import "server/model/entity/system"

type ActivityServer interface {
	DoGetInfo(userId int64) (*[]*system.ActivityInfo, error)
}

type server struct {
}

var Server ActivityServer

func init() {
	Server = &server{}
}
