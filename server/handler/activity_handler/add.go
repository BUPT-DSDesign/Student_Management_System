package activity_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/activity_service"
)

type addResponse struct {
	common.StatusResponse
}

// AddHandler  增加活动Handler
// @Summary      增加活动信息
// @Description  增加活动信息
// @Tags         活动接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  addResponse
// @Router       /activity/add [post]
func AddHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, addResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 获取addActivityRequest
	var addActivityRequest common.AddActivityRequest
	_ = c.ShouldBindJSON(&addActivityRequest)

	// 调用service服务
	if err := activity_service.Server.DoAdd(userId, addActivityRequest); err != nil {
		c.JSON(http.StatusOK, addResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 添加活动成功go
	c.JSON(http.StatusOK, addResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "添加活动成功",
		},
	})
}
