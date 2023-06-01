package course_service

import (
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/utils"
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
	return nil
}

func (f *addFlow) run() error {
	// 生成课程id
	courseId, err := utils.GenerateId()
	if err != nil {
		return err
	}

	// 将addCourseRequest转换为courseInfo
	courseInfo := &system.CourseInfo{
		CourseName:         f.addCourseRequest.CourseName,
		CourseId:           courseId,
		Teacher:            f.addCourseRequest.Teacher,
		Contact:            f.addCourseRequest.Contact,
		SectionList:        f.addCourseRequest.SectionList,
		WeekSchedule:       f.addCourseRequest.WeekSchedule,
		Classroom:          f.addCourseRequest.Classroom,
		CourseLocationNode: nil, // 教室的地点编号
		IsCourseOnline:     f.addCourseRequest.IsCourseOnline,
		ExamTime:           f.addCourseRequest.ExamTime,
		ExamLocation:       f.addCourseRequest.ExamLocation,
		ExamLocationNode:   nil, // 考试的地点编号
		ExamOption:         f.addCourseRequest.ExamOption,
		IsCompulsory:       f.addCourseRequest.IsCompulsory,
	}

	// 将courseInfo存入数据库
	if err := dao.Group.CourseDao.AddCourse(courseInfo); err != nil {
		return err
	}

	return nil
}
