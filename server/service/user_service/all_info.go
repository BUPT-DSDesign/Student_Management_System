package user_service

import (
	"server/model/dao"
	"server/model/entity/system"
)

type allInfoFlow struct {
	// 包含handler层传来的参数等
	userId int64
}

func newAllInfoFlow(userId int64) *allInfoFlow {
	return &allInfoFlow{userId: userId}
}

func (s *server) DoAllInfo(userId int64) (*[]*system.UserInfo, error) {
	return newAllInfoFlow(userId).do()
}

func (f *allInfoFlow) do() (*[]*system.UserInfo, error) {
	userInfo := new([]*system.UserInfo)

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&userInfo); err != nil {
		return nil, err
	}

	return userInfo, nil
}

// 检验参数
func (f *allInfoFlow) checkNum() error {
	return nil
}

func (f *allInfoFlow) run(userInfo **[]*system.UserInfo) error {
	if err := dao.Group.UserDao.QueryAllUser(userInfo); err != nil {
		return err
	}

	return nil
}
