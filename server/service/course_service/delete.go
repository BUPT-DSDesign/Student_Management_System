package course_service

import (
	"server/model/dao"
)

type deleteFlow struct {
	// 包含handler层传来的参数等
	userId   int64
	courseId int64
}

func newDeleteFlow(userId int64, courseId int64) *deleteFlow {
	return &deleteFlow{userId: userId, courseId: courseId}
}

func (s *server) DoDelete(userId int64, courseId int64) error {
	return newDeleteFlow(userId, courseId).do()
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
	// 根据userId, 判断是否是管理员进行操作
	return nil
}

func (f *deleteFlow) run() error {
	//根据课程id删除课程
	if err := dao.Group.CourseDao.DeleteCourse(f.courseId); err != nil {
		return err
	}

	return nil
}
