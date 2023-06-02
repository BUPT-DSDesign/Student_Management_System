package course_service

import (
	"errors"
	"server/model/dao"
	"server/model/entity/common"
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
	// 查看课程时间是否冲突
	// 先遍历课程的周次
	for _, week := range f.addCourseRequest.WeekSchedule {
		var courseIds []int64
		if err := dao.Group.CourseDao.QueryCourseByWeek(week, &courseIds); err != nil {
			return err
		}
		var allSectionList []int
		for _, courseId := range courseIds {
			var sectionList []int
			if err := dao.Group.CourseDao.QuerySectionListById(courseId, &sectionList); err != nil {
				return err
			}
			allSectionList = append(allSectionList, sectionList...)
		}

		// 判断f.addCourseRequest.SectionList和allSectionList是否有交集
		for _, section := range f.addCourseRequest.SectionList {
			for _, allSection := range allSectionList {
				if section == allSection {
					return errors.New("更新后课程时间与其他课程时间冲突, 请重新更新课程")
				}
			}
		}
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
