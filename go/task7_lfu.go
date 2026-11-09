package main

import (
	"fmt"
	"sort"
)

type LFUNode struct {
	Key, Value, Freq int
	Time             int
	IsChain          bool
}

type LFUCache struct {
	Cap   int
	Cache map[int]*LFUNode
	Clock int
}

var GlobalLFU = &LFUCache{Cap: 3, Cache: make(map[int]*LFUNode), Clock: 0}

func (c *LFUCache) Get(key int) int {
	if n, ok := c.Cache[key]; ok {
		n.Freq++
		c.Clock++
		n.Time = c.Clock
		return n.Value
	}
	return -1
}

func (c *LFUCache) Put(key, value int, isChain bool) {
	// Если элемент уже есть, обновляем его
	if n, ok := c.Cache[key]; ok {
		n.Value = value
		n.Freq++
		n.IsChain = isChain
		c.Clock++
		n.Time = c.Clock
		return
	}

	// Если переполнен, удаляем элемент с минимальной частотой (и самым старым временем)
	if len(c.Cache) >= c.Cap {
		minF, minT, minK := 999999, 999999, -1
		for k, n := range c.Cache {
			if n.Freq < minF || (n.Freq == minF && n.Time < minT) {
				minF, minT, minK = n.Freq, n.Time, k
			}
		}
		if minK != -1 {
			delete(c.Cache, minK)
		}
	}

	c.Clock++
	c.Cache[key] = &LFUNode{Key: key, Value: value, Freq: 1, Time: c.Clock, IsChain: isChain}
}

func (c *LFUCache) PrintCache() {
	if len(c.Cache) == 0 {
		fmt.Println("Cache: (пусто)")
		return
	}

	var keys []int
	for k := range c.Cache {
		keys = append(keys, k)
	}

	// Сортируем по убыванию Time (самые новые в начале)
	sort.Slice(keys, func(i, j int) bool {
		return c.Cache[keys[i]].Time > c.Cache[keys[j]].Time
	})

	fmt.Print("Cache: ")
	for _, k := range keys {
		node := c.Cache[k]
		mark := "C"
		if !node.IsChain {
			mark = "K"
		}
		fmt.Printf("[%d:%d freq=%d %s] ", node.Key, node.Value, node.Freq, mark)
	}
	fmt.Println()
}