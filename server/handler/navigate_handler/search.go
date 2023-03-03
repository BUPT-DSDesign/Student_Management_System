package navigate_handler

import (
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"

	"github.com/gin-gonic/gin"
	"server/service/navigate_service"
)

// 导航响应
type NavigateResponse struct {
	common.StatusResponse
	NodeList []system.Coordinate `json:"node_list"`
}

// RunNavigateHandler 导航
// @Summary      导航
// @Description  获取导航路线
// @Tags         导航功能
// @Accept 		 application/json
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Param        from_id query int true "起始节点ID"
// @Param        des_id query int true "结束节点ID"
// @Success      200  {object}  NavigateResponse
// @Router       /navigate/path [get]
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
	nodelist, err := navigate_service.NavigateServer.DoNavigation(navigate_service.Server, navi)
	if err != nil {
		c.JSON(http.StatusOK, NavigateResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
			NodeList: nil,
		})
		return
	}
	c.JSON(http.StatusOK, NavigateResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "成功导航!",
		},
		NodeList: nodelist,
	})

}
