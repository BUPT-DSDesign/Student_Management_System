package dao

func createUserTable() error {
	// 创建用户表
	sql := "CREATE TABLE user_info (" +
		"user_id BIGINT NOT NULL," +
		"username VARCHAR(20) NOT NULL," +
		"password VARCHAR(100) NOT NULL," +
		"salt VARCHAR(20) NOT NULL," +
		"student_id VARCHAR(20) NOT NULL," +
		"is_admin TINYINT NOT NULL," +
		"avatar_url VARCHAR(100) NOT NULL," +
		"signature VARCHAR(100) NOT NULL," +
		"PRIMARY KEY (user_id)" +
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"
	println(sql)
	if err := db.ExecSql(sql); err != nil {
		return err
	}
	return nil
}

func createCourseTable() error {
	// 创建课程表
	sql := "CREATE TABLE course_info (" +
		"course_name VARCHAR(20) NOT NULL," +
		"course_id BIGINT NOT NULL," +
		"teacher VARCHAR(20) NOT NULL," +
		"contact VARCHAR(20) NOT NULL," +
		"classroom VARCHAR(20) NOT NULL," +
		"is_course_online TINYINT NOT NULL," +
		"exam_time VARCHAR(20) NOT NULL," +
		"exam_location VARCHAR(20) NOT NULL," +
		"exam_option VARCHAR(20) NOT NULL," +
		"is_compulsory TINYINT NOT NULL," +
		"PRIMARY KEY (course_id)" +
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"
	if err := db.ExecSql(sql); err != nil {
		return err
	}
	return nil
}

func createStudentCourseTable() error {
	// 创建学生-课程表
	sql := "CREATE TABLE student_course (" +
		"id BIGINT NOT NULL," +
		"student_id BIGINT NOT NULL," +
		"course_id BIGINT NOT NULL," +
		"PRIMARY KEY (id)" +
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"
	if err := db.ExecSql(sql); err != nil {
		return err
	}
	return nil
}

func createLogTable() error {
	// 创建日志表
	sql := "CREATE TABLE log_info (" +
		"log_id BIGINT NOT NULL," +
		"create_time VARCHAR(20) NOT NULL," +
		"content VARCHAR(100) NOT NULL," +
		"user_id BIGINT  NOT NULL," +
		"PRIMARY KEY (log_id)" +
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"
	if err := db.ExecSql(sql); err != nil {
		return err
	}
	return nil
}

func createActivityTable() error {
	// 创建活动表
	sql := "CREATE TABLE activity_info (" +
		"activity_id BIGINT NOT NULL," +
		"activity_name VARCHAR(20) NOT NULL," +
		"user_id BIGINT  NOT NULL," +
		"start_time VARCHAR(20) NOT NULL," +
		"type INT NOT NULL," +
		"location VARCHAR(20) NOT NULL," +
		"tag VARCHAR(20) NOT NULL," +
		"frequency INT NOT NULL," +
		"is_mention TINYINT NOT NULL," +
		"PRIMARY KEY (activity_id)" +
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"

	if err := db.ExecSql(sql); err != nil {
		return err
	}

	return nil
}

func createCourseSectionTable() error {
	// 创建课程-节次表
	sql := "CREATE TABLE course_section (" +
		"id BIGINT NOT NULL," +
		"course_id BIGINT NOT NULL," +
		"section_id INT NOT NULL" +
		"PRIMARY KEY (id)" +
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"
	if err := db.ExecSql(sql); err != nil {
		return err
	}
	return nil
}

func createCourseWeekTable() error {
	// 创建课程-周次表
	sql := "CREATE TABLE course_week (" +
		"id BIGINT NOT NULL," +
		"course_id BIGINT NOT NULL," +
		"week_id INT NOT NULL" +
		"PRIMARY KEY (id)" +
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"
	if err := db.ExecSql(sql); err != nil {
		return err
	}
	return nil
}

func init() {
	//if err := createUserTable(); err != nil {
	//	log.Fatal(err)
	//}
}
