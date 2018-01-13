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
