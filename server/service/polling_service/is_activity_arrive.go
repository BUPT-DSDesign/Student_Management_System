package polling_service

import (
	"errors"
	"server/model/dao"
	"server/model/entity/system"
	"server/utils"
)

type isActivityArriveFlow struct {
	// 包含handler层传来的参数等
	userId    int64
	curTime   string
	timeUnion utils.TimeUnion
}

func newIsActivityArriveFlow(userId int64, curTime string) *isActivityArriveFlow {
	return &isActivityArriveFlow{userId: userId, curTime: curTime}
}

func (s *server) DoIsActivityArrive(userId int64, curTime string) (*system.ActivityInfo, error) {
	return newIsActivityArriveFlow(userId, curTime).do()
}

func (f *isActivityArriveFlow) do() (*system.ActivityInfo, error) {
	var activityInfo *system.ActivityInfo

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&activityInfo); err != nil {
		return nil, err
	}

	return activityInfo, nil
}

// 检验参数
func (f *isActivityArriveFlow) checkNum() error {
	if !utils.CheckTimeFormat(f.curTime) {
		return errors.New("时间格式错误")
	}
	// 将f.curTime转换为timeUnion
	hour, minute := utils.GetHourAndMinute(f.curTime)
	f.timeUnion = utils.TimeUnion{
		Week:   utils.GetWeek(f.curTime),
		Day:    utils.GetDay(f.curTime),
		Hour:   hour,
		Minute: minute,
	}
	return nil
}

func (f *isActivityArriveFlow) run(activityInfo **system.ActivityInfo) error {
	// 先根据userId查出来用户的所有需要提醒的活动
	activityInfos := new([]*system.ActivityInfo)

	if err := dao.Group.ActivityDao.QueryNeedMentionActivity(f.userId, &activityInfos); err != nil {
		return err
	}

	// 然后遍历这些活动，看看是否有活动需要提醒
	for _, activity := range *activityInfos {
		week := utils.GetWeek(activity.StartTime)
		day := utils.GetDay(activity.StartTime)
		hour, minute := utils.GetHourAndMinute(activity.StartTime)
		if activity.Frequency == 0 {
			timeUnion := utils.TimeUnion{
				Week:   week,
				Day:    day,
				Hour:   hour,
				Minute: minute,
			}
			if utils.IsSatisfied(f.timeUnion, timeUnion, activity.AdvanceMentionTime) {
				*activityInfo = activity
				return nil
			}
		} else if activity.Frequency == 1 {
			// 如果是每天则只需要判断时间是否相等
			if f.timeUnion.Week < week {
				continue
			}
			if f.timeUnion.Week == week && f.timeUnion.Day < day {
				continue
			}

			if (f.timeUnion.Hour*60 + f.timeUnion.Minute) == (hour*60 + minute - activity.AdvanceMentionTime) {
				*activityInfo = activity
				return nil
			}
		} else {
			// 如果是每周则需要判断周几和时间是否相等
			if f.timeUnion.Week < week {
				continue
			}

			if f.timeUnion.Day == day && (f.timeUnion.Hour*60+f.timeUnion.Minute) == (hour*60+minute-activity.AdvanceMentionTime) {
				*activityInfo = activity
				return nil
			}
		}
	}
	return nil
}
