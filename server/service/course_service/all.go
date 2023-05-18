package course_service

import (
	"server/model/dao"
	"server/model/entity/system"
)

type allFlow struct {
	// 包含handler层传来的参数等
	userId int64
}

func newAllFlow(userId int64) *allFlow {
	return &allFlow{userId: userId}
}

func (s *server) DoAll(userId int64) (*[]*system.CourseInfo, error) {
	return newAllFlow(userId).do()
}

func (f *allFlow) do() (*[]*system.CourseInfo, error) {
	var courses *[]*system.CourseInfo

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&courses); err != nil {
		return nil, err
	}

	return courses, nil
}

// 检验参数
func (f *allFlow) checkNum() error {
	// 这里userId是上层解析过来的, 一定合法
	return nil
}

func (f *allFlow) run(courses **[]*system.CourseInfo) error {

	if err := dao.Group.CourseDao.QueryAllCourse(courses); err != nil {
		return err
	}

	return nil
}
