package main

import (
	"bufio"
	"database/sql"
	"encoding/json"
	"errors"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
	"log"
	"os"
)

func main() {
	// 创建MySQL数据库连接
	err := errors.New("")
	db, err := sql.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/sms")
	if err != nil {
		panic(err)
	}
	defer db.Close()

	reader := bufio.NewReader(os.Stdin)
	for {
		sqlStr, err := reader.ReadString('\n')
		rows, err := db.Query(sqlStr)
		if err != nil {
			log.Fatal(err)
		}
		defer rows.Close()
		// 将查询结果转换为JSON
		columns, err := rows.Columns()
		if err != nil {
			log.Fatal(err)
		}
		count := len(columns)
		tableData := make([]map[string]interface{}, 0)
		values := make([]interface{}, count)
		valuePtrs := make([]interface{}, count)
		for rows.Next() {
			for i := 0; i < count; i++ {
				valuePtrs[i] = &values[i]
			}
			rows.Scan(valuePtrs...)
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
			log.Fatal(err)
		}

		// 创建一个map
		result := make(map[string]interface{})
		result["status_code"] = 0
		result["status_msg"] = "success"
		result["data"] = string(jsonData)
		resultStr, err := json.Marshal(result)
		fmt.Println(string(resultStr))
	}
}
