package main

import (
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

func main() {
	args := os.Args[1:]
	var query string
	var filename = "data.txt"

	for i := 0; i < len(args); i++ {
		if args[i] == "--query" && i+1 < len(args) {
			query = args[i+1]
		} else if args[i] == "--file" && i+1 < len(args) {
			filename = args[i+1]
		}
	}

	if query == "" {
		fmt.Println("Ошибка: неизвестная команда")
		return
	}

	tokens := strings.Fields(query)
	if len(tokens) == 0 {
		fmt.Println("Ошибка: неизвестная команда")
		return
	}
	cmd := tokens[0]

	loadSets("sets.txt")
	LoadHashes(filename)
	LoadRehash(filename)
	loadLFU(filename)

	if cmd == "FIXBRACKETS" && len(tokens) >= 2 {
		inputStr := strings.Join(tokens[1:], " ")
		fmt.Println(RemovePar(inputStr))

	} else if cmd == "SETADD" && len(tokens) == 3 {
		if globalSets[tokens[1]] == nil {
			globalSets[tokens[1]] = &MySet{}
		}
		globalSets[tokens[1]].Add(tokens[2])
		saveSets("sets.txt")
	} else if cmd == "SET_AT" && len(tokens) == 3 {
		if globalSets[tokens[1]] != nil && globalSets[tokens[1]].Contains(tokens[2]) {
			fmt.Println("TRUE")
		} else {
			fmt.Println("FALSE")
		}
	} else if cmd == "SETDEL" && len(tokens) == 3 {
		if globalSets[tokens[1]] != nil {
			globalSets[tokens[1]].Remove(tokens[2])
			saveSets("sets.txt")
		}
	} else if cmd == "SETPRINT" && len(tokens) == 2 {
		if globalSets[tokens[1]] != nil {
			globalSets[tokens[1]].Print()
		} else {
			fmt.Println("Ошибка: множество не найдено")
		}

	} else if cmd == "PARTITION" && len(tokens) >= 2 {
		numsStr := strings.Join(tokens[1:], "")
		PartitionSubsets(numsStr)

	} else if cmd == "FINDSUM" && len(tokens) >= 2 {
		numsStr := strings.Join(tokens[1:len(tokens)-1], "")
		target, _ := strconv.Atoi(tokens[len(tokens)-1])
		FindSum(numsStr, target)

	} else if cmd == "HUFFMAN" && len(tokens) >= 2 {
		RunHuffman(strings.Join(tokens[1:], " "))

	} else if cmd == "CHAINADD" && len(tokens) == 3 {
		GlobalChain.Insert(tokens[1], tokens[2])
		SaveHashes(filename)
	} else if cmd == "CHAINFIND" && len(tokens) == 2 {
		res := GlobalChain.Find(tokens[1])
		if res != "" {
			fmt.Println("Значение:", res)
		} else {
			fmt.Println("NOT FOUND")
		}
	} else if cmd == "CHAINDEL" && len(tokens) == 2 {
		GlobalChain.Remove(tokens[1])
		SaveHashes(filename)
	} else if cmd == "CHAINPRINT" {
		GlobalChain.Print()

	} else if cmd == "CUCKOOADD" && len(tokens) == 3 {
		GlobalCuckoo.Insert(tokens[1], tokens[2])
		SaveHashes(filename)
	} else if cmd == "CUCKOOFIND" && len(tokens) == 2 {
		res := GlobalCuckoo.Find(tokens[1])
		if res != "" {
			fmt.Println("Значение:", res)
		} else {
			fmt.Println("NOT FOUND")
		}
	} else if cmd == "CUCKOODEL" && len(tokens) == 2 {
		GlobalCuckoo.Remove(tokens[1])
		SaveHashes(filename)
	} else if cmd == "CUCKOOPRINT" {
		GlobalCuckoo.Print()

	} else if cmd == "REHASHADD" && len(tokens) == 3 {
		GlobalRehash.Insert(tokens[1], tokens[2])
		SaveRehash(filename)
	} else if cmd == "REHASHFIND" && len(tokens) == 2 {
		res := GlobalRehash.Find(tokens[1])
		if res != "" {
			fmt.Println("Значение:", res)
		} else {
			fmt.Println("NOT FOUND")
		}
	} else if cmd == "REHASHDEL" && len(tokens) == 2 {
		GlobalRehash.Remove(tokens[1])
		SaveRehash(filename)
	} else if cmd == "REHASHPRINT" {
		GlobalRehash.Print()

	} else if cmd == "LFUSETCAP" && len(tokens) == 2 {
		cap, _ := strconv.Atoi(tokens[1])
		GlobalLFU.Cap = cap
		saveLFU(filename)
	} else if cmd == "LFUCHAIN" && len(tokens) == 3 {
		k, _ := strconv.Atoi(tokens[1])
		v, _ := strconv.Atoi(tokens[2])
		GlobalLFU.Put(k, v, true)
		saveLFU(filename)
	} else if cmd == "LFUCUCKOO" && len(tokens) == 3 {
		k, _ := strconv.Atoi(tokens[1])
		v, _ := strconv.Atoi(tokens[2])
		GlobalLFU.Put(k, v, false)
		saveLFU(filename)
	} else if cmd == "LFUGET" && len(tokens) == 2 {
		k, _ := strconv.Atoi(tokens[1])
		fmt.Println(GlobalLFU.Get(k))
		saveLFU(filename)
	} else if cmd == "LFUPRINT" {
		GlobalLFU.PrintCache()

	} else {
		fmt.Println("Ошибка: неизвестная команда")
	}
}

func saveSets(filename string) {
	f, _ := os.Create(filename)
	defer f.Close()
	for name, set := range globalSets {
		f.WriteString(name + " ")
		for _, el := range set.Elements {
			f.WriteString(el + " ")
		}
		f.WriteString("\n")
	}
}

func loadSets(filename string) {
	data, err := os.ReadFile(filename)
	if err != nil {
		return
	}
	lines := strings.Split(string(data), "\n")
	for _, line := range lines {
		parts := strings.Fields(line)
		if len(parts) > 1 {
			name := parts[0]
			globalSets[name] = &MySet{}
			for _, el := range parts[1:] {
				globalSets[name].Add(el)
			}
		}
	}
}

func saveLFU(filename string) {
	data, _ := os.ReadFile(filename)
	lines := strings.Split(string(data), "\n")
	var keep []string
	for _, l := range lines {
		if !strings.HasPrefix(l, "LFU") && l != "" {
			keep = append(keep, l)
		}
	}

	f, _ := os.Create(filename)
	defer f.Close()
	for _, l := range keep {
		f.WriteString(l + "\n")
	}

	f.WriteString("LFU " + strconv.Itoa(GlobalLFU.Cap))

	// Сортируем по возрастанию Time 
	var keys []int
	for k := range GlobalLFU.Cache {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool {
		return GlobalLFU.Cache[keys[i]].Time < GlobalLFU.Cache[keys[j]].Time
	})

	for _, k := range keys {
		n := GlobalLFU.Cache[k]
		mark := "C"
		if !n.IsChain {
			mark = "K"
		}
		f.WriteString(fmt.Sprintf(" %d:%d#%d#%s", k, n.Value, n.Freq, mark))
	}
	f.WriteString("\n")
}

func loadLFU(filename string) {
	data, err := os.ReadFile(filename)
	if err != nil {
		return
	}
	GlobalLFU.Cache = make(map[int]*LFUNode)
	GlobalLFU.Clock = 0 // Сбрасываем часы
	lines := strings.Split(string(data), "\n")
	for _, line := range lines {
		if strings.HasPrefix(line, "LFU ") {
			parts := strings.Fields(line)
			if len(parts) > 1 {
				cap, _ := strconv.Atoi(parts[1])
				GlobalLFU.Cap = cap
			}
			// Читаем в прямом порядке: от старых к новым
			for i := 2; i < len(parts); i++ {
				item := parts[i]
				p := strings.Split(item, "#")
				if len(p) == 3 {
					kv := strings.Split(p[0], ":")
					if len(kv) == 2 {
						freq, _ := strconv.Atoi(p[1])
						k, _ := strconv.Atoi(kv[0])
						v, _ := strconv.Atoi(kv[1])
						isChain := p[2] == "C"
						GlobalLFU.Clock++
						GlobalLFU.Cache[k] = &LFUNode{Key: k, Value: v, Freq: freq, IsChain: isChain, Time: GlobalLFU.Clock}
					}
				}
			}
			break
		}
	}
}