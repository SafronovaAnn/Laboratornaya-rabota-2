package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

type RehashTable struct {
	Table    []DList
	Capacity int
	Size     int
}

var GlobalRehash = RehashTable{Capacity: 10, Table: make([]DList, 10)}

func (r *RehashTable) hashFunc(key string, cap int) int {
	var h uint = 0
	for _, ch := range key { h = h*31 + uint(ch) }
	return int(h % uint(cap))
}

func (r *RehashTable) Resize() {
	newCap := r.Capacity * 2
	fmt.Printf("Реструктуризация: %d -> %d\n", r.Capacity, newCap)

	newTable := make([]DList, newCap)
	for i := 0; i < newCap; i++ { /* Инициализация пустых списков */ }

	for i := 0; i < r.Capacity; i++ {
		cur := r.Table[i].Head
		for cur != nil {
			key := strings.SplitN(cur.Data, "\x01", 2)[0]
			idx := r.hashFunc(key, newCap)
			newTable[idx].PushBack(cur.Data)
			cur = cur.Next
		}
	}
	r.Table = newTable
	r.Capacity = newCap
}

func (r *RehashTable) Insert(key, value string) {
	idx := r.hashFunc(key, r.Capacity)
	record := key + "\x01" + value

	cur := r.Table[idx].Head
	for cur != nil {
		if strings.HasPrefix(cur.Data, key+"\x01") { return }
		cur = cur.Next
	}
	r.Table[idx].PushBack(record)
	r.Size++

	if float64(r.Size)/float64(r.Capacity) >= 0.9 {
		r.Resize()
	}
}

func (r *RehashTable) InsertNoResize(key, value string) {
	idx := r.hashFunc(key, r.Capacity)
	record := key + "\x01" + value

	cur := r.Table[idx].Head
	for cur != nil {
		if strings.HasPrefix(cur.Data, key+"\x01") { return }
		cur = cur.Next
	}
	r.Table[idx].PushBack(record)
	r.Size++
}

func (r *RehashTable) Find(key string) string {
	idx := r.hashFunc(key, r.Capacity)
	cur := r.Table[idx].Head
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

func (r *RehashTable) Remove(key string) {
	idx := r.hashFunc(key, r.Capacity)
	cur := r.Table[idx].Head
	for cur != nil {
		parts := strings.SplitN(cur.Data, "\x01", 2)
		if parts[0] == key {
			r.Table[idx].DelByValue(cur.Data)
			r.Size--
			return
		}
		cur = cur.Next
	}
}

func (r *RehashTable) Print() {
	fmt.Printf("Хеш-таблица (size: %d, cap: %d):\n", r.Size, r.Capacity)
	for i := 0; i < r.Capacity; i++ {
		if r.Table[i].Head != nil {
			fmt.Printf("[%d]: ", i)
			cur := r.Table[i].Head
			for cur != nil {
				fmt.Print(strings.Replace(cur.Data, "\x01", ":", 1), " ")
				cur = cur.Next
			}
			fmt.Println()
		}
	}
}

func (r *RehashTable) SetCapacity(newCap int) {
	r.Capacity = newCap
	r.Size = 0
	r.Table = make([]DList, newCap)
}


// СОХРАНЕНИЕ / ЗАГРУЗКА
func SaveRehash(filename string) {
	data, _ := os.ReadFile(filename)
	lines := strings.Split(string(data), "\n")
	var keep []string
	for _, l := range lines {
		if !strings.HasPrefix(l, "REHASH") && l != "" {
			keep = append(keep, l)
		}
	}

	f, _ := os.Create(filename)
	defer f.Close()
	for _, l := range keep { f.WriteString(l + "\n") }

	f.WriteString(fmt.Sprintf("REHASH %d", GlobalRehash.Capacity))
	for i := 0; i < GlobalRehash.Capacity; i++ {
		cur := GlobalRehash.Table[i].Head
		for cur != nil {
			f.WriteString(" " + strings.Replace(cur.Data, "\x01", " ", 1))
			cur = cur.Next
		}
	}
	f.WriteString("\n")
}

func LoadRehash(filename string) {
	data, err := os.ReadFile(filename)
	if err != nil { return }

	lines := strings.Split(string(data), "\n")
	for _, line := range lines {
		parts := strings.Fields(line)
		if len(parts) < 2 { continue }

		if parts[0] == "REHASH" {
			cap, _ := strconv.Atoi(parts[1])
			GlobalRehash.SetCapacity(cap)

			for i := 2; i < len(parts); i += 2 {
				if i+1 < len(parts) {
					GlobalRehash.InsertNoResize(parts[i], parts[i+1])
				}
			}
		}
	}
}