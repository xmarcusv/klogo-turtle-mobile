#!/bin/bash

COMANDO=$1
PASSOS=$2

function fd(){
        outbusb 2
	sleep 10
	outbusb 0
}

function bk(){
	outbusb 4
	sleep 10
	outbusb 0
}

function lt(){
	outbusb 8
	sleep 10
	outbusb 0
}

function rt(){
	outbusb 16
	sleep 10
	outbusb 0
}

function sx(){
	outbusb 64
	sleep 10
	outbusb 0
}

function sy(){
	outbusb 128
	sleep 10
	outbusb 0
}


case $COMANDO in
	"C0") fd;;
	"C1") bk;;
	"C2") lt;;
	"C3") rt;;
	"C4") sx;;
	"C5") sy;;
esac
