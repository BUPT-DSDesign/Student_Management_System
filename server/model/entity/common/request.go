package common

// LoginRequest 登录请求
type LoginRequest struct {
	Username string `json:"username" form:"username"`
	Password string `json:"password" form:"password"`
}

// RegisterRequest 注册请求
type RegisterRequest struct {
	StudentId string `json:"student_id" form:"student_id"`
	LoginRequest
}

// AddCourseRequest 添加课程请求
type AddCourseRequest struct {
	CourseName     string `json:"course_name" form:"course_name"`
	Teacher        string `json:"teacher" form:"teacher"`
	Contact        string `json:"contact" form:"contact"`
	SectionList    []int  `json:"section_list" form:"section_list"`
	WeekSchedule   []int  `json:"week_schedule" form:"week_schedule"`
	Classroom      string `json:"classroom" form:"classroom"`
	IsCourseOnline bool   `json:"is_course_online" form:"is_course_online"`
	ExamTime       string `json:"exam_time" form:"exam_time"`
	ExamLocation   string `json:"exam_location" form:"exam_location"`
	ExamOption     int    `json:"exam_option" form:"exam_option"` // 考核方式
	IsCompulsory   bool   `json:"is_compulsory" form:"is_compulsory"`
}

// AddActivityRequest 添加活动请求
type AddActivityRequest struct {
	ActivityName       string  `json:"activity_name" form:"activity_name"`
	StartTime          string  `json:"start_time" form:"start_time"`
	Type               int     `json:"type" form:"type"`
	Location           string  `json:"location" form:"location"`
	Tag                string  `json:"tag" form:"tag"`
	Frequency          int     `json:"frequency" form:"frequency"`
	IsMention          bool    `json:"is_mention" form:"is_mention"` // 是否需要提醒
	AdvanceMentionTime int     `json:"advance_mention_time"`         // 提前提醒时间, 单位是分钟
	Members            []int64 `json:"members" form:"members"`       // 活动成员, 当为集体活动时, 该字段不为空
}

// NavigateRequest 导航请求
type NavigateRequest struct {
	FromId int `json:"from_id"` //开始节点
	DesId  int `json:"des_id"`  //结束节点
}

// FacilityRequest 建筑物信息查询请求
type FacilityRequest struct {
	Count      int64   `json:"count"`              // 返回计数
	FacID      *int64  `json:"fac_id,omitempty"`   // 设施ID，设施ID
	FacName    *string `json:"fac_name,omitempty"` // 设施名，设施名称
	IsSpecific bool    `json:"is_specific"`        // 是否为精确查找，精确查找为True，使用fac_name或fac_id来精确查询；如果不是，则返回建筑物列表
	Offset     int64   `json:"offset"`             // 偏移量
}
