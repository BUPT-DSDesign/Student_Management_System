package main

import (
	"server/router"
)

// @title           Student Management System API
// @version         alpha 1.0
// @description     A application for Data Structure Lesson
// @termsOfService  http://swagger.io/terms/

// @contact.name   API Support
// @contact.url    http://www.swagger.io/support
// @contact.email  support@swagger.io

// @license.name  Apache 2.0
// @license.url   http://www.apache.org/licenses/LICENSE-2.0.html

// @host      localhost:8080
// @BasePath  /Student_Management_System

// @securityDefinitions.apikey  ApiKeyAuth
// @in header
// @name token
// @externalDocs.description  OpenAPI
// @externalDocs.url          https://swagger.io/resources/open-api/
func main() {
	//section := []int{1, 2, 3, 5, 6, 8, 9}
	//for _, v := range utils.SplitSectionList(section) {
	//	fmt.Printf("section: %v, %v\n", v[0], v[1])
	//}

	// userId, _ := utils.GenerateId()
	// tokenString, _ := utils.ReleaseToken(userId)
	// fmt.Printf("userId: %v\n", userId)
	// fmt.Printf("tokenString: %v\n", tokenString)
	r := router.InitRouters()
	//配置Https服务
	//r.Use(middleware.TlsHandler(8080))
	//_ = r.RunTLS(":8080", "./secret/test.pem", "./secret/test.key")
	_ = r.Run(":8080")

}
