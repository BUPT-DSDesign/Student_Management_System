package dao

import "server/model/entity/system"

func (s *courseDao) AddCourse(courseInfo *system.CourseInfo) error {
	/*
		添加课程
	*/
	return nil
}

func (s *courseDao) DeleteCourse(courseId int64) error {
	/*
		先根据课程id删除数据库中的课程数据,
		再根据有哪些学生含有这些课, 删除对应的内容
	*/
	return nil
}
