package activity_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/activity_service"
	"strconv"
)

type deleteResponse struct {
	common.StatusResponse
}

// DeleteHandler  删除活动Handler
// @Summary      删除活动信息
// @Description  删除活动信息
// @Tags         删除活动相关接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  deleteResponse
// @Router       /activity/delete [delete]
func DeleteHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, deleteResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 获取活动id
	activityIdString := c.Query("activity_id")
	activityId, _ := strconv.ParseInt(activityIdString, 10, 64)

	// 调用service服务
	if err := activity_service.Server.DoDelete(userId, activityId); err != nil {
		c.JSON(http.StatusOK, deleteResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 删除活动成功
	c.JSON(http.StatusOK, deleteResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "删除活动成功",
		},
	})
}
