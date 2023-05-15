package navigate_handler

import (
	"encoding/json"
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/service/navigate_service"
	"strconv"
)

type tspResponse struct {
	common.StatusResponse
	NodeList *[][2]float64 `json:"node_list"` // 途径节点的经纬度
	PassList *[][2]float64 `json:"pass_list"` // 用户选择的目标点的经纬度，用户在地图上显示
}

// TSPHandler 旅行商处理函数
// @Summary      旅行商
// @Description  解决旅行商问题
// @Tags         导航接口
// @Accept 		 application/json
// @Produce      application/json
// @Security 	 ApiKeyAuth
// @Param        start_id query int true "开始节点ID"
// @Param        des_ids query []int true "途径节点集合"
// @Success      200  {object}  tspResponse
// @Router       /navigate/tsp [get]
func TSPHandler(c *gin.Context) {
	rawUserId, ok1 := c.Get("user_id")
	_, ok2 := rawUserId.(int64)
	if !ok1 || !ok2 {
		c.JSON(http.StatusOK, tspResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "解析id出错",
			},
		})
		return
	}

	// 起始节点编号
	startIdString := c.Query("start_id")
	startId, _ := strconv.Atoi(startIdString)

	// 途径节点编号集合
	passIdsString := c.Query("pass_ids")
	passIdsMap := make(map[string]int, 0)
	_ = json.Unmarshal([]byte(passIdsString), &passIdsMap)
	var passIds []int
	for _, v := range passIdsMap {
		passIds = append(passIds, v)
	}

	// 调用服务
	nodeList, passList, err := navigate_service.Server.DoTSP(startId, passIds)
	if err != nil {
		c.JSON(http.StatusOK, tspResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}

	// 返回路径
	c.JSON(http.StatusOK, tspResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "路径返回成功",
		},
		NodeList: nodeList,
		PassList: passList,
	})
}
