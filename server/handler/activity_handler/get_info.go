package activity_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/activity_service"
)

type getInfoResponse struct {
	common.StatusResponse
	Activities *[]*system.ActivityInfo `json:"activities"`
}

// GetInfoHandler  课外活动Handler
// @Summary      课外活动信息
// @Description  获取学生所有的课外活动信息
// @Tags         课外活动相关接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  getInfoResponse
// @Router       /activity [get]
func GetInfoHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, getInfoResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 调用service服务
	activities, err := activity_service.Server.DoGetInfo(userId)
	if err != nil {
		c.JSON(http.StatusOK, getInfoResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 活动返回成功
	c.JSON(http.StatusOK, getInfoResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "活动返回成功",
		},
		Activities: activities,
	})
}
