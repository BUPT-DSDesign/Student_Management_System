package utils

import "testing"

func Test(t *testing.T) {
	// 测试雪花算法生成id
	id, err := GenerateId()
	if err != nil {
		t.Error(err)
	}
	t.Log(id)

}
