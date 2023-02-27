package middleware

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/utils"
)

func ShaMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		rawPassword := c.PostForm("password")
		// 密码加密
		password, err := utils.Sha1(rawPassword)
		if err != nil {
			c.JSON(http.StatusOK, common.StatusResponse{
				StatusCode: 403,
				StatusMsg:  err.Error(),
			})
			c.Abort()
			return
		}

		c.Set("password", password)
		c.Next()
	}
}
