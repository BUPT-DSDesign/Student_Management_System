package user_handler

import (
	"net/http"
	"server/model/entity/common"
	"server/service/user_service"

	"github.com/gin-gonic/gin"
)

type loginResponse struct {
	common.StatusResponse
	*common.AccessResponse
}

func LoginHandler(c *gin.Context) {

	// 1.拿到middleware传来的参数
	var loginForm common.LoginRequest
	_ = c.ShouldBind(&loginForm)

	// 2.交给service层处理
	access, err := user_service.Server.DoLogin(loginForm)
	if err != nil {
		c.JSON(http.StatusOK, loginResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 登录成功
	c.JSON(http.StatusOK, loginResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "登录成功",
		},
		AccessResponse: access,
	})
}
