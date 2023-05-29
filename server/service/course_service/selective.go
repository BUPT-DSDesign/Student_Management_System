package course_service

import (
	"server/model/entity/system"
)

type selectiveFlow struct {
	// 包含handler层传来的参数等
	userId int64
}

func newSelectiveFlow(userId int64) *selectiveFlow {
	return &selectiveFlow{userId: userId}
}

func (s *server) DoSelective(userId int64) (*[]*system.CourseInfo, error) {
	return newSelectiveFlow(userId).do()
}

func (f *selectiveFlow) do() (*[]*system.CourseInfo, error) {
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
func (f *selectiveFlow) checkNum() error {
	// 这里userId是上层解析过来的, 一定合法
	return nil
}

func (f *selectiveFlow) run(courses **[]*system.CourseInfo) error {
	//if err := dao.Group.CourseDao.QueryAllSelectiveCourse(courses); err != nil {
	//	return err
	//}

	*courses = &[]*system.CourseInfo{
		{
			CourseName:         "计算机网络",
			CourseId:           2312312,
			Teacher:            "程莉",
			Contact:            "18611555833",
			SectionList:        []int{1, 2, 3, 25, 26},
			WeekSchedule:       []int{1, 3, 5, 6, 7},
			Classroom:          "教三-304",
			CourseLocationNode: nil,
			IsCourseOnline:     false,
			ExamTime:           "",
			ExamLocation:       "",
			ExamLocationNode:   nil,
			ExamOption:         0,
		},

		{
			CourseName:         "计算机组成原理",
			CourseId:           2312313,
			Teacher:            "张斌",
			Contact:            "18611555834",
			SectionList:        []int{15, 16, 30, 31},
			WeekSchedule:       []int{1, 2, 4, 6, 11},
			Classroom:          "教一-305",
			CourseLocationNode: nil,
			IsCourseOnline:     false,
			ExamTime:           "",
			ExamLocation:       "",
			ExamLocationNode:   nil,
			ExamOption:         0,
		},
		{
			CourseName:         "形式语言与自动机",
			CourseId:           2312314,
			Teacher:            "张斌",
			Contact:            "18611555835",
			SectionList:        []int{4, 5, 11, 12},
			WeekSchedule:       []int{1, 2, 4, 5, 7},
			Classroom:          "教三-305",
			CourseLocationNode: nil,
			IsCourseOnline:     false,
			ExamTime:           "",
			ExamLocation:       "",
			ExamLocationNode:   nil,
			ExamOption:         0,
		},
		{
			CourseName:         "形式与政策",
			CourseId:           2312315,
			Teacher:            "程莉",
			Contact:            "18611555836",
			SectionList:        []int{19, 20},
			WeekSchedule:       []int{1, 2, 3, 5, 6, 7, 8},
			Classroom:          "教二-311",
			CourseLocationNode: nil,
			IsCourseOnline:     false,
			ExamTime:           "",
			ExamLocation:       "",
			ExamLocationNode:   nil,
			ExamOption:         0,
		}, {
			CourseName:         "数据结构课设",
			CourseId:           2312316,
			Teacher:            "张海旸",
			Contact:            "18611555837",
			SectionList:        []int{6, 7, 8},
			WeekSchedule:       []int{1, 2, 3, 5, 6, 7, 8},
			Classroom:          "教二-335",
			CourseLocationNode: nil,
			IsCourseOnline:     false,
			ExamTime:           "",
			ExamLocation:       "",
			ExamLocationNode:   nil,
			ExamOption:         0,
		},
	}

	return nil
}