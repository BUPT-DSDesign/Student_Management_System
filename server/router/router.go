package router

import (
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"server/handler/user_handler"
)

func InitRouters() *gin.Engine {
	r := gin.Default()
	r.Use(cors.Default())
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
