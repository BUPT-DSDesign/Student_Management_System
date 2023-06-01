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

	fmt.Printf("activityInfo: %v\n", *activityInfo)

	// 检查活动与课程是否冲突
	isConflict, validTime := check_conflict.ActivityAndCoursesIsExistConflict(f.userId, activityInfo)

	isConflict1 := false
	// 如果是临时事务, 还需要检测它与其他活动是否冲突
	if activityInfo.Type == 1 {
		isConflict1 = check_conflict.TransactionAndActivityIsExistConflict(f.userId, activityInfo)
	}

	// 只有不冲突的时候才插入数据库数据
	if !isConflict && !isConflict1 {
		println("没问题吧")
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
		if validTime == nil {
			return errors.New("该活动(或临时事务)与课程时间冲突, 并且没有合适的时间段")
		}
		return errors.New(fmt.Sprintf("该活动(或临时事务)与课程时间冲突, 以下是合适的时间段为: %v", validTime))
	}

	return nil
}
