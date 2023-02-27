package user_handler

import (
	"net/http"
	"server/model/entity/common"

	"github.com/gin-gonic/gin"
)

type loginResponse struct {
	common.StatusResponse
	*common.AccessResponse
}

func LoginHandler(c *gin.Context) {

	// 1.拿到middleware传来的参数
	// 2.交给service层处理

	c.JSON(http.StatusOK, loginResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "登录成功",
		},
	})
}
