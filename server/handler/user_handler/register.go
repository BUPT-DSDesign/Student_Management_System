package user_handler

import (
	"net/http"
	"server/model/entity/common"
	"server/service/user_service"

	"github.com/gin-gonic/gin"
)

type registerResponse struct {
	common.StatusResponse
	*common.AccessResponse
}

func RegisterHandler(c *gin.Context) {
	// 1.拿到middleware传来的参数

	// 表单数据
	var registerForm common.RegisterRequest
	c.ShouldBind(&registerForm)

	rawPassword, ok1 := c.Get("password")
	rawSalt, ok2 := c.Get("salt")
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, registerResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "中间件中set出错",
			},
		})
		return
	}

	password := rawPassword.(string)
	salt := rawSalt.(string)

	// 2.交给service层处理
	access, err := user_service.Server.DoRegister(registerForm, password, salt)
	if err != nil {
		c.JSON(http.StatusOK, registerResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
	}

	// 注册成功
	c.JSON(http.StatusOK, registerResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "注册成功",
		},
		AccessResponse: access,
	})
}
