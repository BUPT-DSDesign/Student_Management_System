package router

import (
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/swaggo/files"
	"github.com/swaggo/gin-swagger"
	_ "server/docs"
	"server/handler/course_handler"
	"server/handler/navigate_handler"
	"server/handler/user_handler"
	"server/middleware"
)

func InitRouters() *gin.Engine {
	r := gin.Default()
	r.Use(cors.New(cors.Config{
		AllowOrigins: []string{"*"},
		AllowMethods: []string{"POST, GET, PUT, DELETE, OPTIONS"},
		// 主要就是下面这两个，带上token就可以了
		AllowHeaders:     []string{"Content-Type, Content-Length, token"},
		ExposeHeaders:    []string{"Access-Control-Allow-Headers, token"},
		AllowCredentials: true,
	}))
	// Swagger路由
	r.GET("/swagger/*any", ginSwagger.WrapHandler(swaggerFiles.Handler))

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
		courseGroup.GET("/table", middleware.JwtAuthMiddleware(), course_handler.TableHandler)
	}
	// 导航路由
	navigateGroup := rootPath.Group("/navigate")
	{
		//获取导航路径
		navigateGroup.GET("/path", middleware.JwtAuthMiddleware(), navigate_handler.RunNavigateHandler)
		//获取建筑物信息
		navigateGroup.GET("/facility", middleware.JwtAuthMiddleware(), navigate_handler.GetFacilityInfoHandler)
	}
	return r
}
