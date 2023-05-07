package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/course_service"
	"strconv"
)

type deleteResponse struct {
	common.StatusResponse
}

// DeleteHandler  删除课程Handler
// @Summary      删除课程
// @Description  根据id删除课程
// @Tags         课程表相关接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  deleteResponse
// @Router       /course/delete [delete]
func DeleteHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, deleteResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 得到课程id
	courseIdString := c.Query("course_id")
	courseId, _ := strconv.ParseInt(courseIdString, 10, 64)

	// 调用service服务
	if err := course_service.Server.DoDelete(userId, courseId); err != nil {
		c.JSON(http.StatusOK, deleteResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 成功删除课程
	c.JSON(http.StatusOK, deleteResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "成功删除课程",
		},
	})
}
