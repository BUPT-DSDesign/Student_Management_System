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

	*activities = &[]*system.ActivityInfo{
		{
			ActivityName: "心理扩展活动",
			ActivityId:   21321321,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     2,
			StartTime:   "08:00",
			DurationTime: 1,
			Type:         0,
			Tag:          1,
			Location:     "学生活动中心(南门)",
			IsOnline:     false,
			LocationNode: 23123, // 与上面Location对应
			IsMention:    false,
		},
		{
			ActivityName: "乐跑活动",
			ActivityId:   21321322,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     3,
			StartTime:   "10:00",
			DurationTime: 2,
			Type:         0,
			Tag:          0,
			Location:     "操场",
			IsOnline:     false,
			LocationNode: 23124, // 与上面Location对应
			IsMention:    false,
		},
		{
			ActivityName: "约会",
			ActivityId:   21321323,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     1,
			StartTime:   "20:00",
			DurationTime: 3,
			Type:         0,
			Tag:          1,
			Location:     "北邮锦江酒店",
			IsOnline:     false,
			LocationNode: 23125, // 与上面Location对应
			IsMention:    true,
		},
		{
			ActivityName: "开会",
			ActivityId:   21321324,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     1,
			StartTime:   "09:00",
			DurationTime: 1,
			Type:         0,
			Tag:          1,
			Location:     "经管楼",
			IsOnline:     false,
			LocationNode: 23126, // 与上面Location对应
			IsMention:    false,
		},
		{
			ActivityName: "取快递",
			ActivityId:   21321325,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     4,
			StartTime:   "14:00",
			DurationTime: 1,
			Type:         1,
			Tag:          1,
			Location:     "快递站(顺丰、京东)",
			IsOnline:     false,
			LocationNode: 23127, // 与上面Location对应
			IsMention:    false,
		},
		{
			ActivityName: "取东西",
			ActivityId:   21321326,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     1,
			StartTime:   "15:00",
			DurationTime: 1,
			Type:         1,
			Tag:          1,
			Location:     "学三公寓",
			IsOnline:     false,
			LocationNode: 23128, // 与上面Location对应
			IsMention:    false,
		},
		{
			ActivityName: "买笔",
			ActivityId:   21321327,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     1,
			StartTime:   "19:00",
			DurationTime: 1,
			Type:         1,
			Tag:          1,
			Location:     "物美超市",
			IsOnline:     false,
			LocationNode: 23129, // 与上面Location对应
			IsMention:    false,
		},
		{
			ActivityName: "团建",
			ActivityId:   21321328,
			UserId:       123456789,
			StartWeek:    1,
			StartDay:     1,
			StartTime:   "19:20",
			DurationTime: 1,
			Type:         0,
			Tag:          1,
			Location:     "学生发展中心",
			IsOnline:     false,
			LocationNode: 23130, // 与上面Location对应
			IsMention:    false,
		},
		
	}

	return nil
}
