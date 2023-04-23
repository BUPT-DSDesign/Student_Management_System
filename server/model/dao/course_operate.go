package dao

import (
	"fmt"
	"server/model/entity/system"
	"server/utils"
)

func (s *courseDao) AddCourse(courseInfo *system.CourseInfo) error {
	/*
		添加课程
	*/
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
	return nil
}
