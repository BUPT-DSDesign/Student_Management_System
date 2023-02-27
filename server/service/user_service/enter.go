package user_service

import "server/model/entity/common"

type UserServer interface {
	DoRegister(loginForm common.RegisterRequest, password string, salt string) (*common.AccessResponse, error)
}

type server struct {
}

var Server UserServer

func init() {
	Server = &server{}
}
