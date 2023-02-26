package router

import (
	"server/handler"

	"github.com/gin-gonic/gin"
)

func InitRouters() *gin.Engine {
	r := gin.Default()
	rootPath := r.Group("/Student_Management_System")

	// 用户路由
	userGroup := rootPath.Group("/user")
	{
		//用户注册
		userGroup.POST("/register", handler.UserRegister) // 后续使用中间件
		//用户登录
		userGroup.POST("/login", handler.UserLogin)
	}

	return r
}
