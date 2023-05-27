package utils

import (
	"fmt"
	"testing"
)

func TestArray(t *testing.T) {
	nums := []int{1, 2, 3, 4, 5, 5, 6, 7, 7, 8}
	fmt.Println(RemoveRepeatedElement(nums))
}
