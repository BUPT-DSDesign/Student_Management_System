package log_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/log_service"
)

type infoResponse struct {
	common.StatusResponse
	Logs *[]*system.LogInfo `json:"logs"`
}

// InfoHandler 查询某个用户的日志
// @Summary      查询日志
// @Description  查询日志
// @Tags         日志接口
// @Accept 		 application/json
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  infoResponse
// @Router       /log/info [get]
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
	logs, err := log_service.Server.DoInfo(userId)
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
			StatusMsg:  "日志信息返回成功",
		},
		Logs: logs,
	})
}
