package router

import (
	"net/http"
	_ "server/docs"
	"server/handler/activity_handler"
	"server/handler/course_handler"
	"server/handler/log_handler"
	"server/handler/navigate_handler"
	"server/handler/polling_handler"
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
		// 所有用户信息
		userGroup.GET("/info/all", middleware.JwtAuthMiddleware(), user_handler.AllInfoHandler)
	}

	// 课程路由
	courseGroup := rootPath.Group("/course")
	{
		// 课程表
		courseGroup.GET("/table", middleware.JwtAuthMiddleware(), course_handler.TableHandler)
		// 添加课程
		courseGroup.POST("/add", middleware.JwtAuthMiddleware(), course_handler.AddHandler)
		// 删除课程
		courseGroup.DELETE("/delete", middleware.JwtAuthMiddleware(), course_handler.DeleteHandler)
		// 更新课程
		courseGroup.PUT("/update", middleware.JwtAuthMiddleware(), course_handler.UpdateHandler)
		// 搜索课程
		courseGroup.GET("/search", middleware.JwtAuthMiddleware(), course_handler.SearchHandler)
		// 所有课程
		courseGroup.GET("/all", middleware.JwtAuthMiddleware(), course_handler.AllHandler)
		// 选修课程
		courseGroup.GET("/selective", middleware.JwtAuthMiddleware(), course_handler.SelectiveHandler)
		// 学生选择课程
		courseGroup.POST("/select", middleware.JwtAuthMiddleware(), course_handler.SelectHandler)
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
		// 获取课外活动列表
		activityGroup.GET("/info", middleware.JwtAuthMiddleware(), activity_handler.GetInfoHandler)
		// 添加课外活动
		activityGroup.POST("/add", middleware.JwtAuthMiddleware(), activity_handler.AddHandler)
		// 删除课外活动
		activityGroup.DELETE("/delete", middleware.JwtAuthMiddleware(), activity_handler.DeleteHandler)
	}

	// 日志路由
	logGroup := rootPath.Group("/log")
	{
		// 添加日志
		logGroup.POST("/add", middleware.JwtAuthMiddleware(), log_handler.AddHandler)
		// 获取日志列表
		logGroup.GET("/info", middleware.JwtAuthMiddleware(), log_handler.InfoHandler)
		// 删除日志
		logGroup.DELETE("/delete", middleware.JwtAuthMiddleware(), log_handler.DeleteHandler)
	}

	// 轮询路由
	pollingGroup := rootPath.Group("/polling")
	{
		// 获取课程是否来临
		pollingGroup.GET("/is_course_arrive", middleware.JwtAuthMiddleware(), polling_handler.IsCourseArriveHandler)
		// 获取课外活动是否来临
		pollingGroup.GET("/is_activity_arrive", middleware.JwtAuthMiddleware(), polling_handler.IsActivityArriveHandler)
	}

	return r
}
