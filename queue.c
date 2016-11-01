/*****************************************************************************
 * Copyright (C) Shreyansh Gopawar gopawarsr15.it@coep.ac.in
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
void qinit(queue *q){
	q->head = q->tail = NULL;
	q->len = 0;
}
void enqueue(queue *q, data d){
	node *tmp;
	tmp = (node *) malloc(sizeof(node));
	tmp->d = d;
	tmp->next = NULL;
	if(q->tail) 
		q->tail->next = tmp;
	else
		q->head = tmp;
	q->tail = tmp;
	q->len++;
}
data dequeue(queue *q){
	data d;
	node *tmp;
	d = q->head->d;
	tmp = q->head->next;
	free(q->head);
	q->head = tmp;
	if(q->head == NULL)
		q->tail = NULL;
	q->len--;
	return d;
}
int qempty(queue *q){
	return q->head == NULL;
}
int qfull(queue *q){
	return 0;
}
int queuelen(queue q) {
	return q.len;
}
