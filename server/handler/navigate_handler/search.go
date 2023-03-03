package navigate_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/navigate_service"
)

// 导航响应
type NavigateResponse struct {
	common.StatusResponse
	NodeList []system.Coordinate `json:"node_list"`
}

func RunNavigateHandler(c *gin.Context) {
	var navi system.NavigateRequest
	if err := c.ShouldBind(&navi); err != nil {
		c.JSON(http.StatusOK, NavigateResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "id解析错误",
			},
			NodeList: nil,
		})
		return
	}
	//var nodelist []system.Coordinate
	nodelist, err := navigate_service.NavigateServer.DoNavigation(&navigate_service.Server, navi)
	if err != nil {
		c.JSON(http.StatusOK, NavigateResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
			NodeList: nil,
		})
	}
	c.JSON(http.StatusOK, NavigateResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "成功导航!",
		},
		NodeList: nodelist,
	})

}
