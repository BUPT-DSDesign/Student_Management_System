package course_service

import (
	"errors"
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
)

type updateFlow struct {
	// 包含handler层传来的参数等
	userId           int64
	courseId         int64
	addCourseRequest common.AddCourseRequest
}

func newUpdateFlow(userId int64, courseId int64, addCourseRequest common.AddCourseRequest) *updateFlow {
	return &updateFlow{userId: userId, courseId: courseId, addCourseRequest: addCourseRequest}
}

func (s *server) DoUpdate(userId int64, courseId int64, updateCourseRequest common.AddCourseRequest) error {
	return newUpdateFlow(userId, courseId, updateCourseRequest).do()
}

func (f *updateFlow) do() error {
	if err := f.checkNum(); err != nil {
		return err
	}
	if err := f.run(); err != nil {
		return err
	}

	return nil
}

// 检验参数
func (f *updateFlow) checkNum() error {
	// 根据userId, 判断是否是管理员进行操作
	var userInfo *system.UserInfo
	if err := dao.Group.UserDao.QueryUserById(f.userId, &userInfo); err != nil {
		return err
	}
	if userInfo.IsAdmin == false {
		return errors.New("您不是管理员, 没有权限更新课程")
	}
	return nil
}

func (f *updateFlow) run() error {
	// 根据f.addCourseRequest更新课程
	if err := dao.Group.CourseDao.UpdateCourse(f.courseId, &f.addCourseRequest); err != nil {
		return err
	}

	return nil
}
