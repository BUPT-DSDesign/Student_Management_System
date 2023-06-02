package dao

import (
	"encoding/json"
	"errors"
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

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 添加到课程-节次表
	for _, v := range courseInfo.SectionList {
		id, _ := utils.GenerateId()
		sqlStr = fmt.Sprintf("INSERT INTO course_section VALUES('%v', '%v', '%v')", id, courseInfo.CourseId, v)
		if err = db.ExecSql(sqlStr); err != nil {
			return err
		}
		jsonStr, err := ReadLine()
		if err != nil {
			return err
		}

		// 用一个map来接收返回的json
		var result map[string]interface{}
		_ = json.Unmarshal(jsonStr, &result)

		// 判断result.status_code是否为0
		if result["status_code"].(float64) != 0 {
			return errors.New(result["status_msg"].(string))
		}
	}

	// 添加课程-周次表
	for _, v := range courseInfo.WeekSchedule {
		id, _ := utils.GenerateId()
		sqlStr = fmt.Sprintf("INSERT INTO course_week VALUES('%v', '%v', '%v')", id, courseInfo.CourseId, v)
		if err = db.ExecSql(sqlStr); err != nil {
			return err
		}
		jsonStr, err := ReadLine()
		if err != nil {
			return err
		}

		// 用一个map来接收返回的json
		var result map[string]interface{}
		_ = json.Unmarshal(jsonStr, &result)

		// 判断result.status_code是否为0
		if result["status_code"].(float64) != 0 {
			return errors.New(result["status_msg"].(string))
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

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 再删除学生选课表中的内容
	sqlStr = fmt.Sprintf("DELETE FROM student_course WHERE course_id = '%v'", courseId)
	if err = db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err = ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 删除course_section表中的内容
	sqlStr = fmt.Sprintf("DELETE FROM course_section WHERE course_id = '%v'", courseId)
	if err = db.ExecSql(sqlStr); err != nil {
		return err
	}
	jsonStr, err = ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 删除course_week表中的内容
	sqlStr = fmt.Sprintf("DELETE FROM course_week WHERE course_id = '%v'", courseId)
	if err = db.ExecSql(sqlStr); err != nil {
		return err
	}
	jsonStr, err = ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	return nil
}

func (s *courseDao) UpdateCourse(courseId int64, newCourseInfo *common.AddCourseRequest) error {
	/*
		更新课程, 先根绝id删除课程, 再根据新的信息添加课程
	*/
	// 先删除课程
	sqlStr := fmt.Sprintf("DELETE FROM course_info WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}
	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}
	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)
	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 再删除course_section表中的内容
	sqlStr = fmt.Sprintf("DELETE FROM course_section WHERE course_id = '%v'", courseId)
	if err = db.ExecSql(sqlStr); err != nil {
		return err
	}
	jsonStr, err = ReadLine()
	if err != nil {
		return err
	}
	// 用一个map来接收返回的json
	_ = json.Unmarshal(jsonStr, &result)
	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}
	// 再删除course_week表中的内容
	sqlStr = fmt.Sprintf("DELETE FROM course_week WHERE course_id = '%v'", courseId)
	if err = db.ExecSql(sqlStr); err != nil {
		return err
	}
	jsonStr, err = ReadLine()
	if err != nil {
		return err
	}
	// 用一个map来接收返回的json
	_ = json.Unmarshal(jsonStr, &result)
	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 再根据newCourseInfo添加课程
	courseInfo := &system.CourseInfo{
		CourseName:         newCourseInfo.CourseName,
		CourseId:           courseId,
		Teacher:            newCourseInfo.Teacher,
		Contact:            newCourseInfo.Contact,
		SectionList:        newCourseInfo.SectionList,
		WeekSchedule:       newCourseInfo.WeekSchedule,
		Classroom:          newCourseInfo.Classroom,
		CourseLocationNode: nil, // 教室的地点编号
		IsCourseOnline:     newCourseInfo.IsCourseOnline,
		ExamTime:           newCourseInfo.ExamTime,
		ExamLocation:       newCourseInfo.ExamLocation,
		ExamLocationNode:   nil, // 考试的地点编号
		ExamOption:         newCourseInfo.ExamOption,
		IsCompulsory:       newCourseInfo.IsCompulsory,
	}

	if err = s.AddCourse(courseInfo); err != nil {
		return err
	}

	return nil
}

func (s *courseDao) QueryAllCourse(courses **[]*system.CourseInfo) error {
	sqlStr := "SELECT * FROM course_info"
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]*system.CourseInfo
	_ = json.Unmarshal([]byte(result["data"].(string)), *courses)

	// 将result["data"]中的is_compulsory、is_course_online转换为bool,并赋值给courses
	mpStr := result["data"].(string)

	// 将mpStr转换为map
	mp := make([]map[string]interface{}, 0)
	_ = json.Unmarshal([]byte(mpStr), &mp)

	for i, course := range **courses {
		if mp[i]["is_compulsory"].(float64) == 0 {
			course.IsCompulsory = false
		} else {
			println("true成功")
			course.IsCompulsory = true
		}
		if mp[i]["is_course_online"].(float64) == 0 {
			course.IsCourseOnline = false
		} else {
			course.IsCourseOnline = true
		}
	}

	return nil
}

func (s *courseDao) QueryCourseByUserId(userId int64, courses *[]*system.CourseInfo) error {
	// 查询学生选课表
	// 方法是先查询必修课，再查询选修课
	// 必修课
	compulsoryCourse := new([]*system.CourseInfo)
	if err := s.QueryCompulsoryCourse(&compulsoryCourse); err != nil {
		return err
	}

	// 将compulsoryCourse中is_compulsory置为1
	for _, course := range *compulsoryCourse {
		course.IsCompulsory = true
	}

	// 选修课
	electiveCourse := new([]*system.CourseInfo)
	if err := s.QueryElectiveCourse(userId, &electiveCourse); err != nil {
		return err
	}
	*courses = append(*compulsoryCourse, *electiveCourse...)
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

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]*system.CourseInfo
	_ = json.Unmarshal([]byte(result["data"].(string)), *courses)

	return nil
}

func (s *courseDao) QueryElectiveCourse(userId int64, courses **[]*system.CourseInfo) error {
	/*
		查询选修课
	*/

	// 先查询课程id
	sonSqlStr := fmt.Sprintf("SELECT * FROM student_course WHERE student_id = '%v'", userId)
	if err := db.ExecSql(sonSqlStr); err != nil {
		return err
	}

	sonJsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(sonJsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	var studentCourse []*system.StudentCourse
	// 将result.data转换为[]*system.CourseInfo
	_ = json.Unmarshal([]byte(result["data"].(string)), &studentCourse)

	realCourses := new([]*system.CourseInfo)

	// 再查询课程信息
	for _, v := range studentCourse {
		var courseInfo []*system.CourseInfo
		sqlStr := fmt.Sprintf("SELECT * FROM course_info WHERE course_id = '%v'", v.CourseId)
		if err := db.ExecSql(sqlStr); err != nil {
			return err
		}

		jsonStr, err := ReadLine()
		if err != nil {
			return err
		}
		var result1 map[string]interface{}
		_ = json.Unmarshal(jsonStr, &result1)
		if result1["status_code"].(float64) != 0 {
			return errors.New(result1["status_msg"].(string))
		}
		_ = json.Unmarshal([]byte(result1["data"].(string)), &courseInfo)
		// 将courseInfo添加到courses中
		*realCourses = append(*realCourses, courseInfo...)
	}

	*courses = realCourses

	return nil
}

func (s *courseDao) JudgeIsStudentSelectCourse(userId int64, courseId int64) bool {
	// 判断学生是否已经选过该课程
	sqlStr := fmt.Sprintf("SELECT * FROM student_course WHERE student_id = '%v' AND course_id = '%v'", userId, courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return false
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return false
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return false
	}

	// 将result.data转换为[]system.StudentCourse
	var studentCourse []system.StudentCourse
	_ = json.Unmarshal([]byte(result["data"].(string)), &studentCourse)

	if len(studentCourse) != 0 {
		return true
	}

	return false

}

func (s *courseDao) QueryAllSelectiveCourse(courses **[]*system.CourseInfo) error {
	/*
		查询所有选修课
	*/
	sqlStr := "SELECT * FROM course_info WHERE is_compulsory = 0"
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]*system.CourseInfo
	_ = json.Unmarshal([]byte(result["data"].(string)), *courses)

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

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	return nil
}

func (s *courseDao) QuerySectionListById(courseId int64, sectionList *[]int) error {
	/*
		根据课程id查询节次
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM course_section WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	var courseSection []*system.CourseSection
	_ = json.Unmarshal([]byte(result["data"].(string)), &courseSection)

	for _, v := range courseSection {
		*sectionList = append(*sectionList, v.SectionId)
	}

	return nil
}

func (s *courseDao) QueryWeekScheduleById(courseId int64, weekSchedule *[]int) error {
	/*
		根据课程id查询周次
	*/
	sqlStr := fmt.Sprintf("SELECT week_id FROM course_week WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	var courseWeek []*system.CourseWeek
	_ = json.Unmarshal([]byte(result["data"].(string)), &courseWeek)

	for _, v := range courseWeek {
		*weekSchedule = append(*weekSchedule, v.WeekId)
	}

	// 将result.data转换为[]int
	_ = json.Unmarshal([]byte(result["data"].(string)), weekSchedule)

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

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]*system.CourseInfo
	_ = json.Unmarshal([]byte(result["data"].(string)), *courses)

	// 将result["data"]中的is_compulsory、is_course_online转换为bool,并赋值给courses
	mpStr := result["data"].(string)

	// 将mpStr转换为map
	mp := make([]map[string]interface{}, 0)
	_ = json.Unmarshal([]byte(mpStr), &mp)

	for i, v := range **courses {
		if mp[i]["is_compulsory"].(float64) == 0 {
			v.IsCompulsory = false
		} else {
			v.IsCompulsory = true
		}
		if mp[i]["is_course_online"].(float64) == 0 {
			v.IsCourseOnline = false
		} else {
			v.IsCourseOnline = true
		}
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

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	// 将result.data转换为[]*system.CourseInfo
	_ = json.Unmarshal([]byte(result["data"].(string)), *courses)

	// 将result["data"]中的is_compulsory、is_course_online转换为bool,并赋值给courses
	mpStr := result["data"].(string)

	// 将mpStr转换为map
	mp := make([]map[string]interface{}, 0)
	_ = json.Unmarshal([]byte(mpStr), &mp)

	for i, v := range **courses {
		if mp[i]["is_compulsory"].(float64) == 0 {
			v.IsCompulsory = false
		} else {
			v.IsCompulsory = true
		}
		if mp[i]["is_course_online"].(float64) == 0 {
			v.IsCourseOnline = false
		} else {
			v.IsCourseOnline = true
		}
	}

	return nil
}

// QueryCourseBySection 根据节次查询课程
func (s *courseDao) QueryCourseBySection(section int, courseIds *[]int64) error {
	/*
		根据节次查询课程
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM course_section WHERE section_id = %d", section)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	var courseSection []*system.CourseSection
	// 将result.data转换为
	_ = json.Unmarshal([]byte(result["data"].(string)), &courseSection)

	for _, v := range courseSection {
		*courseIds = append(*courseIds, v.CourseId)
	}

	return nil
}

func (s *courseDao) QueryCourseByWeek(week int, courseIds *[]int64) error {
	/*
		根据周次查询课程
	*/
	sqlStr := fmt.Sprintf("SELECT * FROM course_week WHERE week_id = %d", week)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	var courseWeek []*system.CourseWeek
	// 将result.data转换为
	_ = json.Unmarshal([]byte(result["data"].(string)), &courseWeek)

	for _, v := range courseWeek {
		*courseIds = append(*courseIds, v.CourseId)
	}

	return nil
}

// QueryCourseById 根据课程id查询课程信息
func (s *courseDao) QueryCourseById(courseId int64, course *system.CourseInfo) error {
	sqlStr := fmt.Sprintf("SELECT * FROM course_info WHERE course_id = '%v'", courseId)
	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	jsonStr, err := ReadLine()
	if err != nil {
		return err
	}

	// 用一个map来接收返回的json
	var result map[string]interface{}
	_ = json.Unmarshal(jsonStr, &result)

	// 判断result.status_code是否为0
	if result["status_code"].(float64) != 0 {
		return errors.New(result["status_msg"].(string))
	}

	var courseInfo []*system.CourseInfo
	// 将result.data转换为
	_ = json.Unmarshal([]byte(result["data"].(string)), &courseInfo)

	// 将result["data"]中的is_compulsory、is_course_online转换为bool,并赋值给courses
	mpStr := result["data"].(string)

	// 将mpStr转换为map
	mp := make([]map[string]interface{}, 0)
	_ = json.Unmarshal([]byte(mpStr), &mp)

	for i, v := range courseInfo {
		if mp[i]["is_compulsory"].(float64) == 0 {
			v.IsCompulsory = false
		} else {
			v.IsCompulsory = true
		}
		if mp[i]["is_course_online"].(float64) == 0 {
			v.IsCourseOnline = false
		} else {
			v.IsCourseOnline = true
		}
	}

	if len(courseInfo) != 0 {
		*course = *courseInfo[0]

	}

	return nil
}
