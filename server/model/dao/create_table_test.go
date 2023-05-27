package dao

import "testing"

func TestSqlStr(t *testing.T) {
	sql := "CREATE TABLE user_info (" +
		"user_id BIGINT NOT NULL," +
		"username VARCHAR(20) NOT NULL," +
		"password VARCHAR(100) NOT NULL," +
		"salt VARCHAR(20) NOT NULL," +
		"student_id VARCHAR(20) NOT NULL," +
		"is_admin TINYINT NOT NULL," +
		"avatar_url VARCHAR(100) NOT NULL," +
		"signature VARCHAR(100) NOT NULL," +
		"PRIMARY KEY (user_id)," +
		"UNIQUE KEY `username` (`username`)" +
		");"
	println(sql)
}
