package utils

import (
	"testing"
)

func TestArray(t *testing.T) {
	a := "第1周-星期1-08:00"

	b := "第1周-星期2-09:00"
	println(GetDay(b))
	c := "第1周-星期1-09:00"
	println(GetDay(c))
	println(IsBetween(a, b, c))
}
