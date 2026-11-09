package main

import (
	"strconv"
	"strings"
)

type SNode struct {
	Data string
	Next *SNode
}
type Stack struct{ Head *SNode }

func InitStack() *Stack { return &Stack{Head: nil} }
func (s *Stack) Push(v string) { s.Head = &SNode{Data: v, Next: s.Head} }
func (s *Stack) Pop() *SNode {
	if s.Head == nil {
		return nil
	}
	old := s.Head
	s.Head = s.Head.Next
	return old
}
func (s *Stack) IsEmpty() bool { return s.Head == nil }

func RemovePar(s string) string {
	st := InitStack()
	keep := make([]bool, len(s))
	for i := range keep {
		keep[i] = true
	}

	for i := 0; i < len(s); i++ {
		if s[i] == '(' {
			st.Push(strconv.Itoa(i))
		} else if s[i] == ')' {
			if !st.IsEmpty() {
				st.Pop()
			} else {
				keep[i] = false
			}
		}
	}

	for !st.IsEmpty() {
		node := st.Pop()
		idx, _ := strconv.Atoi(node.Data)
		keep[idx] = false
	}

	var res strings.Builder
	for i := 0; i < len(s); i++ {
		if keep[i] {
			res.WriteByte(s[i])
		}
	}
	return res.String()
}