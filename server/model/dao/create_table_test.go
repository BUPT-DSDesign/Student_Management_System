package dao

import "testing"

func TestSqlStr(t *testing.T) {
	sql := "select * from user_info"
	err := db.ExecSql(sql)
	if err != nil {
		return
	}

}
