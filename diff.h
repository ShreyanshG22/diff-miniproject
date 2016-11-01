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

#define NEITHER 0 
#define DOWN 1 
#define RIGHT 2 
#define DOWN_AND_RIGHT 3 

int regular_file1_flag = 0;
int regular_file2_flag = 0;
int binary_file1_flag = 0;
int binary_file2_flag = 0;
int directory1_flag = 0;
int directory2_flag = 0;
int ignore_space_change_flag = 0;
int ignore_blank_lines_flag = 0;
int ignore_case_flag = 0;
int no_details_flag = 0;
int ignore_all_space_flag = 0;
int ignore_trailing_leading_space_blank_lines_flag = 0;
int ignore_all_tabs_flag = 0;

int printUsage();
int displayError();
int comparefiles(char argv1[], char argv2[], int option);
int comparebinaryfiles(char argv1[], char argv2[]);
int comparedirectory(char argv1[], char argv2[], int option);
int numofLines(char File[]);
int dirnum(char *argv);
static void usage ();
void LCS(char **a,char **b);
