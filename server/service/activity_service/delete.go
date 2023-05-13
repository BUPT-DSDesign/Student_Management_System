package activity_service

import "server/model/dao"

type deleteFlow struct {
	// 包含handler层传来的参数等
	userId     int64
	activityId int64
}

func newDeleteFlow(userId int64, activityId int64) *deleteFlow {
	return &deleteFlow{userId: userId, activityId: activityId}
}

func (s *server) DoDelete(userId int64, activityId int64) error {
	return newDeleteFlow(userId, activityId).do()
}

func (f *deleteFlow) do() error {
	if err := f.checkNum(); err != nil {
		return err
	}
	if err := f.run(); err != nil {
		return err
	}

	return nil
}

// 检验参数
func (f *deleteFlow) checkNum() error {
	// 这里userId是上层解析过来的, 一定合法
	return nil
}

func (f *deleteFlow) run() error {
	// 根据f.activityId删除活动
	if err := dao.Group.ActivityDao.DeleteActivity(f.activityId); err != nil {
		return err
	}
	return nil
}
