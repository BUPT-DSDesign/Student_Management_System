package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/course_service"
)

type addResponse struct {
	common.StatusResponse
}

// AddHandler  添加课程Handler
// @Summary      添加课程
// @Description  管理员添加课程
// @Tags         管理员添加课程相关接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  addResponse
// @Router       /course/add [post]
func AddHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, tableResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 得到addCourseRequest
	var addCourseRequest common.AddCourseRequest

	_ = c.ShouldBindJSON(&addCourseRequest)

	// 调用service服务
	if err := course_service.Server.DoAdd(userId, addCourseRequest); err != nil {
		c.JSON(http.StatusOK, tableResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 添加课程返回成功
	c.JSON(http.StatusOK, tableResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "添加课程成功",
		},
	})
}
