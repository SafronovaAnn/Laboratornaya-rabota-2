package main

import (
	"fmt"
	"strconv"
	"strings"
)

func FindSum(numsStr string, target int) {
	strs := strings.Split(numsStr, ",")
	var nums []int
	for _, s := range strs {
		v, _ := strconv.Atoi(s)
		nums = append(nums, v)
	}

	found := false
	for start := 0; start < len(nums); start++ {
		sum := 0
		for end := start; end < len(nums); end++ {
			sum += nums[end]
			if sum == target {
				found = true
				fmt.Printf("%v\n", nums[start:end+1])
			}
		}
	}
	if !found {
		fmt.Printf("Подмассивов с суммой %d не найдено.\n", target)
	}
}