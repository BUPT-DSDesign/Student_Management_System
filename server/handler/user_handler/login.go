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

// LoginHandler  登录Handler
// @Summary      登录
// @Description  用户登录
// @Tags         用户鉴权
// @Accept 		 application/json
// @Produce      application/json
// @Param		 username query string true "用户名"
// @Param        password query string true "密码"
// @Success      200  {object}  loginResponse
// @Router       /user/login [post]
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
