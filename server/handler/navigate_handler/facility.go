package navigate_handler

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/navigate_service"
)

// 设施查询响应
type FacilityResponse struct {
	common.StatusResponse
	Result []FacilitiyInfo `json:"result"`
}

// 学校设施信息
type FacilitiyInfo struct {
	FacID   int64  `json:"fac_id"`   // 设施ID，设施ID
	FacName string `json:"fac_name"` // 设施名，设施名称
}

func GetFacilityInfoHandler(c *gin.Context) {
	var facReq system.FacilityRequest
	if err := c.ShouldBind(&facReq); err != nil {
		c.JSON(http.StatusOK, FacilityResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "请求解析错误",
			},
			Result: nil,
		})
		return
	}
	result, err := navigate_service.Server.DoGetFacilityList(facReq)
	if err != nil {
		c.JSON(http.StatusOK, FacilityResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
			Result: nil,
		})
		return
	}
	c.JSON(http.StatusOK, FacilityResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "响应成功",
		},
		Result: result,
	})
}
