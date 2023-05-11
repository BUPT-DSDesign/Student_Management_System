package system

// StudentCourse 每当一名同学选择一门课程, 就会在数据库中添加一条记录, 记录存放学生id和课程id
type StudentCourse struct {
	Id        int64 `json:"id"`
	StudentId int64 `json:"student_id"`
	CourseId  int64 `json:"course_id"`
}
