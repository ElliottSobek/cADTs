SHELL := /bin/bash

CC := gcc

CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c11 -g -D_POSIX_C_SOURCE=200809L

OBJECTS := SinglyLinkedList.o DoublyLinkedList.o BinarySearchTree.o HashTable.o

.PHONY: all clean

all: SinglyLinkedList DoublyLinkedList BinarySearchTree HashTable

SinglyLinkedList: SinglyLinkedList.o

DoublyLinkedList: DoublyLinkedList.o

BinarySearchTree: BinarySearchTree.o

HashTable: HashTable.o

$(OBJECTS):

clean:
	$(RM) *.o
