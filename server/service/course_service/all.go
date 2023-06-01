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
	courses := new([]*system.CourseInfo)

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

	// 添加课程的周次和节次
	for _, course := range **courses {
		// 课程的周次和节次
		var weekSchedule []int
		var sectionList []int
		if err := dao.Group.CourseDao.QueryWeekScheduleById(course.CourseId, &weekSchedule); err != nil {
			return err
		}
		if err := dao.Group.CourseDao.QuerySectionListById(course.CourseId, &sectionList); err != nil {
			return err
		}
		course.WeekSchedule = weekSchedule
		course.SectionList = sectionList
	}

	return nil
}
