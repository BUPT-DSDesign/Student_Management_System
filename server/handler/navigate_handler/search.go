package navigate_handler

import (
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"

	"github.com/gin-gonic/gin"
)

func RunNavigateHandler(c *gin.Context) {
	var navi system.NavigateRequest
	if err := c.ShouldBind(&navi); err != nil {
		c.JSON(http.StatusOK, system.NavigateResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "id解析错误",
			},
			NodeList: nil,
		})
		return
	}
	var nodelist []system.Coordinate
	//nodeList := dijkstra.Dijkstra(navi.FromId,navi.DesId,)
	c.JSON(http.StatusOK, system.NavigateResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "成功导航!",
		},
		NodeList: nodelist,
	})

}
