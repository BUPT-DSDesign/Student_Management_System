package course_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/course_service"
)

type CourseInfo struct {
	*system.CourseInfo
	IsSelected bool `json:"is_selected"`
}

type selectiveResponse struct {
	common.StatusResponse
	CourseInfos *[]CourseInfo `json:"course_list"`
}

// SelectiveHandler  选修课Handler
// @Summary      选修课
// @Description  获取所有选修课
// @Tags         课程接口
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Success      200  {object}  selectiveResponse
// @Router       /course/selective [get]
func SelectiveHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, selectiveResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 调用service服务
	courses, err := course_service.Server.DoSelective(userId)
	if err != nil {
		c.JSON(http.StatusOK, selectiveResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	var courseInfos []CourseInfo
	for _, course := range *courses {
		courseInfos = append(courseInfos, CourseInfo{
			CourseInfo: course,
			IsSelected: dao.Group.CourseDao.JudgeIsStudentSelectCourse(userId, course.CourseId),
		})
	}

	// 课程表返回成功
	c.JSON(http.StatusOK, selectiveResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "选修课列表返回成功",
		},
		CourseInfos: &courseInfos,
	})
}
