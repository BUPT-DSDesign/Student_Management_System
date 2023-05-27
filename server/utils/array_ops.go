package utils

func RemoveRepeatedElement(nums []int) []int {
	if len(nums) <= 1 {
		return nums
	}
	j := 0
	for i := 1; i < len(nums); i++ {
		if nums[i] != nums[j] {
			j++
			nums[j] = nums[i]
		}
	}
	return nums[:j+1]
}
