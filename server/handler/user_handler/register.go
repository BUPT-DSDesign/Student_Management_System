package user_handler

import (
	"net/http"
	"server/model/entity/common"

	"github.com/gin-gonic/gin"
)

type registerResponse struct {
	common.StatusResponse
	*common.AcessResponse
}

func RegisterHandler(c *gin.Context) {
	// 用户名
	// 密码
	// 1.拿到middleware传来的参数
	// 2.交给service层处理
	
	c.JSON(http.StatusOK, registerResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg: "注册成功",
		},
	})
}