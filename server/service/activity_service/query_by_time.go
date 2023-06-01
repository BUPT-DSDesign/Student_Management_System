package activity_service

import (
	"server/model/dao"
	"server/model/entity/system"
	"server/utils"
)

type queryFlow struct {
	// 包含handler层传来的参数等
	userId   int64
	fromTime string
	toTime   string
}

func newQueryFlow(userId int64, fromTime string, toTime string) *queryFlow {
	return &queryFlow{userId: userId, fromTime: fromTime, toTime: toTime}
}

func (s *server) DoQueryByTime(userId int64, fromTime string, toTime string) (*[]*system.ActivityInfo, error) {
	return newQueryFlow(userId, fromTime, toTime).do()
}

func (f *queryFlow) do() (*[]*system.ActivityInfo, error) {
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
func (f *queryFlow) checkNum() error {
	// 这里userId是上层解析过来的, 一定合法
	return nil
}

func (f *queryFlow) run(activities **[]*system.ActivityInfo) error {
	// 在数据库中根据f.userId查询活动
	activitiesInfo := new([]*system.ActivityInfo)
	if err := dao.Group.ActivityDao.QueryAllActivityByUserId(f.userId, &activitiesInfo); err != nil {
		return err
	}
	// 将满足时间段的活动放入activities
	for _, activity := range *activitiesInfo {
		if activity.Type == 0 {
			endTime := utils.AddByDuration(activity.StartTime, 60)
			if utils.IsLessThan(f.fromTime, activity.StartTime) && utils.IsLessThan(endTime, f.toTime) {
				**activities = append(**activities, activity)
			}
		} else {
			if utils.IsLessThan(f.fromTime, activity.StartTime) && utils.IsLessThan(activity.StartTime, f.toTime) {
				**activities = append(**activities, activity)
			}
		}
	}

	return nil
}
