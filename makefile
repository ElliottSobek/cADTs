CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c11 -O3

.PHONY: all clean

all: SinglyLinkedList DoublyLinkedList BinarySearchTree HashTable
####################################
SinglyLinkedList: SinglyLinkedList.o
	$(CC) $(CFLAGS) $< -o $@

SinglyLinkedList.o: SinglyLinkedList.c
####################################
DoublyLinkedList: DoublyLinkedList.o
	$(CC) $(CFLAGS) $< -o $@

DoublyLinkedList.o: DoublyLinkedList.c
####################################
BinarySearchTree: BinarySearchTree.o
	$(CC) $(CFLAGS) $< -o $@

BinarySearchTree.o: BinarySearchTree.c
####################################
HashTable: HashTable.o
	$(CC) $(CFLAGS) $< -o $@

HashTable.o: HashTable.c
####################################
clean:
	rm *.o
