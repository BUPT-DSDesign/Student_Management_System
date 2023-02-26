package router

import (
	"server/handler/user_handler"

	"github.com/gin-gonic/gin"
)

func InitRouters() *gin.Engine {
	r := gin.Default()
	rootPath := r.Group("/Student_Management_System")

	// 用户路由
	userGroup := rootPath.Group("/user")
	{
		//用户注册
		userGroup.POST("/register", user_handler.RegisterHandler) // 后续使用中间件
		//用户登录
		userGroup.POST("/login", user_handler.LoginHandler)
	}

	return r
}
