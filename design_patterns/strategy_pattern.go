package main

import "fmt"

type Character struct {
   weapon WeaponBehvior
}

func (character *Character)SetWeapon(w WeaponBehvior) {
   character.weapon = w
}

func (character *Character)Fight() {
   fmt.Println("character fight")
}

type Queen struct {
   Character
}

func (queen *Queen)Fight() {
   fmt.Println("queen fight")
}

type King struct {
   Character
}

func (king *King)Fight() {
   fmt.Println("king fight")
}

type WeaponBehvior interface {
   useWeapon()
}

type SwordBehavior struct{
}

func (SwordBehavior *SwordBehavior)useWeapon() {
   fmt.Println("用宝剑挥舞")
}

type KnifeBehavior struct{
}

func (knifeBehavior *KnifeBehavior)useWeapon() {
   fmt.Println("用匕首挥舞")
}

func main() {
   a := &Character{"aaaaa"}
   a.Print()
}