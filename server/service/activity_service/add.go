package activity_service

import (
	"errors"
	"fmt"
	"server/algorithm/check_conflict"
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/utils"
)

type addFlow struct {
	// 包含handler层传来的参数等
	userId             int64
	addActivityRequest common.AddActivityRequest
}

func newAddFlow(userId int64, addActivityRequest common.AddActivityRequest) *addFlow {
	return &addFlow{userId: userId, addActivityRequest: addActivityRequest}
}

func (s *server) DoAdd(userId int64, addActivityRequest common.AddActivityRequest) error {
	return newAddFlow(userId, addActivityRequest).do()
}

func (f *addFlow) do() error {
	if err := f.checkNum(); err != nil {
		return err
	}
	if err := f.run(); err != nil {
		return err
	}

	return nil
}

// 检验参数
func (f *addFlow) checkNum() error {
	// 这里userId是上层解析过来的, 一定合法
	return nil
}

func (f *addFlow) run() error {
	// 生成活动id
	activityId, err := utils.GenerateId()
	if err != nil {
		return err
	}

	// 将addActivityRequest转换为activityInfo
	activityInfo := &system.ActivityInfo{
		ActivityId:         activityId,
		ActivityName:       f.addActivityRequest.ActivityName,
		UserId:             f.userId,
		StartTime:          f.addActivityRequest.StartTime,
		Type:               f.addActivityRequest.Type,
		Location:           f.addActivityRequest.Location,
		Tag:                f.addActivityRequest.Tag,
		Frequency:          f.addActivityRequest.Frequency,
		IsMention:          f.addActivityRequest.IsMention,
		AdvanceMentionTime: f.addActivityRequest.AdvanceMentionTime,
	}

	// 检查活动与课程是否冲突
	isConflict, validTime, validTimeList := check_conflict.ActivityAndCoursesIsExistConflict(f.userId, activityInfo)

	//isConflict1 := false
	//// 如果是临时事务, 还需要检测它与其他活动是否冲突
	//if activityInfo.Type == 1 {
	//	isConflict1 = check_conflict.TransactionAndActivityIsExistConflict(f.userId, activityInfo)
	//}

	// 只有不冲突的时候才插入数据库数据
	if !isConflict {
		// 如果是集体活动, 则其他成员也需要参与冲突检测
		// 创建一个bool数组和一个时间段数组
		var isMembersConflict []bool
		var membersValidTimeList [][][2]int
		// 先将validTimeList加入membersValidTimeList
		membersValidTimeList = append(membersValidTimeList, validTimeList)
		if len(f.addActivityRequest.Members) != 0 {
			for _, memberId := range f.addActivityRequest.Members {
				isMemberConflict, _, memberValidTimeList := check_conflict.ActivityAndCoursesIsExistConflict(memberId, activityInfo)
				isMembersConflict = append(isMembersConflict, isMemberConflict)
				membersValidTimeList = append(membersValidTimeList, memberValidTimeList)
			}
		}

		membersIsConflict := false
		for _, isMemberConflict := range isMembersConflict {
			if isMemberConflict {
				membersIsConflict = true
				break
			}
		}

		if membersIsConflict {
			// 如果有成员冲突, 则遍历membersValidTimeList, 看是否可以找出合适的时间段
			// 开一个1500长度的数组, 用来记录每个时间段的冲突次数
			var validNum [1500]int
			for _, memberValidTimeList := range membersValidTimeList {
				for _, v := range memberValidTimeList {
					for i := v[0]; i <= v[1]; i++ {
						validNum[i]++
					}
				}
			}
			peopleSize := len(f.addActivityRequest.Members) + 1
			// 遍历validNum, 找到validNum[i]等于peopleSize的时间段
			var finalValidTime [][2]int
			l := 0
			r := 0
			for i := 0; i < 1500; i++ {
				if validNum[i] == peopleSize {
					if l == 0 {
						l = i
					} else {
						r = i
					}
				} else {
					if r-l >= 60 {
						finalValidTime = append(finalValidTime, [2]int{l, r})
					}
					l = 0
					r = 0
				}
			}
			if len(finalValidTime) == 0 {
				// 如果没有可行时间，集体活动给出当天冲突最少的三个时间
				var finalValidTime1 [][2]int
				peopleSize--
				for ; peopleSize >= 1; peopleSize-- {
					l := 0
					r := 0
					for i := 0; i < 1500; i++ {
						if validNum[i] == peopleSize {
							if l == 0 {
								l = i
							} else {
								r = i
							}
						} else {
							if r-l >= 60 {
								finalValidTime1 = append(finalValidTime1, [2]int{l, r})
							}
							l = 0
							r = 0
						}
					}
				}

				// 如果finalValidTime1为空，说明没有可行时间
				if len(finalValidTime1) == 0 {
					return errors.New("该集体活动与某些成员时间冲突, 且当天任何成员都没有空闲的时间段")
				} else {
					var result []string
					for _, v := range finalValidTime1 {
						startTime := utils.GetFormatTimeByTranVal(v[0])
						endTime := utils.GetFormatTimeByTranVal(v[1])
						result = append(result, startTime+" ~ "+endTime)
					}

					return errors.New(fmt.Sprintf("该集体活动与某些成员时间冲突, 以下是冲突人数最少的时间段排序\n: %v", result))
				}
			} else {
				var result []string
				for _, v := range finalValidTime {
					startTime := utils.GetFormatTimeByTranVal(v[0])
					endTime := utils.GetFormatTimeByTranVal(v[1])
					result = append(result, startTime+" ~ "+endTime)
				}
				return errors.New(fmt.Sprintf("该集体活动与某些成员时间冲突, 以下是当天成员都有空的时间段\n: %v", result))
			}
		}

		// 将activityInfo插入数据库
		if err = dao.Group.ActivityDao.AddActivity(activityInfo); err != nil {
			return err
		}

		// 遍历f.addActivityRequest.Members, 如果是集体活动, 则其成员也需要加入该活动
		for _, memberId := range f.addActivityRequest.Members {
			println(memberId)
			// 生成一个活动id
			activityId, err = utils.GenerateId()
			if err != nil {
				return err
			}
			newActivityInfo := &system.ActivityInfo{
				ActivityId:         activityId,
				ActivityName:       f.addActivityRequest.ActivityName,
				UserId:             memberId,
				StartTime:          f.addActivityRequest.StartTime,
				Type:               f.addActivityRequest.Type,
				Location:           f.addActivityRequest.Location,
				Tag:                f.addActivityRequest.Tag,
				Frequency:          f.addActivityRequest.Frequency,
				IsMention:          f.addActivityRequest.IsMention,
				AdvanceMentionTime: f.addActivityRequest.AdvanceMentionTime,
			}
			// 将newActivityInfo插入数据库
			if err = dao.Group.ActivityDao.AddActivity(newActivityInfo); err != nil {
				return err
			}
		}
	} else {
		if len(f.addActivityRequest.Members) == 0 {
			if validTime == nil {
				return errors.New("该活动(或临时事务)与其他时间冲突, 并且没有合适的时间段")
			}
			return errors.New(fmt.Sprintf("该活动(或临时事务)与其他时间冲突, 以下是当天合适的时间段\n: %v", validTime))
		} else {
			// 如果是集体活动, 则其他成员也需要参与冲突检测
			// 创建一个bool数组和一个时间段数组
			var isMembersConflict []bool
			var membersValidTimeList [][][2]int
			// 先将validTimeList加入membersValidTimeList
			membersValidTimeList = append(membersValidTimeList, validTimeList)
			if len(f.addActivityRequest.Members) != 0 {
				for _, memberId := range f.addActivityRequest.Members {
					isMemberConflict, _, memberValidTimeList := check_conflict.ActivityAndCoursesIsExistConflict(memberId, activityInfo)
					isMembersConflict = append(isMembersConflict, isMemberConflict)
					membersValidTimeList = append(membersValidTimeList, memberValidTimeList)
				}
			}

			membersIsConflict := false
			for _, isMemberConflict := range isMembersConflict {
				if isMemberConflict {
					membersIsConflict = true
					break
				}
			}

			if membersIsConflict {
				// 如果有成员冲突, 则遍历membersValidTimeList, 看是否可以找出合适的时间段
				// 开一个1500长度的数组, 用来记录每个时间段的冲突次数
				var validNum [1500]int
				for _, memberValidTimeList := range membersValidTimeList {
					for _, v := range memberValidTimeList {
						for i := v[0]; i <= v[1]; i++ {
							validNum[i]++
						}
					}
				}
				peopleSize := len(f.addActivityRequest.Members) + 1
				// 遍历validNum, 找到validNum[i]等于peopleSize的时间段
				var finalValidTime [][2]int
				l := 0
				r := 0
				for i := 0; i < 1500; i++ {
					if validNum[i] == peopleSize {
						if l == 0 {
							l = i
						} else {
							r = i
						}
					} else {
						if r-l >= 60 {
							finalValidTime = append(finalValidTime, [2]int{l, r})
						}
						l = 0
						r = 0
					}
				}
				if len(finalValidTime) == 0 {
					// 如果没有可行时间，集体活动给出当天冲突最少的三个时间
					var finalValidTime1 [][2]int
					peopleSize--
					for ; peopleSize >= 1; peopleSize-- {
						l := 0
						r := 0
						for i := 0; i < 1500; i++ {
							if validNum[i] == peopleSize {
								if l == 0 {
									l = i
								} else {
									r = i
								}
							} else {
								if r-l >= 60 {
									finalValidTime1 = append(finalValidTime1, [2]int{l, r})
								}
								l = 0
								r = 0
							}
						}
					}

					// 如果finalValidTime1为空，说明没有可行时间
					if len(finalValidTime1) == 0 {
						return errors.New("该集体活动与某些成员时间冲突, 且当天任何成员都没有空闲的时间段")
					} else {
						var result []string
						for _, v := range finalValidTime1 {
							startTime := utils.GetFormatTimeByTranVal(v[0])
							endTime := utils.GetFormatTimeByTranVal(v[1])
							result = append(result, startTime+" ~ "+endTime)
						}
						return errors.New(fmt.Sprintf("该集体活动与某些成员时间冲突, 以下是冲突人数最少的时间段排序\n: %v", finalValidTime1))
					}
				} else {
					var result []string
					for _, v := range finalValidTime {
						startTime := utils.GetFormatTimeByTranVal(v[0])
						endTime := utils.GetFormatTimeByTranVal(v[1])
						result = append(result, startTime+" ~ "+endTime)
					}
					return errors.New(fmt.Sprintf("该集体活动与某些成员时间冲突, 以下是当天成员都有空的时间段\n: %v", finalValidTime))
				}
			}
		}
	}

	return nil
}
