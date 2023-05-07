package router

import (
	"net/http"
	_ "server/docs"
	"server/handler/activity_handler"
	"server/handler/course_handler"
	"server/handler/log_handler"
	"server/handler/navigate_handler"
	"server/handler/user_handler"
	"server/middleware"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	swaggerFiles "github.com/swaggo/files"
	ginSwagger "github.com/swaggo/gin-swagger"
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
	r.StaticFS("/static", http.Dir("./static"))

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
		// 用户信息
		userGroup.GET("/info", middleware.JwtAuthMiddleware(), user_handler.InfoHandler)
		// 用户上传头像
		userGroup.POST("/upload_avatar", middleware.JwtAuthMiddleware(), user_handler.UploadAvatarHandler)
		// 编辑个性签名
		userGroup.PUT("/edit_signature", middleware.JwtAuthMiddleware(), user_handler.EditSignatureHandler)
	}

	// 课程路由
	courseGroup := rootPath.Group("/course")
	{
		// 课程表
		courseGroup.GET("/table", middleware.JwtAuthMiddleware(), course_handler.TableHandler)
		// 添加课程
		courseGroup.POST("/add", middleware.JwtAuthMiddleware(), course_handler.AddHandler)
		// 删除课程
	}

	// 导航路由
	navigateGroup := rootPath.Group("/navigate")
	{
		// 获取导航路径
		navigateGroup.GET("/path", middleware.JwtAuthMiddleware(), navigate_handler.RunNavigateHandler)
		// 获取建筑物信息
		navigateGroup.GET("/facility", middleware.JwtAuthMiddleware(), navigate_handler.GetFacilityInfoHandler)
		// 旅行商问题
		navigateGroup.GET("/tsp", middleware.JwtAuthMiddleware(), navigate_handler.TSPHandler)
	}

	// 课外活动以及临时事务路由
	activityGroup := rootPath.Group("/activity")
	{
		activityGroup.GET("/info", middleware.JwtAuthMiddleware(), activity_handler.GetInfoHandler)
	}

	// 日志路由
	logGroup := rootPath.Group("/log")
	{
		logGroup.GET("/add", middleware.JwtAuthMiddleware(), log_handler.AddHandler)
		logGroup.GET("/info", middleware.JwtAuthMiddleware(), log_handler.InfoHandler)
		logGroup.DELETE("/delete", middleware.JwtAuthMiddleware(), log_handler.DeleteHandler)
	}

	return r
}
