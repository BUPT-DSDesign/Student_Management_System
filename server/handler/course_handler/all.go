package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/course_service"
)

type allResponse struct {
	common.StatusResponse
	Courses *[]*system.CourseInfo `json:"course_list"`
}

// AllHandler  所有课Handler
// @Summary      所有课
// @Description  获取所有所有课
// @Tags         课程接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  allResponse
// @Router       /course/all [get]
func AllHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, allResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 调用service服务
	courses, err := course_service.Server.DoAll(userId)
	if err != nil {
		c.JSON(http.StatusOK, allResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 课程表返回成功
	c.JSON(http.StatusOK, allResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "课程列表返回成功",
		},
		Courses: courses,
	})
}
