package middleware

import (
	"net/http"
	"server/model/entity/common"
	"server/utils"

	"github.com/gin-gonic/gin"
)

// 返回错误, 后续可能移到其他位置
func reportErr(c *gin.Context, err error) {
	c.JSON(http.StatusOK, common.StatusResponse{
		StatusCode: 403,
		StatusMsg:  err.Error(),
	})
}

func ShaMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		rawPassword := c.PostForm("password")
		// 密码sha1加密
		sha1Password, err := utils.Sha1(rawPassword)
		if err != nil {
			reportErr(c, err)
			c.Abort()
			return
		}

		salt, err := utils.GenerateSalt(10) // 盐, 默认盐的长度为10, 后期可以写到const里面
		if err != nil {
			reportErr(c, err)
			c.Abort()
			return
		}

		unitePassword := sha1Password + salt
		password, err := utils.Sha256(unitePassword)
		if err != nil {
			reportErr(c, err)
			c.Abort()
			return
		}

		c.Set("password", password)
		c.Set("salt", salt)
		c.Next()
	}
}
