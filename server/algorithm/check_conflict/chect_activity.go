package check_conflict

import (
	"server/model/dao"
	"server/model/entity/system"
	"server/utils"
	"sort"
)

// ActivityAndCoursesIsExistConflict 检查活动和课程是否存在冲突
func ActivityAndCoursesIsExistConflict(userId int64, activity *system.ActivityInfo) (bool, []string) {
	switch activity.Type {
	case 0:
		{
			// 如果是单次活动
			var sectionList []int
			isConflict := false
			week := utils.GetWeek(activity.StartTime)
			var courseIds []int64
			if err := dao.Group.CourseDao.QueryCourseByWeek(week, &courseIds); err != nil {
				return false, nil
			}
			for _, courseId := range courseIds {
				// 先判断这门课程是否是自己的
				var courseInfo system.CourseInfo
				if err := dao.Group.CourseDao.QueryCourseById(courseId, &courseInfo); err != nil {
					return false, nil
				}
				// 如果是自己的课程, 才需要检测冲突
				if courseInfo.IsCompulsory || dao.Group.CourseDao.JudgeIsStudentSelectCourse(userId, courseId) {
					var rawSectionList []int
					if err := dao.Group.CourseDao.QuerySectionListById(courseId, &rawSectionList); err != nil {
						return false, nil
					}
					for _, section := range rawSectionList {
						if utils.GetDayBySection(section) == utils.GetDay(activity.StartTime) {
							sectionList = append(sectionList, section)
						}
					}
				}
			}

			// 将sectionList按照时间顺序排序
			sort.Slice(sectionList, func(i, j int) bool {
				return sectionList[i] < sectionList[j]
			})

			// 根据sectionList得到时间段
			timeList := utils.SplitSectionList(sectionList)
			// 将timeList按照时间顺序排序
			sort.Slice(timeList, func(i, j int) bool {
				return timeList[i][0] < timeList[j][0]
			})

			// 判断hour*60+minute是否在timeList中
			hour, minute := utils.GetHourAndMinute(activity.StartTime)
			for _, time := range timeList {
				if hour*60+minute >= time[0] && hour*60+minute <= time[1] {
					isConflict = true
					break
				}
			}

			if isConflict {
				// 如果冲突, 由于这是单次活动, 所以需要找出找出可行的时间段
				// 下面的错误怎么改
				var validTime []string
				newTimeList := append([][2]int{{360, 360}}, timeList...)
				newTimeList = append(newTimeList, [2]int{1320, 1320})
				for i := 0; i < len(newTimeList)-1; i++ {
					if newTimeList[i+1][0]-newTimeList[i][1] >= 60 {
						// 可行
						startTime := utils.GetFormatTimeByTranVal(newTimeList[i][1])
						endTime := utils.GetFormatTimeByTranVal(newTimeList[i+1][0])
						validTime = append(validTime, startTime+" ~ "+endTime)
					}
				}
				return true, validTime

			}

			// 不冲突
			return false, nil
		}
	case 1:
		{
			isConflict := false
			// 如果是每天活动
			// 根据数据库查询必修课和选修课
			var coursesInfo []*system.CourseInfo
			if err := dao.Group.CourseDao.QueryCourseByUserId(userId, &coursesInfo); err != nil {
				return false, nil
			}
			var sectionList []int
			for _, courseInfo := range coursesInfo {
				var rawSectionList []int
				if err := dao.Group.CourseDao.QuerySectionListById(courseInfo.CourseId, &rawSectionList); err != nil {
					return false, nil
				}
				sectionList = append(sectionList, rawSectionList...)
			}

			// 将sectionList按照时间顺序排序
			sort.Slice(sectionList, func(i, j int) bool {
				return sectionList[i] < sectionList[j]
			})

			// 根据sectionList得到时间段
			timeList := utils.SplitSectionList(sectionList)
			// 将timeList按照时间顺序排序
			sort.Slice(timeList, func(i, j int) bool {
				return timeList[i][0] < timeList[j][0]
			})

			// 判断hour*60+minute是否在timeList中
			hour, minute := utils.GetHourAndMinute(activity.StartTime)
			for _, time := range timeList {
				if hour*60+minute >= time[0] && hour*60+minute <= time[1] {
					isConflict = true
					break
				}
			}

			// 直接返回冲突, 因为这是每天活动, 不需要找出可行的时间段
			return isConflict, nil

		}
	case 2:
		{
			isConflict := false
			// 如果是每周活动
			// 根据活动是在周几得到当天的节次
			activitySectionList := utils.GetSectionListByDay(utils.GetDay(activity.StartTime))
			// 查找在这个节次表中的课程id
			// 需要开一个map, key是课程id, values是这个课程的节次
			courseIdAndSectionMap := make(map[int64][]int)
			for _, section := range activitySectionList {
				var courseIdList []int64
				if err := dao.Group.CourseDao.QueryCourseBySection(section, &courseIdList); err != nil {
					return false, nil
				}
				for _, courseId := range courseIdList {
					courseIdAndSectionMap[courseId] = append(courseIdAndSectionMap[courseId], section)
				}
			}

			var sectionList []int
			// 遍历map
			for courseId, rawSectionList := range courseIdAndSectionMap {
				// 先判断这门课程是否是自己的
				var courseInfo system.CourseInfo
				if err := dao.Group.CourseDao.QueryCourseById(courseId, &courseInfo); err != nil {
					return false, nil
				}
				// 如果是自己的课程, 才需要检测冲突
				if courseInfo.IsCompulsory || dao.Group.CourseDao.JudgeIsStudentSelectCourse(userId, courseId) {
					sectionList = append(sectionList, rawSectionList...)
				}
			}

			// 将sectionList按照时间顺序排序
			sort.Slice(sectionList, func(i, j int) bool {
				return sectionList[i] < sectionList[j]
			})

			// 根据sectionList得到时间段
			timeList := utils.SplitSectionList(sectionList)
			// 将timeList按照时间顺序排序
			sort.Slice(timeList, func(i, j int) bool {
				return timeList[i][0] < timeList[j][0]
			})

			// 判断hour*60+minute是否在timeList中
			hour, minute := utils.GetHourAndMinute(activity.StartTime)
			for _, time := range timeList {
				if hour*60+minute >= time[0] && hour*60+minute <= time[1] {
					isConflict = true
					break
				}
			}

			// 直接返回冲突, 因为这是每周活动, 不需要找出可行的时间段
			return isConflict, nil
		}
	}

	return false, nil
}
