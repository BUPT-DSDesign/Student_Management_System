package dao

import (
	"encoding/json"
	"fmt"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/utils"
)

func (s *courseDao) AddCourse(courseInfo *system.CourseInfo) error {
	//goland:noinspection ALL
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

	_, err := ReadLine()
	if err != nil {
		return err
	}

	// 添加到课程-节次表
	for _, v := range courseInfo.SectionList {
		id, _ := utils.GenerateId()
		sqlStr = fmt.Sprintf("INSERT INTO course_section VALUES('%v', '%v', '%v')", id, courseInfo.CourseId, v)
		if err = db.ExecSql(sqlStr); err != nil {
			return err
		}
		_, err = ReadLine()
		if err != nil {
			return err
		}
	}

	// 添加课程-周次表
	for _, v := range courseInfo.WeekSchedule {
		id, _ := utils.GenerateId()
		sqlStr = fmt.Sprintf("INSERT INTO course_week VALUES('%v', '%v', '%v')", id, courseInfo.CourseId, v)
		if err = db.ExecSql(sqlStr); err != nil {
			return err
		}
		_, err = ReadLine()
		if err != nil {
			return err
		}
	}

	return nil
}

func (s *courseDao) DeleteCourse(courseId int64) error {
	// 先删除课程
	sqlStr := fmt.Sprintf("DELETE FROM course_info WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}
	_, err := ReadLine()
	if err != nil {
		return err
	}

	// 再删除学生选课表中的内容
	sqlStr = fmt.Sprintf("DELETE FROM student_course WHERE course_id = '%v'", courseId)
	if err = db.ExecSql(sqlStr); err != nil {
		return err
	}
	_, err = ReadLine()
	if err != nil {
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

	_, err := ReadLine()
	if err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QueryAllCourse(courses **[]*system.CourseInfo) error {
	sqlStr := "SELECT * FROM course_info"
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, *courses); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QueryCompulsoryCourse(courses **[]*system.CourseInfo) error {
	/*
		查询必修课
	*/
	sqlStr := "SELECT * FROM course_info WHERE is_compulsory = 1"
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, *courses); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QueryElectiveCourse(userId int64, courses **[]*system.CourseInfo) error {
	/*
		查询选修课
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM course_info WHERE course_id IN (SELECT course_id FROM student_course WHERE user_id = '%v')", userId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, *courses); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) JudgeIsStudentSelectCourse(userId int64, courseId int64) bool {
	// 判断学生是否已经选过该课程
	sqlStr := fmt.Sprintf("SELECT * FROM student_course WHERE student_id = '%v' AND course_id = '%v'", userId, courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return false
	}

	result, err := ReadLine()
	if err != nil {
		return false
	}
	var studentCourse system.StudentCourse
	_ = json.Unmarshal(result, &studentCourse)

	if studentCourse.StudentId == 0 {
		return false
	}
	return true

}

func (s *courseDao) QueryAllSelectiveCourse(courses **[]*system.CourseInfo) error {
	/*
		查询所有选修课
	*/
	sqlStr := "SELECT * FROM course_info WHERE is_compulsory = 0"
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, *courses); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) SelectCourse(userId int64, courseId int64) error {
	/*
		学生选课
	*/
	id, _ := utils.GenerateId()

	sqlStr := fmt.Sprintf("INSERT INTO student_course VALUES ('%v', '%v', '%v')", id, userId, courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	_, err := ReadLine()
	if err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QuerySectionListById(courseId int64, sectionList *[]int) error {
	/*
		根据课程id查询节次
	*/
	sqlStr := fmt.Sprintf("SELECT section_id FROM course_section WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, sectionList); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QueryWeekScheduleById(courseId int64, weekSchedule *[]int) error {
	/*
		根据课程id查询周次
	*/
	sqlStr := fmt.Sprintf("SELECT week_id FROM course_week_schedule WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, weekSchedule); err != nil {
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

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, *courses); err != nil {
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

	result, err := ReadLine()
	if err != nil {
		return err
	}
	if err = json.Unmarshal(result, *courses); err != nil {
		return err
	}

	return nil
}
