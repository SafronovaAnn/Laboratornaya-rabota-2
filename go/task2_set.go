package main

import (
	"fmt"
	"strings"
)

type MySet struct {
	Elements []string
}

var globalSets = make(map[string]*MySet)

func (m *MySet) Add(v string) {
	for _, el := range m.Elements {
		if el == v {
			return
		}
	}
	m.Elements = append(m.Elements, v)
}

func (m *MySet) Contains(v string) bool {
	for _, el := range m.Elements {
		if el == v {
			return true
		}
	}
	return false
}

func (m *MySet) Remove(v string) {
	for i, el := range m.Elements {
		if el == v {
			m.Elements = append(m.Elements[:i], m.Elements[i+1:]...)
			return
		}
	}
}

func (m *MySet) Print() {
	fmt.Printf("{%s}\n", strings.Join(m.Elements, ", "))
}