package user_service

import (
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
	var userInfo *[]*system.UserInfo

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
	// 这里参数一定合法
	// 根据userId, 判断是否是管理员进行操作
	//var userInfo *system.UserInfo
	//if err := dao.Group.UserDao.QueryUserById(f.userId, &userInfo); err != nil {
	//	return err
	//}
	//if userInfo.IsAdmin == false {
	//	return errors.New("您不是管理员, 没有权限查看所有学生信息")
	//}
	return nil
}

func (f *allInfoFlow) run(userInfo **[]*system.UserInfo) error {
	//if err := dao.Group.UserDao.QueryAllUser(userInfo); err != nil {
	//	return err
	//}

	*userInfo = &[]*system.UserInfo{{
		Username:  "admin",
		Password:  "213213",
		Salt:      "3213",
		UserId:    0,
		StudentId: "32131",
		IsAdmin:   false,
		AvatarUrl: "http://127.0.0.1:8080/static/avatar.jpg",
		Signature: "",
	}, {
		Username:  "admin",
		Password:  "213213",
		Salt:      "3213",
		UserId:    0,
		StudentId: "32131",
		IsAdmin:   false,
		AvatarUrl: "http://127.0.0.1:8080/static/avatar.jpg",
		Signature: "开心每一天",
	},
	}

	return nil
}
