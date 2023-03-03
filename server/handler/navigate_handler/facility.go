package navigate_handler

import (
	"net/http"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/service/navigate_service"

	"github.com/gin-gonic/gin"
)

// 设施查询响应
type facilityResponse struct {
	common.StatusResponse
<<<<<<< HEAD
	result *[]*system.FacilityInfo `json:"result"`
=======
	Result []system.FacilitiyInfo `json:"result"`
>>>>>>> cc128dcc33e6533ef96a77e26ec019ffe3127576
}

func GetFacilityInfoHandler(c *gin.Context) {
	var facReq common.FacilityRequest
	if err := c.ShouldBind(&facReq); err != nil {
		c.JSON(http.StatusOK, facilityResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 1,
				StatusMsg:  "请求解析错误",
			},
		})
		return
	}

	//
	result, err := navigate_service.Server.DoGetFacilityList(facReq)
	if err != nil {
		c.JSON(http.StatusOK, facilityResponse{
			StatusResponse: common.StatusResponse{
				StatusCode: 2,
				StatusMsg:  err.Error(),
			},
		})
		return
	}
	c.JSON(http.StatusOK, facilityResponse{
		StatusResponse: common.StatusResponse{
			StatusCode: 0,
			StatusMsg:  "响应成功",
		},
		result: result,
	})
}
