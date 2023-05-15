package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/course_service"
	"strconv"
)

type updateResponse struct {
	common.StatusResponse
}

// UpdateHandler  更新课程Handler
// @Summary      更新课程
// @Description  更新课程信息
// @Tags        课程接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  updateResponse
// @Router       /course/update [put]
func UpdateHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, updateResponse{
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

	// 得到courseId
	courseIdString := c.Query("course_id")
	courseId, _ := strconv.ParseInt(courseIdString, 10, 64)

	// 调用service服务
	if err := course_service.Server.DoUpdate(userId, courseId, addCourseRequest); err != nil {
		c.JSON(http.StatusOK, updateResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 成功删除课程
	c.JSON(http.StatusOK, updateResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "成功删除课程",
		},
	})
}
