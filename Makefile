# Runs rules for given commands.
# Copyright (C) 2017  Elliott Sobek
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c11 -O3 -D_POSIX_C_SOURCE=200809L

.PHONY: all clean

all: SinglyLinkedList DoublyLinkedList BinarySearchTree HashTable
####################################
SinglyLinkedList: SinglyLinkedList.o

SinglyLinkedList.o: SinglyLinkedList.c
####################################
DoublyLinkedList: DoublyLinkedList.o

DoublyLinkedList.o: DoublyLinkedList.c
####################################
BinarySearchTree: BinarySearchTree.o

BinarySearchTree.o: BinarySearchTree.c
####################################
HashTable: HashTable.o

HashTable.o: HashTable.c
####################################
clean:
	$(RM) *.o
