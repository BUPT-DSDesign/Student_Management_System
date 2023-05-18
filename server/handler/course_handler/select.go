package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/course_service"
	"strconv"
)

type selectResponse struct {
	common.StatusResponse
}

// SelectHandler  选课Handler
// @Summary      选课
// @Description  学生选课
// @Tags         课程接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  selectResponse
// @Router       /course/select [post]
func SelectHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, selectResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 获取课程id
	courseIdString := c.Query("course_id")
	courseId, _ := strconv.ParseInt(courseIdString, 10, 64)

	// 调用service服务
	if err := course_service.Server.DoSelect(userId, courseId); err != nil {
		c.JSON(http.StatusOK, selectResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 课程表返回成功
	c.JSON(http.StatusOK, selectResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "选课成功",
		},
	})
}
