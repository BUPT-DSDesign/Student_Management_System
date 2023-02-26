package main

import (
	"fmt"
	"server/utils"
)

func main() {
	userId, _ := utils.GenerateId()
	tokenString, _ := utils.ReleaseToken(userId)
	fmt.Printf("userId: %v\n", userId)
	fmt.Printf("tokenString: %v\n", tokenString)

}
