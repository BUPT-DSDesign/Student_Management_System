package router

import (
	"server/handler/course_handler"
	"server/handler/navigate_handler"
	"server/handler/user_handler"
	"server/middleware"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
)

func InitRouters() *gin.Engine {
	r := gin.Default()
	r.Use(cors.Default())
	rootPath := r.Group("/Student_Management_System")

	// 用户路由
	userGroup := rootPath.Group("/user")
	{
		//用户注册
		userGroup.POST("/register", middleware.ShaMiddleware(), user_handler.RegisterHandler)
		//用户登录
		userGroup.POST("/login", user_handler.LoginHandler)
	}

	// 课程路由
	courseGroup := rootPath.Group("/course")
	{
		// 课程表
		courseGroup.GET("/table/", middleware.JwtAuthMiddleware(), course_handler.TableHandler)
	}
	// 导航路由
	navigateGroup := rootPath.Group("/navigate")
	{
		//获取导航路径
		navigateGroup.GET("/run", middleware.JwtAuthMiddleware(), navigate_handler.RunNavigateHandler)
	}
	return r
}
