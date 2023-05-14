package utils

import (
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

	return node.Generate().Int64(), nil
}
