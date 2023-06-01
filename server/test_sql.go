package main

import (
	"bufio"
	"database/sql"
	"encoding/json"
	"fmt"
	"log"
	"os"

	_ "github.com/go-sql-driver/mysql"
)

func main() {
	// 创建MySQL数据库连接
	db, err := sql.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/sms")
	if err != nil {
		log.Fatalf("failed to connect to database: %v", err)
	}
	defer db.Close()

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		sqlStr := scanner.Text()

		// 预处理 SQL 语句
		stmt, err := db.Prepare(sqlStr)
		if err != nil {
			log.Printf("failed to prepare SQL statement: %v", err)
			continue
		}
		defer stmt.Close()

		// 执行 SQL 语句
		rows, err := stmt.Query()
		if err != nil {
			log.Printf("failed to execute SQL statement: %v", err)
			continue
		}
		defer rows.Close()

		// 将查询结果转换为JSON
		columns, err := rows.Columns()
		if err != nil {
			log.Printf("failed to get column names: %v", err)
			continue
		}
		count := len(columns)
		tableData := make([]map[string]interface{}, 0)
		values := make([]interface{}, count)
		valuePtrs := make([]interface{}, count)
		for rows.Next() {
			for i := 0; i < count; i++ {
				valuePtrs[i] = &values[i]
			}
			if err := rows.Scan(valuePtrs...); err != nil {
				log.Printf("failed to scan查询结果: %v", err)
				continue
			}
			entry := make(map[string]interface{})
			for i, col := range columns {
				val := values[i]
				b, ok := val.([]byte)
				if ok {
					entry[col] = string(b)
				} else {
					entry[col] = val
				}
			}
			tableData = append(tableData, entry)
		}
		jsonData, err := json.Marshal(tableData)
		if err != nil {
			log.Printf("failed to marshal query result to JSON: %v", err)
			continue
		}

		// 创建一个map
		result := make(map[string]interface{})
		result["status_code"] = 0
		result["status_msg"] = "success"
		result["data"] = string(jsonData)
		resultStr, err := json.Marshal(result)
		if err != nil {
			log.Printf("failed to marshal response to JSON: %v", err)
			continue
		}
		fmt.Println(string(resultStr))
	}
}
