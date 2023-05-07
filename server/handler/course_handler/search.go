package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/course_service"
	"strconv"
)

type searchResponse struct {
	common.StatusResponse
	Courses *[]*system.CourseInfo `json:"course_list"`
}

// SearchHandler  查询课程Handler
// @Summary      查询课程
// @Description  查询课程
// @Tags         查询课程相关接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  searchResponse
// @Router       /course/search [get]
func SearchHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, searchResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 得到isByCourseName以及关键字
	isByCourseNameString := c.Query("is_course_name")
	isByCourseName, _ := strconv.ParseInt(isByCourseNameString, 10, 8)
	keyWord := c.Query("key_word")

	// 调用service服务
	courses, err := course_service.Server.DoSearch(userId, uint8(isByCourseName), keyWord)
	if err != nil {
		c.JSON(http.StatusOK, searchResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 课程表返回成功
	c.JSON(http.StatusOK, searchResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "课程表返回成功",
		},
		Courses: courses,
	})
}
