package system

type CourseInfo struct {
	CourseName         string `json:"course_name"`
	CourseId           int64  `json:"course_id"`
	Teacher            string `json:"teacher"`
	Contact            string `json:"contact"`
	SectionList        []int  `json:"section_list"`
	WeekSchedule       []int  `json:"week_schedule"`
	Classroom          string `json:"classroom"`            // 线下课程记录教室, 线上课程记录会议链接
	CourseLocationNode []int  `json:"course_location_node"` // 对应导航地图的节点, 设置成数组是因为一个地点可能对应多个node
	IsCourseOnline     bool   `json:"is_course_online"`
	ExamTime           string `json:"exam_time"` // 考试时间, 格式为'第几周-星期几-几点~几点（HH:mm~HH:mm）', 如'第十六周-星期三-19:00~20:00'
	ExamLocation       string `json:"exam_location"`
	ExamLocationNode   []int  `json:"exam_location_node"`
	ExamOption         int    `json:"exam_option"`
	IsCompulsory       bool   `json:"is_compulsory"` // 课程是否必修
}
