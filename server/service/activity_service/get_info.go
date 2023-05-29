package activity_service

import (
	"server/algorithm/my_sort"
	"server/model/dao"
	"server/model/entity/system"
)

type getInfoFlow struct {
	// 包含handler层传来的参数等
	userId int64
}

func newGetInfoFlow(userId int64) *getInfoFlow {
	return &getInfoFlow{userId: userId}
}

func (s *server) DoGetInfo(userId int64) (*[]*system.ActivityInfo, error) {
	return newGetInfoFlow(userId).do()
}

func (f *getInfoFlow) do() (*[]*system.ActivityInfo, error) {
	var activities *[]*system.ActivityInfo

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&activities); err != nil {
		return nil, err
	}

	return activities, nil
}

// 检验参数
func (f *getInfoFlow) checkNum() error {
	// 这里userId是上层解析过来的, 一定合法
	return nil
}

func (f *getInfoFlow) run(activities **[]*system.ActivityInfo) error {
	/*
		在数据库中根据f.userId查询活动
		调用dao层的CRUD操作
	*/

	if err := dao.Group.ActivityDao.QueryAllActivityByUserId(f.userId, activities); err != nil {
		return err
	}

	// 对活动按照时间进行排序
	var activitySlice my_sort.ActivitySlice
	for _, activity := range **activities {
		activitySlice = append(activitySlice, activity)
	}
	my_sort.QuickSort(activitySlice)

	// 将排序后的活动放回activities
	for i, activity := range activitySlice {
		(**activities)[i] = activity
	}

	return nil
}
