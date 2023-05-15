package polling_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/polling_service"
)

type isActivityArriveResponse struct {
	common.StatusResponse
	IsArrive     bool                 `json:"is_arrive"`
	ActivityInfo *system.ActivityInfo `json:"activity_info"`
}

// IsActivityArriveHandler 活动是否来临, 用于活动闹钟
// @Summary      活动是否来临
// @Description  活动是否来临
// @Tags         轮询接口
// @Accept 		 application/json
// @Produce      application/json
// @Success      200  {object}  isActivityArriveResponse
// @Router       /polling/is_activity_arrive [get]
func IsActivityArriveHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, isActivityArriveResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
			IsArrive: false,
		})
		return
	}

	// 获取curTime参数
	curTime := c.Query("cur_time")

	// 调用服务
	activityInfo, err := polling_service.Server.DoIsActivityArrive(userId, curTime)
	if err != nil {
		c.JSON(http.StatusOK, isActivityArriveResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
			IsArrive: false,
		})
		return
	}

	c.JSON(http.StatusOK, isActivityArriveResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "轮询成功",
		},
		IsArrive:     activityInfo != nil,
		ActivityInfo: activityInfo,
	})
}
