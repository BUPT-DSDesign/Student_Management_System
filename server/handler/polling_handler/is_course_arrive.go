package polling_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/polling_service"
)

type isCourseArriveResponse struct {
	common.StatusResponse
	IsArrive   bool               `json:"is_arrive"`
	CourseInfo *system.CourseInfo `json:"course_info"`
}

// IsCourseArriveHandler 课程是否来临
// @Summary      课程是否来临
// @Description  课程是否来临
// @Tags         用户鉴权
// @Accept 		 application/json
// @Produce      application/json
// @Success      200  {object}  isCourseArriveResponse
// @Router       /polling/is_course_arrive [get]
func IsCourseArriveHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, isCourseArriveResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
			IsArrive: false,
		})
		return
	}

	// 获取curTime参数
	curTime := c.Query("cur_time")

	// 调用服务
	courseInfo, err := polling_service.Server.DoIsCourseArrive(userId, curTime)
	if err != nil {
		c.JSON(http.StatusOK, isCourseArriveResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
			IsArrive: false,
		})
		return
	}

	c.JSON(http.StatusOK, isCourseArriveResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "轮询成功",
		},
		IsArrive:   courseInfo != nil,
		CourseInfo: courseInfo,
	})
}
