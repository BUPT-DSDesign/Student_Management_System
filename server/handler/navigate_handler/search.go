package navigate_handler

import (
	"fmt"
	"net/http"
	"server/model/entity/common"
	"strconv"

	"github.com/gin-gonic/gin"
	"server/service/navigate_service"
)

// 导航响应
type NavigateResponse struct {
	common.StatusResponse
	NodeList [][2]float64 `json:"node_list"`
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
	var navi common.NavigateRequest
	res, ok := c.GetQuery("from_id")
	navi.FromId, _ = strconv.Atoi(res)
	res, ok = c.GetQuery("des_id")
	navi.DesId, _ = strconv.Atoi(res)
	if !ok {
		c.JSON(http.StatusOK, NavigateResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "id解析错误",
			},
			NodeList: nil,
		})
		return
	}
	fmt.Println(navi)
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
