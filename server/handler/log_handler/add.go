package log_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/log_service"
)

type addResponse struct {
	common.StatusResponse
	LogId int64 `json:"log_id"`
}

// AddHandler 添加日志处理函数
// @Summary      添加日志
// @Description  添加一条日志
// @Tags         添加日志
// @Accept 		 application/json
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Param        create_time query string true "时间"
// @Param        content query string true "内容"
// @Success      200  {object}  addResponse
// @Router       /log/add [get]
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

	// 日志创建时间
	createTime := c.Query("create_time")

	// 日志内容
	content := c.Query("content")

	// 调用服务
	logId, err := log_service.Server.DoAdd(createTime, content, userId)
	if err != nil {
		c.JSON(http.StatusOK, addResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	c.JSON(http.StatusOK, addResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "添加日志成功",
		},
		LogId: logId,
	})
}
