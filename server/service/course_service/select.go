package course_service

import (
	"server/model/dao"
)

type selectFlow struct {
	// 包含handler层传来的参数等
	userId   int64
	courseId int64
}

func newSelectFlow(userId int64, courseId int64) *selectFlow {
	return &selectFlow{userId: userId, courseId: courseId}
}

func (s *server) DoSelect(userId int64, courseId int64) error {
	return newSelectFlow(userId, courseId).do()
}

func (f *selectFlow) do() error {

	if err := f.checkNum(); err != nil {
		return err
	}
	if err := f.run(); err != nil {
		return err
	}

	return nil
}

// 检验参数
func (f *selectFlow) checkNum() error {
	// 这里参数一定合法
	return nil
}

func (f *selectFlow) run() error {

	if err := dao.Group.CourseDao.SelectCourse(f.userId, f.courseId); err != nil {
		return err
	}
	return nil
}
