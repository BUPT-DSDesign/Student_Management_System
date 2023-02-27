package middleware

import (
	"crypto/sha1"
	"encoding/hex"
	"net/http"
	"server/model/entity/common"

	"github.com/gin-gonic/gin"
)

// 使用sha1对密码加密
func sha(s string) (string, error) {
	hash := sha1.New()
	if _, err := hash.Write([]byte(s)); err != nil {
		return "", err
	}

	return hex.EncodeToString(hash.Sum(nil)), nil
}

func ShaMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		rawPassword := c.PostForm("password")

		// 密码加密
		password, err := sha(rawPassword)
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
