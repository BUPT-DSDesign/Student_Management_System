package course_service

import (
	"server/model/entity/common"
)

type addFlow struct {
	// 包含handler层传来的参数等
	userId           int64
	addCourseRequest common.AddCourseRequest
}

func newAddFlow(userId int64, addCourseRequest common.AddCourseRequest) *addFlow {
	return &addFlow{userId: userId, addCourseRequest: addCourseRequest}
}

func (s *server) DoAdd(userId int64, addCourseRequest common.AddCourseRequest) error {
	return newAddFlow(userId, addCourseRequest).do()
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
	// 根据userId, 判断是否是管理员进行操作
	//var userInfo *system.UserInfo
	//if err := dao.Group.UserDao.QueryUserById(f.userId, &userInfo); err != nil {
	//	return err
	//}
	//if userInfo.IsAdmin == false {
	//	return errors.New("您不是管理员, 没有权限添加课程")
	//}
	return nil
}

func (f *addFlow) run() error {
	/*
		调用dao层的CRUD操作
	*/

	return nil
}
