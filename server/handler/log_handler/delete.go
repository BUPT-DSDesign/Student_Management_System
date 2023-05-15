package log_handler

import (
	"encoding/json"
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/log_service"
)

type deleteResponse struct {
	common.StatusResponse
}

// DeleteHandler 删除日志处理函数
// @Summary      删除日志
// @Description  删除一条日志
// @Tags         日志接口
// @Accept 		 application/json
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Param        log_id query string true "日志id"
// @Success      200  {object}  deleteResponse
// @Router       /log/delete [get]
func DeleteHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	_, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, deleteResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 日志id
	logIdsString := c.Query("log_ids")
	logIdsMap := make(map[string]int64, 0)
	_ = json.Unmarshal([]byte(logIdsString), &logIdsMap)
	var logIds []int64
	for _, v := range logIdsMap {
		logIds = append(logIds, v)
	}

	// 调用服务
	if err := log_service.Server.DoDelete(logIds); err != nil {
		c.JSON(http.StatusOK, deleteResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	c.JSON(http.StatusOK, deleteResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "删除日志成功",
		},
	})
}
