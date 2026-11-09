package main

import (
	"fmt"
	"os"
	"strings"
)

type DNode struct {
	Data string
	Prev *DNode
	Next *DNode
}

type DList struct {
	Head *DNode
	Tail *DNode
}

func (d *DList) PushBack(val string) {
	n := &DNode{Data: val, Prev: d.Tail}
	if d.Head == nil {
		d.Head = n
		d.Tail = n
		return
	}
	d.Tail.Next = n
	d.Tail = n
}

func (d *DList) DelByValue(val string) {
	cur := d.Head
	for cur != nil {
		if cur.Data == val {
			if cur.Prev != nil { cur.Prev.Next = cur.Next } else { d.Head = cur.Next }
			if cur.Next != nil { cur.Next.Prev = cur.Prev } else { d.Tail = cur.Prev }
			return
		}
		cur = cur.Next
	}
}

// 6.1 МЕТОД ЦЕПОЧЕК
const TableSize = 10 

type ChainingHashTable struct {
	Buckets [TableSize]DList
}

var GlobalChain ChainingHashTable

func (c *ChainingHashTable) hashFunc(key string) int {
	var h uint = 0
	for _, ch := range key { h = h*31 + uint(ch) }
	return int(h % TableSize)
}

func (c *ChainingHashTable) Insert(key, value string) {
	idx := c.hashFunc(key)
	record := key + "\x01" + value
	
	// Проверка на дубликат
	cur := c.Buckets[idx].Head
	for cur != nil {
		if strings.HasPrefix(cur.Data, key+"\x01") { return }
		cur = cur.Next
	}
	c.Buckets[idx].PushBack(record)
}

func (c *ChainingHashTable) Find(key string) string {
	idx := c.hashFunc(key)
	cur := c.Buckets[idx].Head
	for cur != nil {
		parts := strings.SplitN(cur.Data, "\x01", 2)
		if parts[0] == key {
			if len(parts) > 1 { return parts[1] }
			return ""
		}
		cur = cur.Next
	}
	return ""
}

func (c *ChainingHashTable) Remove(key string) {
	idx := c.hashFunc(key)
	cur := c.Buckets[idx].Head
	for cur != nil {
		parts := strings.SplitN(cur.Data, "\x01", 2)
		if parts[0] == key {
			c.Buckets[idx].DelByValue(cur.Data)
			return
		}
		cur = cur.Next
	}
}

func (c *ChainingHashTable) Print() {
	fmt.Println("--- Chaining Hash Table ---")
	for i := 0; i < TableSize; i++ {
		if c.Buckets[i].Head != nil {
			fmt.Printf("[%d]: ", i)
			cur := c.Buckets[i].Head
			for cur != nil {
				fmt.Print(strings.Replace(cur.Data, "\x01", ":", 1), " ")
				cur = cur.Next
			}
			fmt.Println()
		}
	}
}

// 6.1 ХЕШИРОВАНИЕ КУКУШКИ
type CuckooHashTable struct {
	Table1 [TableSize]string
	Table2 [TableSize]string
}

var GlobalCuckoo CuckooHashTable

func (c *CuckooHashTable) hash1(key string) int {
	var h uint = 0
	for _, ch := range key { h = h*31 + uint(ch) }
	return int(h % TableSize)
}

func (c *CuckooHashTable) hash2(key string) int {
	var h uint = 0
	for _, ch := range key { h = h*53 + uint(ch) }
	return int(h % TableSize)
}

func (c *CuckooHashTable) Insert(key, value string) bool {
	if c.Find(key) != "" { return true }
	
	current := key + "\x01" + value
	currentKey := key
	maxLoops := 50

	for i := 0; i < maxLoops; i++ {
		pos1 := c.hash1(currentKey)
		if c.Table1[pos1] == "" {
			c.Table1[pos1] = current
			return true
		}
		evicted := c.Table1[pos1]
		c.Table1[pos1] = current
		current = evicted
		currentKey = strings.SplitN(current, "\x01", 2)[0]

		pos2 := c.hash2(currentKey)
		if c.Table2[pos2] == "" {
			c.Table2[pos2] = current
			return true
		}
		evicted = c.Table2[pos2]
		c.Table2[pos2] = current
		current = evicted
		currentKey = strings.SplitN(current, "\x01", 2)[0]
	}
	fmt.Println("Ошибка: зацикливание при вставке", key)
	return false
}

func (c *CuckooHashTable) Find(key string) string {
	pos1 := c.hash1(key)
	if c.Table1[pos1] != "" {
		parts := strings.SplitN(c.Table1[pos1], "\x01", 2)
		if parts[0] == key { return parts[1] }
	}
	pos2 := c.hash2(key)
	if c.Table2[pos2] != "" {
		parts := strings.SplitN(c.Table2[pos2], "\x01", 2)
		if parts[0] == key { return parts[1] }
	}
	return ""
}

func (c *CuckooHashTable) Remove(key string) {
	pos1 := c.hash1(key)
	if c.Table1[pos1] != "" && strings.HasPrefix(c.Table1[pos1], key+"\x01") {
		c.Table1[pos1] = ""
		return
	}
	pos2 := c.hash2(key)
	if c.Table2[pos2] != "" && strings.HasPrefix(c.Table2[pos2], key+"\x01") {
		c.Table2[pos2] = ""
		return
	}
}

func (c *CuckooHashTable) Print() {
	fmt.Println("--- Cuckoo Hash Table 1 ---")
	for i, val := range c.Table1 {
		if val != "" { fmt.Printf("[%d]: %s\n", i, strings.Replace(val, "\x01", ":", 1)) }
	}
	fmt.Println("--- Cuckoo Hash Table 2 ---")
	for i, val := range c.Table2 {
		if val != "" { fmt.Printf("[%d]: %s\n", i, strings.Replace(val, "\x01", ":", 1)) }
	}
}

// СОХРАНЕНИЕ / ЗАГРУЗКА (в общий файл)
func SaveHashes(filename string) {
	// Читаем старый файл, чтобы не потерять SET, REHASH и LFU
	data, _ := os.ReadFile(filename)
	lines := strings.Split(string(data), "\n")
	var keep []string
	for _, l := range lines {
		if !strings.HasPrefix(l, "CHAIN") && !strings.HasPrefix(l, "CUCKOO") && l != "" {
			keep = append(keep, l)
		}
	}

	f, _ := os.Create(filename)
	defer f.Close()
	for _, l := range keep { f.WriteString(l + "\n") }

	// CHAIN
	f.WriteString("CHAIN")
	for i := 0; i < TableSize; i++ {
		cur := GlobalChain.Buckets[i].Head
		for cur != nil {
			f.WriteString(" " + strings.Replace(cur.Data, "\x01", " ", 1))
			cur = cur.Next
		}
	}
	f.WriteString("\n")

	// CUCKOO
	f.WriteString("CUCKOO")
	for _, val := range GlobalCuckoo.Table1 {
		if val != "" { f.WriteString(" " + strings.Replace(val, "\x01", " ", 1)) }
	}
	for _, val := range GlobalCuckoo.Table2 {
		if val != "" { f.WriteString(" " + strings.Replace(val, "\x01", " ", 1)) }
	}
	f.WriteString("\n")
}

func LoadHashes(filename string) {
	data, err := os.ReadFile(filename)
	if err != nil { return }
	
	lines := strings.Split(string(data), "\n")
	for _, line := range lines {
		parts := strings.Fields(line)
		if len(parts) == 0 { continue }
		
		if parts[0] == "CHAIN" {
			for i := 1; i < len(parts); i += 2 {
				if i+1 < len(parts) { GlobalChain.Insert(parts[i], parts[i+1]) }
			}
		} else if parts[0] == "CUCKOO" {
			for i := 1; i < len(parts); i += 2 {
				if i+1 < len(parts) { GlobalCuckoo.Insert(parts[i], parts[i+1]) }
			}
		}
	}
}