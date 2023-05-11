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
	// 先删除课程
	sqlStr := fmt.Sprintf("DELETE FROM course_info WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	// 再删除学生选课表中的内容
	sqlStr = fmt.Sprintf("DELETE FROM student_course WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}
	return nil
}

func (s *courseDao) UpdateCourse(courseId int64, newCourseInfo *common.AddCourseRequest) error {
	/*
		更新课程
	*/
	sqlStr := fmt.Sprintf("UPDATE course_info SET course_name = '%v', teacher = '%v', contact = '%v', classroom = '%v', is_course_online = '%v', exam_time = '%v', exam_location = '%v', exam_option = '%v', is_compulsory = '%v' WHERE course_id = '%v'",
		newCourseInfo.CourseName,
		newCourseInfo.Teacher,
		newCourseInfo.Contact,
		newCourseInfo.Classroom,
		utils.BoolToInt8(newCourseInfo.IsCourseOnline),
		newCourseInfo.ExamTime,
		newCourseInfo.ExamLocation,
		newCourseInfo.ExamOption,
		utils.BoolToInt8(newCourseInfo.IsCompulsory),
		courseId,
	)

	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QueryCourseByName(courseName string, courses **[]*system.CourseInfo) error {
	/*
		根据课程名查询课程
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM course_info WHERE course_name = '%v'", courseName)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QueryCourseByClassroom(classroom string, courses **[]*system.CourseInfo) error {
	/*
		根据教室查询课程
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM course_info WHERE classroom = '%v'", classroom)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	return nil
}
