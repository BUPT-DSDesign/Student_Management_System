package user_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/user_service"
)

type editSignatureResponse struct {
	common.StatusResponse
}

// EditSignatureHandler 编辑个性签名
// @Summary      个性签名
// @Description  编辑个性签名
// @Tags         用户鉴权
// @Accept 		 application/json
// @Produce      application/json
// @Success      200  {object}  editSignatureResponse
// @Router       /user/edit_signature[put]

func EditSignatureHandler(c *gin.Context) {
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

	signature := c.Query("signature")

	// 调用服务
	if err := user_service.Server.DoEditSignature(userId, signature); err != nil {
		c.JSON(http.StatusOK, editSignatureResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 编辑成功
	c.JSON(http.StatusOK, editSignatureResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "编辑成功",
		},
	})
}
