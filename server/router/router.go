package router

import (
	"github.com/gin-gonic/gin"
	"server/handler"
)

func InitRouters() *gin.Engine {
	r := gin.Default()
	rootPath := r.Group("/Student_Management_System")
	//用户注册
	rootPath.POST("user/register", handler.UserRegister)
	//用户登录
	rootPath.POST("user/login", handler.UserLogin)
	return r
}
