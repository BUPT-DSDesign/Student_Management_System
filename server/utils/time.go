package utils

/*
	时间工具包, 用于处理时间相关的操作
*/

type TimeUnion struct {
	Week   int // 第几周
	Day    int // 星期几
	Hour   int // 几点
	Minute int // 几分
}

// CheckTimeFormat 检查时间格式是否正确
// 时间格式为: 第几周-星期几-几点:几分, 例如: 第1周-星期1-08:00
func CheckTimeFormat(time string) bool {
	if len(time) != 21 && len(time) != 22 {
		return false
	}

	if len(time) == 21 {
		if time[0:3] != "第" || time[4:7] != "周" || time[7:8] != "-" || time[8:11] != "星" || time[11:14] != "期" || time[15:16] != "-" || time[18:19] != ":" {
			return false
		}
		return true
	}

	if len(time) == 22 {
		if time[0:3] != "第" || time[5:8] != "周" || time[8:9] != "-" || time[9:12] != "星" || time[12:15] != "期" || time[16:17] != "-" || time[19:20] != ":" {
			return false
		}
		return true
	}
	return true
}

// GetWeek 获取周数
func GetWeek(time string) int {
	if len(time) == 21 {
		return int(time[3] - '0')
	} else {
		return int((time[3]-'0')*10) + int(time[4]-'0')
	}
}

// GetDay 获取星期几
func GetDay(time string) int {
	if len(time) == 21 {
		return int(time[14] - '0')
	} else {
		return int(time[15] - '0')
	}
}

// GetHourAndMinute 获取几点几分
func GetHourAndMinute(time string) (int, int) {
	if len(time) == 21 {
		return int(time[16]-'0')*10 + int(time[17]-'0'), int(time[19]-'0')*10 + int(time[20]-'0')
	} else {
		return int(time[17]-'0')*10 + int(time[18]-'0'), int(time[20]-'0')*10 + int(time[21]-'0')
	}
}

// TransSectionToTime 将课程节次转换为时间, 1 -> 星期一: 8:00
func TransSectionToTime(section int) (int, int, int) {
	day := (section-1)/9 + 1
	hour := (section-1)%9 + 8
	if (section-1)%9 > 3 {
		hour++
	}
	minute := 0
	return day, hour, minute
}

// IsSatisfied 判断两个时间是否满足条件 time1 + duration = time2 ,duration是分钟
func IsSatisfied(time1 TimeUnion, time2 TimeUnion, duration int) bool {
	if time1.Week != time2.Week {
		return false
	}
	if time1.Day != time2.Day {
		return false
	}

	if time1.Hour*60+time1.Minute+duration != time2.Hour*60+time2.Minute {
		return false
	}
	return true
}

// SplitSectionList 将课程节次列表分割为时间段列表
func SplitSectionList(sectionList []int) [][2]int {
	var timeList [][2]int
	l := 0
	for i := 0; i < len(sectionList); i++ {
		if i == 0 {
			// 课程节次转换为时间
			_, hour, minute := TransSectionToTime(sectionList[i])
			l = hour*60 + minute
		} else if i < len(sectionList)-1 {
			if sectionList[i]-sectionList[i-1] != 1 {
				// 课程节次转换为时间
				_, hour, minute := TransSectionToTime(sectionList[i-1])
				timeList = append(timeList, [2]int{l, (hour+1)*60 + minute})
				_, hour, minute = TransSectionToTime(sectionList[i])
				l = hour*60 + minute
			}
		}
		if i == len(sectionList)-1 {
			// 课程节次转换为时间
			_, hour, minute := TransSectionToTime(sectionList[i])
			if i != 0 && sectionList[i]-sectionList[i-1] == 1 {
				timeList = append(timeList, [2]int{l, (hour+1)*60 + minute})
			} else {
				if i == 0 {
					timeList = append(timeList, [2]int{l, (hour+1)*60 + minute})
				} else {
					_, hour1, minute1 := TransSectionToTime(sectionList[i-1])
					timeList = append(timeList, [2]int{l, (hour1+1)*60 + minute1})
					timeList = append(timeList, [2]int{hour*60 + minute, (hour+1)*60 + minute})
				}
			}
		}

	}
	return timeList
}
