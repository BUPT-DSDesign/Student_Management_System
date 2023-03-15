package user_handler

import (
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/user_service"

	"github.com/gin-gonic/gin"
)

type infoResponse struct {
	common.StatusResponse
	UserInfo *system.UserInfo
}

// InfoHandler 用户信息
// @Summary      用户信息
// @Description  获取用户信息
// @Tags         用户鉴权
// @Accept 		 application/json
// @Produce      application/json
// @Success      200  {object}  infoResponse
// @Router       /user/info[get]
func InfoHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, infoResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 调用服务
	userInfo, err := user_service.Server.DoInfo(userId)
	if err != nil {
		c.JSON(http.StatusOK, infoResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	c.JSON(http.StatusOK, infoResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "获取信息成功",
		},
		UserInfo: userInfo,
	})
}
