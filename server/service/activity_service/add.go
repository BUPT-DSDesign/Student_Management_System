package activity_service

import (
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/utils"
)

type addFlow struct {
	// 包含handler层传来的参数等
	userId             int64
	addActivityRequest common.AddActivityRequest
}

func newAddFlow(userId int64, addActivityRequest common.AddActivityRequest) *addFlow {
	return &addFlow{userId: userId, addActivityRequest: addActivityRequest}
}

func (s *server) DoAdd(userId int64, addActivityRequest common.AddActivityRequest) error {
	return newAddFlow(userId, addActivityRequest).do()
}

func (f *addFlow) do() error {
	if err := f.checkNum(); err != nil {
		return err
	}
	if err := f.run(); err != nil {
		return err
	}

	return nil
}

// 检验参数
func (f *addFlow) checkNum() error {
	// 这里userId是上层解析过来的, 一定合法
	return nil
}

func (f *addFlow) run() error {
	// 生成活动id
	activityId, err := utils.GenerateId()
	if err != nil {
		return err
	}

	// 将addActivityRequest转换为activityInfo
	activityInfo := &system.ActivityInfo{
		ActivityId:   activityId,
		ActivityName: f.addActivityRequest.ActivityName,
		UserId:       f.userId,
		StartTime:    f.addActivityRequest.StartTime,
		Type:         f.addActivityRequest.Type,
		Location:     f.addActivityRequest.Location,
		Tag:          f.addActivityRequest.Tag,
		Frequency:    f.addActivityRequest.Frequency,
		IsMention:    f.addActivityRequest.IsMention,
	}

	// 将activityInfo插入数据库
	if err := dao.Group.ActivityDao.AddActivity(activityInfo); err != nil {
		return err
	}

	return nil
}
