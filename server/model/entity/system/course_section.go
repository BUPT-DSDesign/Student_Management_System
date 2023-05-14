package system

type CourseSection struct {
	Id int64 `json:"id"`
	// 课程id
	CourseId  int64 `json:"course_id"`
	SectionId int   `json:"section_id"`
}
