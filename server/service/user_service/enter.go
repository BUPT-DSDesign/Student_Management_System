package user_service

import (
	"mime/multipart"
	"server/model/entity/common"
	"server/model/entity/system"

	"github.com/gin-gonic/gin"
)

type UserServer interface {
	DoRegister(loginForm common.RegisterRequest, password string, salt string) (*common.AccessResponse, error)
	DoLogin(loginForm common.LoginRequest) (*common.AccessResponse, error)
	DoInfo(userId int64) (*system.UserInfo, error)
	DoUploadAvatar(userId int64, file *multipart.FileHeader, c *gin.Context) (*string, error)
	DoEditSignature(userId int64, signature string) error
	DoAllInfo(userId int64) (*[]*system.UserInfo, error)
}

type server struct {
}

var Server UserServer

func init() {
	Server = &server{}
}
