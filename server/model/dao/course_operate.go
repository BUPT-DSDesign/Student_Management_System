package dao

import (
	"fmt"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/utils"
)

func (s *courseDao) AddCourse(courseInfo *system.CourseInfo) error {
	sqlStr := fmt.Sprintf("INSERT INTO course_info VALUES('%v', '%v', '%v', '%v', '%v', '%v', '%v', '%v', '%v', '%v')",
		courseInfo.CourseName,
		courseInfo.CourseId,
		courseInfo.Teacher,
		courseInfo.Contact,
		courseInfo.Classroom,
		utils.BoolToInt8(courseInfo.IsCourseOnline),
		courseInfo.ExamTime,
		courseInfo.ExamLocation,
		courseInfo.ExamOption,
		utils.BoolToInt8(courseInfo.IsCompulsory),
	)

	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) DeleteCourse(courseId int64) error {
	/*
		先根据课程id删除数据库中的课程数据,
		再根据有哪些学生含有这些课, 删除对应的内容
	*/
	sqlStr := fmt.Sprintf("DELETE FROM course_info WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}
	return nil
}

func (s *courseDao) UpdateCourse(newCourseInfo *common.AddCourseRequest) error {
	/*
		更新课程
	*/
	return nil
}

func (s *courseDao) QueryCourseByName(courseName string, courses **[]*system.CourseInfo) error {
	/*
		根据课程名查询课程
	*/
	return nil
}

func (s *courseDao) QueryCourseByClassroom(classroom string, courses **[]*system.CourseInfo) error {
	/*
		根据教室查询课程
	*/
	return nil
}
