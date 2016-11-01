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

#include "data.h"
typedef struct node {
	data d;
	struct node *next;
}node;
typedef struct queue {   
	node *head, *tail;
	int len;
}queue;

void qinit(queue *q);
void enqueue(queue *q, data d);
data dequeue(queue *q);
int queuelen(queue q);
int qempty(queue *q);
int qfull(queue *q);