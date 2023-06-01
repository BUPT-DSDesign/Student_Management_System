package user_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/user_service"
)

type UserInfo struct {
	*system.UserInfo
	// 拥有的课程
	Courses *[]*system.CourseInfo `json:"courses"`
}

type allInfoResponse struct {
	common.StatusResponse
	UserInfos []UserInfo `json:"user_infos"`
}

// AllInfoHandler 所有用户信息
// @Summary      所有用户信息
// @Description  获取所有用户信息
// @Tags         用户接口
// @Accept 		 application/json
// @Produce      application/json
// @Success      200  {object}  allInfoResponse
// @Router       /user/info/all [get]
func AllInfoHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	userId, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, allInfoResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 调用服务
	var userInfos []UserInfo
	users, err := user_service.Server.DoAllInfo(userId)
	if err != nil {
		c.JSON(http.StatusOK, allInfoResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 遍历users, 获取每个用户的课程
	for _, user := range *users {
		courses := new([]*system.CourseInfo)
		// 给courses造几个数据
		if err := dao.Group.CourseDao.QueryCourseByUserId(user.UserId, courses); err != nil {
			c.JSON(http.StatusOK, allInfoResponse{
				StatusResponse: common.StatusResponse{
					StatusCode: 3,
					StatusMsg:  err.Error(),
				},
			})
			return
		}
		userInfos = append(userInfos, UserInfo{
			UserInfo: user,
			Courses:  courses,
		})
	}

	c.JSON(http.StatusOK, allInfoResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "获取所有学生信息成功",
		},
		UserInfos: userInfos,
	})
}
