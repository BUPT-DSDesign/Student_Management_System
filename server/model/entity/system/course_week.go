package system

type CourseWeek struct {
	Id int64 `json:"id"`
	// 课程id
	CourseId int64 `json:"course_id"`
	// 第几周
	WeekId int `json:"week_id"`
}
