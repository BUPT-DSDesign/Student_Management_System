package activity_service

import (
	"server/model/entity/common"
	"server/model/entity/system"
)

type ActivityServer interface {
	DoGetInfo(userId int64) (*[]*system.ActivityInfo, error)
	DoAdd(userId int64, addActivityRequest common.AddActivityRequest) error
	DoDelete(userId int64, activityId int64) error
}

type server struct {
}

var Server ActivityServer

func init() {
	Server = &server{}
}
