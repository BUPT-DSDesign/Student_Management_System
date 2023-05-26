package check_conflict

import (
	"fmt"
	"testing"
)

func Test(t *testing.T) {
	strs := []string{"8:00 ~ 9:00", "10:00 ~11 :00"}
	sprintf := fmt.Sprintf("该活动与课程时间冲突, 以下是合适的时间段:\n %v(当天)", strs)
	fmt.Println(sprintf)
}
