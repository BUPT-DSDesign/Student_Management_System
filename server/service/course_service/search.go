package course_service

import (
	"server/model/dao"
	"server/model/entity/system"
)

type searchFlow struct {
	// 包含handler层传来的参数等
	userId         int64
	isByCourseName uint8
	keyWord        string
}

func newSearchFlow(userId int64, isByCourseName uint8, keyWord string) *searchFlow {
	return &searchFlow{userId: userId, isByCourseName: isByCourseName, keyWord: keyWord}
}

func (s *server) DoSearch(userId int64, isByCourseName uint8, keyWord string) (*[]*system.CourseInfo, error) {
	return newSearchFlow(userId, isByCourseName, keyWord).do()
}

func (f *searchFlow) do() (*[]*system.CourseInfo, error) {
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
func (f *searchFlow) checkNum() error {
	// 这里参数一定合法
	return nil
}

func (f *searchFlow) run(courses **[]*system.CourseInfo) error {
	// 判断是否是通过课程名查找
	if f.isByCourseName == 1 {
		if err := dao.Group.CourseDao.QueryCourseByName(f.keyWord, courses); err != nil {
			return err
		}
		return nil
	}

	if err := dao.Group.CourseDao.QueryCourseByClassroom(f.keyWord, courses); err != nil {
		return err
	}

	return nil
}
