package polling_service

import (
	"errors"
	"server/model/dao"
	"server/model/entity/system"
	"server/utils"
	"sort"
)

type isCourseArriveFlow struct {
	// 包含handler层传来的参数等
	userId    int64
	curTime   string
	timeUnion utils.TimeUnion
}

func newIsCourseArriveFlow(userId int64, curTime string) *isCourseArriveFlow {
	return &isCourseArriveFlow{userId: userId, curTime: curTime}
}

func (s *server) DoIsCourseArrive(userId int64, curTime string) (*system.CourseInfo, error) {
	return newIsCourseArriveFlow(userId, curTime).do()
}

func (f *isCourseArriveFlow) do() (*system.CourseInfo, error) {
	var courseInfo *system.CourseInfo

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&courseInfo); err != nil {
		return nil, err
	}

	return courseInfo, nil
}

// 检验参数
func (f *isCourseArriveFlow) checkNum() error {
	if !utils.CheckTimeFormat(f.curTime) {
		return errors.New("时间格式错误")
	}
	// 将f.curTime转换为timeUnion
	hour, minute := utils.GetHourAndMinute(f.curTime)
	f.timeUnion = utils.TimeUnion{
		Week:   utils.GetWeek(f.curTime),
		Day:    utils.GetDay(f.curTime),
		Hour:   hour,
		Minute: minute,
	}
	return nil
}

func (f *isCourseArriveFlow) run(courseInfo **system.CourseInfo) error {
	// 先查出来必修课程
	courseInfos := new([]*system.CourseInfo)
	if err := dao.Group.CourseDao.QueryCompulsoryCourse(&courseInfos); err != nil {
		return err
	}
	// 然后遍历所有必修课
	for _, course := range *courseInfos {
		// 根据course.CourseId查询出所有的周次
		var weekSchedule []int
		if err := dao.Group.CourseDao.QueryWeekScheduleById(course.CourseId, &weekSchedule); err != nil {
			return err
		}

		// 打印weekSchedule
		// 将weekSchedule排序
		sort.Ints(weekSchedule)
		target := utils.GetWeek(f.curTime)
		index := sort.SearchInts(weekSchedule, target)
		if index >= len(weekSchedule) || weekSchedule[index] != target {
			continue
		}

		// 根据course.CourseId查询出所有的节次
		var rawSectionList []int
		if err := dao.Group.CourseDao.QuerySectionListById(course.CourseId, &rawSectionList); err != nil {
			return err
		}

		// 打印rawSectionList
		var sectionList []int
		for _, section := range rawSectionList {
			day, _, _ := utils.TransSectionToTime(section)
			if day == utils.GetDay(f.curTime) {
				sectionList = append(sectionList, section)
			}
		}

		// 将节次转换为时间
		timeList := utils.SplitSectionList(sectionList)

		// 判断当前时间是否与课程开始时间相隔1小时
		for _, v := range timeList {
			timeUnion := utils.TimeUnion{
				Week:   utils.GetWeek(f.curTime),
				Day:    utils.GetDay(f.curTime),
				Hour:   v[0] / 60,
				Minute: v[0] % 60,
			}
			// 如果当前时间与课程开始时间相隔1小时
			if utils.IsSatisfied(f.timeUnion, timeUnion, 60) {
				*courseInfo = course
				return nil
			}
		}
	}

	// 查出来选修课程
	courseInfos = new([]*system.CourseInfo)
	if err := dao.Group.CourseDao.QueryElectiveCourse(f.userId, &courseInfos); err != nil {
		return err
	}
	// 然后遍历所有选修课
	for _, course := range *courseInfos {
		// 根据course.CourseId查询出所有的周次
		var weekSchedule []int
		if err := dao.Group.CourseDao.QueryWeekScheduleById(course.CourseId, &weekSchedule); err != nil {
			return err
		}
		sort.Ints(weekSchedule)
		target := utils.GetWeek(f.curTime)
		index := sort.SearchInts(weekSchedule, target)
		if index >= len(weekSchedule) || weekSchedule[index] != target {
			continue
		}

		// 根据course.CourseId查询出所有的节次
		var rawSectionList []int
		if err := dao.Group.CourseDao.QuerySectionListById(course.CourseId, &rawSectionList); err != nil {
			return err
		}
		var sectionList []int
		for _, section := range rawSectionList {
			day, _, _ := utils.TransSectionToTime(section)
			if day == utils.GetDay(f.curTime) {
				sectionList = append(sectionList, section)
			}
		}

		// 将节次转换为时间
		timeList := utils.SplitSectionList(sectionList)

		// 判断当前时间是否与课程开始时间相隔1小时
		for _, v := range timeList {
			timeUnion := utils.TimeUnion{
				Week:   utils.GetWeek(f.curTime),
				Day:    utils.GetDay(f.curTime),
				Hour:   v[0] / 60,
				Minute: v[0] % 60,
			}
			// 如果当前时间与课程开始时间相隔1小时
			if utils.IsSatisfied(f.timeUnion, timeUnion, 60) {
				*courseInfo = course
				return nil
			}
		}
	}

	return nil
}
