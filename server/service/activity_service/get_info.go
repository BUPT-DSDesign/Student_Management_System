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
			StartDay:     1,
			StartTime:   "8:00",
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
			StartDay:     2,
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
			StartDay:     3,
			StartTime:   "20:00",
			DurationTime: 3,
			Type:         1,
			Tag:          1,
			Location:     "北邮锦江酒店",
			IsOnline:     false,
			LocationNode: 23124, // 与上面Location对应
			IsMention:    true,
		},
	}

	return nil
}
