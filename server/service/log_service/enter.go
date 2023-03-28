package log_service

import "server/model/entity/system"

type LogServer interface {
	DoAdd(createTime string, content string, userId int64) (int64, error)
	DoInfo(userId int64) (*[]*system.LogInfo, error)
}

type server struct {
}

var Server LogServer

func init() {
	Server = &server{}
}
