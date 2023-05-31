package activity_service

import (
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

	//if err := dao.Group.ActivityDao.QueryAllActivityByUserId(f.userId, activities); err != nil {
	//	return err
	//}
	//
	//// 对活动按照时间进行排序
	//var activitySlice my_sort.ActivitySlice
	//for _, activity := range **activities {
	//	activitySlice = append(activitySlice, activity)
	//}
	//my_sort.QuickSort(activitySlice)
	//
	//// 将排序后的活动放回activities
	//for i, activity := range activitySlice {
	//	(**activities)[i] = activity
	//}

	// 随便返回一些活动
	*activities = &[]*system.ActivityInfo{
		{
			ActivityName: "团建",
			ActivityId:   1,
			UserId:       1,
			StartTime:    "第四周-星期1-09:00",
			Type:         0,
			Location:     "1",
			Tag:          "集体活动",
			Frequency:    1,
		},
		{
			ActivityName: "五月鲜花节",
			ActivityId:   2,
			UserId:       1,
			StartTime:    "第三周-星期1-08:00",
			Type:         0,
			Location:     "2",
			Tag:          "集体活动",
			Frequency:    2,
		},
		{
			ActivityName: "约会",
			ActivityId:   2,
			UserId:       1,
			StartTime:    "第三周-星期1-21:00",
			Type:         1,
			Location:     "2",
			Tag:          "临时活动",
			Frequency:    2,
		},
		{
			ActivityName: "运动会",
			ActivityId:   2,
			UserId:       1,
			StartTime:    "第三周-星期1-19:00",
			Type:         0,
			Location:     "2",
			Tag:          "集体活动",
			Frequency:    2,
		},
	}

	return nil
}
