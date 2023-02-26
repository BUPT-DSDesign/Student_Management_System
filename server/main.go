package main

import (
	"server/router"
)

func main() {
	// userId, _ := utils.GenerateId()
	// tokenString, _ := utils.ReleaseToken(userId)
	// fmt.Printf("userId: %v\n", userId)
	// fmt.Printf("tokenString: %v\n", tokenString)

	r := router.InitRouters()
	r.Run(":8080")
}
