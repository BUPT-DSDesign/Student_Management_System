package user_service

import (
	"server/model/dao"
	"server/model/entity/system"
)

type infoFlow struct {
	// 包含handler层传来的参数等
	userId int64
}

func newInfoFlow(userId int64) *infoFlow {
	return &infoFlow{userId: userId}
}

func (s *server) DoInfo(userId int64) (*system.UserInfo, error) {
	return newInfoFlow(userId).do()
}

func (f *infoFlow) do() (*system.UserInfo, error) {
	var userInfo *system.UserInfo

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&userInfo); err != nil {
		return nil, err
	}

	return userInfo, nil
}

// 检验参数
func (f *infoFlow) checkNum() error {
	// 这里参数一定合法
	return nil
}

func (f *infoFlow) run(userInfo **system.UserInfo) error {
	// 根据f.userId得到用户信息
	if err := dao.Group.UserDao.QueryUserById(f.userId, userInfo); err != nil {
		return err
	}

	*userInfo = &system.UserInfo{
		Username:  "admin",
		Password:  "213213",
		Salt:      "3213",
		UserId:    0,
		StudentId: "32131",
		IsAdmin:   false,
		AvatarUrl: "http://127.0.0.1:8080/static/avatar.jpg",
		Signature: "开心每一天",
	}

	return nil
}
