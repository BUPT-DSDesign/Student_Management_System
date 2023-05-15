package user_handler

import (
	"net/http"
	"server/model/entity/common"
	"server/service/user_service"

	"github.com/gin-gonic/gin"
)

type uploadAvatarResponse struct {
	common.StatusResponse
	AvatarUrl *string `json:"avatar_url"`
}

// UploadAvatarHandler 上传头像
// @Summary      上传头像
// @Description  用户上传头像
// @Tags         用户接口
// @Accept 		 application/json
// @Produce      application/json
// @Success      200  {object}  uploadAvatarResponse
// @Router       /user/upload_avatar [post]
func UploadAvatarHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, uploadAvatarResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	file, err := c.FormFile("file")
	if err != nil {
		c.JSON(http.StatusOK, uploadAvatarResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 调用服务
	avatarUrl, err := user_service.Server.DoUploadAvatar(userId, file, c)
	if err != nil {
		c.JSON(http.StatusOK, uploadAvatarResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	c.JSON(http.StatusOK, uploadAvatarResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "上传头像成功",
		},
		AvatarUrl: avatarUrl,
	})
}
