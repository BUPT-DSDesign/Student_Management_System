package dao

import (
	"fmt"
	"server/model/entity/system"
	"server/utils"
)

func (s *activityDao) AddActivity(activityInfo *system.ActivityInfo) error {
	// 将activityInfo转换为sql语句
	sqlStr := fmt.Sprintf("INSERT INTO activity_info VALUES('%v', '%v', '%v', '%v', '%v', '%v', '%v', '%v', '%v')",
		activityInfo.ActivityId,
		activityInfo.ActivityName,
		activityInfo.UserId,
		activityInfo.StartTime,
		activityInfo.Type,
		activityInfo.Location,
		activityInfo.Tag,
		activityInfo.Frequency,
		utils.BoolToInt8(activityInfo.IsMention),
	)

	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	return nil
}

func (s *activityDao) DeleteActivity(activityId int64) error {
	sqlStr := fmt.Sprintf("DELETE FROM activity_info WHERE activity_id = '%v'", activityId)

	if err := db.ExecSql(sqlStr); err != nil {
		return err
	}

	return nil
}
