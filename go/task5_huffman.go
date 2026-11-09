package main

import (
	"fmt"
	"sort"
	"strings"
)

type HNode struct {
	Ch    rune
	Freq  int
	Left  *HNode
	Right *HNode
}

func (n *HNode) IsLeaf() bool { return n.Left == nil && n.Right == nil }

func GenerateCodes(node *HNode, code string, codes map[rune]string) {
	if node == nil { return }
	if node.IsLeaf() {
		if code == "" { code = "0" } // Для одного символа
		codes[node.Ch] = code
	}
	GenerateCodes(node.Left, code+"0", codes)
	GenerateCodes(node.Right, code+"1", codes)
}

func DecodeHuffman(root *HNode, encoded string) string {
	if root.IsLeaf() {
		return strings.Repeat(string(root.Ch), len(encoded))
	}
	var result strings.Builder
	curr := root
	for _, bit := range encoded {
		if bit == '0' { curr = curr.Left } else { curr = curr.Right }
		if curr.IsLeaf() {
			result.WriteRune(curr.Ch)
			curr = root
		}
	}
	return result.String()
}

func RunHuffman(text string) {
	if text == "" { return }
	freq := make(map[rune]int)
	for _, ch := range text { freq[ch]++ }

	var nodes []*HNode
	for ch, f := range freq { nodes = append(nodes, &HNode{Ch: ch, Freq: f}) }

	// Строим дерево
	for len(nodes) > 1 {
		// Сортируем по частоте
		sort.Slice(nodes, func(i, j int) bool { return nodes[i].Freq < nodes[j].Freq })
		left := nodes[0]
		right := nodes[1]
		parent := &HNode{Freq: left.Freq + right.Freq, Left: left, Right: right}
		nodes = append(nodes[2:], parent)
	}
	root := nodes[0]

	codes := make(map[rune]string)
	GenerateCodes(root, "", codes)

	// Вывод таблицы 
	var keys []rune
	for k := range codes { keys = append(keys, k) }
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })

	for _, k := range keys {
		sym := string(k)
		if k == ' ' { sym = "_" } 
		fmt.Printf("%s : %s\n", sym, codes[k])
	}

	var encoded strings.Builder
	for _, ch := range text { encoded.WriteString(codes[ch]) }
	fmt.Printf("\nЗакодированная строка:\n%s\n", encoded.String())

	decoded := DecodeHuffman(root, encoded.String())
	fmt.Printf("\nРаскодированная строка:\n%s\n", decoded)
}