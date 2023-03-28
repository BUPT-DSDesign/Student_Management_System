package log_service

import (
	"server/model/entity/system"
)

type infoFlow struct {
	// 包含handler层传来的参数等
	userId int64
}

func newInfoFlow(userId int64) *infoFlow {
	return &infoFlow{userId: userId}
}

func (s *server) DoInfo(userId int64) (*[]*system.LogInfo, error) {
	return newInfoFlow(userId).do()
}

func (f *infoFlow) do() (*[]*system.LogInfo, error) {
	var logs *[]*system.LogInfo

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&logs); err != nil {
		return nil, err
	}

	return logs, nil
}

// 检验参数
func (f *infoFlow) checkNum() error {
	// 这里userId参数一定合法
	return nil
}

func (f *infoFlow) run(logs **[]*system.LogInfo) error {
	*logs = &[]*system.LogInfo{
		{
			LogId:      32132131,
			CreateTime: "第二周-星期三-8:00",
			Content:    "登录",
			UserId:     123,
		},
		{
			LogId:      32132133,
			CreateTime: "第二周-星期天-9:00",
			Content:    "导航",
			UserId:     123,
		},
	}
	return nil
}
