package utils

// BoolToInt8 将bool类型转换为int8
func BoolToInt8(b bool) int8 {
	if b {
		return int8(1)
	}
	return int8(0)
}
