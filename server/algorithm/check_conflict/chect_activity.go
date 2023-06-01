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
			/*
				如果是单次活动, 则检测策略为:
				1.先根据活动的周数查询这周的所有课程
				2.再将这些课程中不是自己的课程过滤掉
				3.之后将课程属于活动当天的节次加入到一个数组中
				4.最后将sectionList数组转换为时间段来检查活动时间是否和这些课程时间冲突
			*/
			isConflict := false

			// 活动所在的周数
			week := utils.GetWeek(activity.StartTime)

			// 现根据第几周查询这周的所有课程id
			var courseIds []int64
			if err := dao.Group.CourseDao.QueryCourseByWeek(week, &courseIds); err != nil {
				return false, nil
			}

			// 得到sectionList
			var sectionList []int
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

			// 判断活动时间是否在timeList中
			hour, minute := utils.GetHourAndMinute(activity.StartTime)
			for _, time := range timeList {
				if hour*60+minute >= time[0] && hour*60+minute <= time[1] || hour*60+minute+60 >= time[0] && hour*60+minute+60 <= time[1] {
					isConflict = true
					break
				}
			}

			// 如果冲突, 由于这是单次活动, 所以需要找出找出可行的时间段

			if isConflict {
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
			/*
				如果是每天活动, 则检测策略为:
				1.先查询这个用户的所有课程
				2.判断这个课程最后一节课所在的周数是否大于活动所在的周数, 如果小于就不需要检测冲突
				3.如果大于, 可以直接把这个课程的节次加入到sectionList中
				4.如果等于, 就需要将节次满足大于这个活动周几的节次加入到sectionList中
				5.最后将sectionList数组转换为时间段来检查活动时间是否和这些课程时间冲突
			*/
			isConflict := false

			// 活动所在的周数
			week := utils.GetWeek(activity.StartTime)
			// 活动所在的星期
			day := utils.GetDay(activity.StartTime)

			// 先根据userId查询这个用户的所有课程
			var coursesInfo []*system.CourseInfo
			if err := dao.Group.CourseDao.QueryCourseByUserId(userId, &coursesInfo); err != nil {
				return false, nil
			}

			// 得到sectionList
			var sectionList []int
			for _, courseInfo := range coursesInfo {
				// 先查询这门课程的周数
				var rawWeekList []int
				if err := dao.Group.CourseDao.QueryWeekScheduleById(courseInfo.CourseId, &rawWeekList); err != nil {
					return false, nil
				}
				maxWeek := utils.QueryMaxNum(rawWeekList)

				// 如果这个课程的最大周数小于活动所在的周数, 则一定不冲突
				if maxWeek < week {
					continue
				}

				// 否则查询这门课程的节次
				var rawSectionList []int
				if err := dao.Group.CourseDao.QuerySectionListById(courseInfo.CourseId, &rawSectionList); err != nil {
					return false, nil
				}
				if maxWeek > week {
					sectionList = append(sectionList, rawSectionList...)
				} else {
					// 遍历rawSectionList, 如果这门课程的节次在活动所在的星期几后面, 则加入sectionList
					for _, section := range rawSectionList {
						if utils.GetDayBySection(section) >= day {
							sectionList = append(sectionList, section)
						}
					}
				}
			}

			// 将sectionList重复元素去除
			sectionList = utils.RemoveRepeatedElement(sectionList)

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

			// 判断活动时间是否在timeList中
			hour, minute := utils.GetHourAndMinute(activity.StartTime)
			for _, time := range timeList {
				if hour*60+minute >= time[0] && hour*60+minute <= time[1] || hour*60+minute+60 >= time[0] && hour*60+minute+60 <= time[1] {
					isConflict = true
					break
				}
			}

			// 直接返回冲突, 因为这是每天活动, 不需要找出可行的时间段
			return isConflict, nil

		}
	case 2:
		{
			/*
				如果是每周活动, 则检测策略为:
				1.先根据这个活动的日子查询对应的节次
				2.根据这些节次查询这个课程, 得到一个map, key是课程id, values是这个课程的节次
				3.遍历map, 判断key即课程id是否是自己的课程, 如果不是, 则continue
				4.如果是, 就查询这个课程最后一节课所在的周数是否大于活动所在的周数, 如果小于就不需要检测冲突
				5.如果大于或者等于, 就将value即对应的节次加入到sectionList中
				6.最后将sectionList数组转换为时间段来检查活动时间是否和这些课程时间冲突
			*/
			isConflict := false

			// 活动最早开始的周数
			week := utils.GetWeek(activity.StartTime)

			// 根据活动是在周几得到当天的节次
			activitySectionList := utils.GetSectionListByDay(utils.GetDay(activity.StartTime))

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

			// 得到sectionList
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
					// 然后查询这么课程的周数
					var rawWeekList []int
					if err := dao.Group.CourseDao.QueryWeekScheduleById(courseId, &rawWeekList); err != nil {
						return false, nil
					}
					maxWeek := utils.QueryMaxNum(rawWeekList)
					if maxWeek < week {
						continue
					}
					sectionList = append(sectionList, rawSectionList...)
				}
			}

			// 将sectionList重复元素去除
			sectionList = utils.RemoveRepeatedElement(sectionList)

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

			// 判断活动时间是否在timeList中
			hour, minute := utils.GetHourAndMinute(activity.StartTime)
			for _, time := range timeList {
				if hour*60+minute >= time[0] && hour*60+minute <= time[1] || hour*60+minute+60 >= time[0] && hour*60+minute+60 <= time[1] {
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

// TransactionAndActivityIsExistConflict 检测事务和活动是否冲突
func TransactionAndActivityIsExistConflict(userId int64, activity *system.ActivityInfo) bool {
	// 先根据userId查询出学生的活动
	activities := new([]*system.ActivityInfo)
	if err := dao.Group.ActivityDao.QueryAllActivityByUserId(userId, &activities); err != nil {
		return false
	}

	// 遍历activities, 判断是否冲突
	for _, activityInfo := range *activities {
		// 如果activityInfo是事务, 就不需要检测, 因为一个时间可以同时做多个事务
		if activityInfo.Type == 1 {
			continue
		}

		switch activityInfo.Frequency {
		case 0:
			{
				// 如果是一次性活动, 就需要检测
				if utils.IsBetween(activity.StartTime, activityInfo.StartTime, utils.AddByDuration(activityInfo.StartTime, 60)) {
					return true
				}
			}
		case 1:
			{
				// 如果是每天活动, 就需要检测
				if !utils.IsLessThan(activity.StartTime, activity.StartTime) {
					// 得到各自的小时和分钟
					hour, minute := utils.GetHourAndMinute(activity.StartTime)
					activityHour, activityMinute := utils.GetHourAndMinute(activityInfo.StartTime)
					if hour*60+minute >= activityHour*60+activityMinute && hour*60+minute <= activityHour*60+activityMinute+60 {
						return true
					}
				}
			}
		case 2:
			{
				// 如果是每周活动, 就需要检测
				if !utils.IsLessThan(activity.StartTime, activity.StartTime) && utils.GetDay(activity.StartTime) == utils.GetDay(activityInfo.StartTime) {
					// 得到各自的小时和分钟
					hour, minute := utils.GetHourAndMinute(activity.StartTime)
					activityHour, activityMinute := utils.GetHourAndMinute(activityInfo.StartTime)
					if hour*60+minute >= activityHour*60+activityMinute && hour*60+minute <= activityHour*60+activityMinute+60 {
						return true
					}
				}
			}
		}
	}

	return false
}
