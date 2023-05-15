package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/course_service"
)

type tableResponse struct {
	common.StatusResponse
	Courses *[]*system.CourseInfo `json:"course_list"`
}

// TableHandler  课程表Handler
// @Summary      课程表
// @Description  获取学生所有的课程表信息
// @Tags         课程接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  tableResponse
// @Router       /course/table [get]
func TableHandler(c *gin.Context) {
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

	// 调用service服务
	courses, err := course_service.Server.DoTable(userId)
	if err != nil {
		c.JSON(http.StatusOK, tableResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 课程表返回成功
	c.JSON(http.StatusOK, tableResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "课程表返回成功",
		},
		Courses: courses,
	})
}
