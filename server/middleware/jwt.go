package middleware

import (
	"net/http"
	"server/model/entity/common"
	"server/utils"
	"time"

	"github.com/gin-gonic/gin"
)

// jwt鉴权
func JwtAuthMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		tokenString := c.Query("token") // token串
		// 用户不存在
		if tokenString == "" {
			c.JSON(http.StatusOK, common.StatusResponse{
				StatusCode: 401,
				StatusMsg:  "用户不存在",
			})
			c.Abort()
			return
		}

		claim, err := utils.ParaseToken(tokenString)
		// 无效token
		if err != nil {
			c.JSON(http.StatusOK, common.StatusResponse{
				StatusCode: 403,
				StatusMsg:  err.Error(),
			})
			c.Abort()
			return
		}

		// token过期
		if time.Now().Unix() > claim.ExpiresAt.Time.Unix() {
			c.JSON(http.StatusOK, common.StatusResponse{
				StatusCode: 401,
				StatusMsg:  "token过期",
			})
			c.Abort()
			return
		}
		c.Set("user_id", claim.UserId)
		c.Next()
	}
}
