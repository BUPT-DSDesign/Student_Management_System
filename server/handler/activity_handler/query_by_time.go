package activity_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/activity_service"
)

type queryResponse struct {
	common.StatusResponse
	Activities *[]*system.ActivityInfo `json:"activities"`
}

// QueryHandler  课外活动Handler
// @Summary      查询课外活动
// @Description  通过时间段查询学生的课外活动信息
// @Tags         活动接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  queryResponse
// @Router       /activity/query [get]
func QueryHandler(c *gin.Context) {
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

	// 获取时间段
	fromTime := c.Query("from_time")
	toTime := c.Query("to_time")

	// 调用service服务
	activities, err := activity_service.Server.DoQueryByTime(userId, fromTime, toTime)
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
