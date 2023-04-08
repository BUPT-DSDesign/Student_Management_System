package log_service

import (
	"server/model/dao"
	"server/model/entity/system"
	"server/utils"
)

type addFlow struct {
	// 包含handler层传来的参数等
	createTime string
	content    string
	userId     int64
}

func newAddFlow(createTime string, content string, userId int64) *addFlow {
	return &addFlow{createTime: createTime, content: content, userId: userId}
}

func (s *server) DoAdd(createTime string, content string, userId int64) (int64, error) {
	return newAddFlow(createTime, content, userId).do()
}

func (f *addFlow) do() (int64, error) {
	var logId int64

	if err := f.checkNum(); err != nil {
		return 0, err
	}
	if err := f.run(&logId); err != nil {
		return 0, err
	}

	return logId, nil
}

// 检验参数
func (f *addFlow) checkNum() error {
	// 这里参数一定合法
	return nil
}

func (f *addFlow) run(logId *int64) error {
	// 得到日志id
	id, err := utils.GenerateId()
	if err != nil {
		return err
	}

	// 创建一个日志实例, 后来存入数据库
	logInfo := &system.LogInfo{
		LogId:      id,
		CreateTime: f.createTime,
		Content:    f.content,
		UserId:     f.userId,
	}

	err = dao.Group.LogDao.AddLog(logInfo)
	if err != nil {
		return err
	}

	*logId = id

	return nil
}
