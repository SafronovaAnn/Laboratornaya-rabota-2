package main

import (
	"fmt"
	"sort"
	"strconv"
	"strings"
)

func PartitionSubsets(numsStr string) {
	// Проверяем на минус до всего остального
	if strings.Contains(numsStr, "-") {
		fmt.Println("Ошибка: все числа должны быть натуральными (положительными)")
		return
	}

	strs := strings.Split(numsStr, ",")
	var nums []int
	for _, s := range strs {
		v, err := strconv.Atoi(strings.TrimSpace(s))
		if err == nil {
			nums = append(nums, v)
		}
	}

	set := &MySet{}
	for _, v := range nums {
		set.Add(strconv.Itoa(v))
	}

	var uniqueNums []int
	total := 0
	for _, s := range set.Elements {
		v, _ := strconv.Atoi(s)
		uniqueNums = append(uniqueNums, v)
		total += v
	}

	if len(uniqueNums) == 0 {
		fmt.Println("Ошибка: пустой ввод")
		return
	}

	target := total / 2
	if target < 0 {
		target = 0
	}

	dp := make([]bool, target+1)
	dp[0] = true

	for _, x := range uniqueNums {
		for i := target; i >= x; i-- {
			if dp[i-x] {
				dp[i] = true
			}
		}
	}

	sum1 := 0
	for i := target; i >= 0; i-- {
		if dp[i] {
			sum1 = i
			break
		}
	}
	diff := (total - sum1) - sum1

	var g1, g2 []int
	curr := sum1
	for i := len(uniqueNums) - 1; i >= 0; i-- {
		if curr >= uniqueNums[i] && dp[curr-uniqueNums[i]] {
			g1 = append(g1, uniqueNums[i])
			curr -= uniqueNums[i]
		} else {
			g2 = append(g2, uniqueNums[i])
		}
	}

	sort.Ints(g1)
	sort.Ints(g2)
	fmt.Printf("Подмножество 1: %v\n", g1)
	fmt.Printf("Подмножество 2: %v\n", g2)
	fmt.Printf("Разница между суммами: %d\n", diff)
}