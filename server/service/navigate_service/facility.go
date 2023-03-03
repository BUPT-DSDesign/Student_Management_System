package navigate_service

import (
	"errors"
<<<<<<< HEAD
	"server/model/entity/common"
	"server/model/entity/system"
)

func (s *server) DoGetFacilityList(request common.FacilityRequest) (*[]*system.FacilityInfo, error) {
=======
	"server/model/entity/system"
)

func (s *server) DoGetFacilityList(request system.FacilityRequest) ([]system.FacilitiyInfo, error) {
>>>>>>> cc128dcc33e6533ef96a77e26ec019ffe3127576
	if request.IsSpecific {
		//如果是精确查询某个建筑物
		if request.FacID != nil {
			if *request.FacID < 0 || *request.FacID >= int64(len(s.tempNodeList)) {
				//建筑物ID越界
				return nil, errors.New("不存在这样的建筑物id")
			}
<<<<<<< HEAD
			var res []*system.FacilityInfo
			res = append(res, &system.FacilityInfo{
=======
			var res []system.FacilitiyInfo
			res = append(res, system.FacilitiyInfo{
>>>>>>> cc128dcc33e6533ef96a77e26ec019ffe3127576
				FacID:   *request.FacID,
				FacName: s.tempNodeList[*request.FacID].Name,
			})
			return &res, nil
		} else if request.FacName != nil {
			// todo 等待实现名字匹配查找
			return nil, nil
		} else {
			return nil, errors.New("精确查找需要提供建筑物ID或建筑物名称")
		}
	} else {
		//不是精确匹配查找,返回建筑物列表
		//边界条件判断
		if request.Offset < 0 || request.Offset > int64(len(s.tempNodeList)) || request.Offset+request.Count > int64(len(s.tempNodeList)) || request.Count <= 0 {
			return nil, errors.New("查询offset或count错误")
		}
<<<<<<< HEAD
		var res []*system.FacilityInfo
		for i := request.Offset; i < request.Offset+request.Count; i++ {
			res = append(res, &system.FacilityInfo{
=======
		var res []system.FacilitiyInfo
		for i := request.Offset; i < request.Offset+request.Count; i++ {
			res = append(res, system.FacilitiyInfo{
>>>>>>> cc128dcc33e6533ef96a77e26ec019ffe3127576
				FacID:   int64(s.tempNodeList[i].Index),
				FacName: s.tempNodeList[i].Name,
			})
		}
		return &res, nil
	}
}
