package user_service

import (
	"github.com/gin-gonic/gin"
	"mime/multipart"
	"server/model/entity/common"
)

type UserServer interface {
	DoRegister(loginForm common.RegisterRequest, password string, salt string) (*common.AccessResponse, error)
	DoLogin(loginForm common.LoginRequest) (*common.AccessResponse, error)
	DoUploadAvatar(userId int64, file *multipart.FileHeader, c *gin.Context) (*string, error)
}

type server struct {
}

var Server UserServer

func init() {
	Server = &server{}
}
