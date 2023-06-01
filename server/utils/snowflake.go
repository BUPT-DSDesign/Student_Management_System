package utils

import (
	"strconv"
	"time"

	"github.com/bwmarrin/snowflake"
)

// GenerateId 雪花算法生成用户Id
func GenerateId() (int64, error) {
	// 创建一个节点
	node, err := snowflake.NewNode(time.Now().Unix()%1024 + 1)
	if err != nil {
		return int64(0), err
	}

	id := node.Generate().Int64()
	idStr := strconv.FormatInt(id, 10)
	idStr = idStr[6:]
	id, err = strconv.ParseInt(idStr, 10, 64)
	return id, nil
}
