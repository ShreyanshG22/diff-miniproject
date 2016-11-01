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
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "diff.h"
#include "queue.h"
#include "data.h"

int str1, str2;
int filn1, filn2;

int main(int argc, char *argv[]) {
	char *Option[10], *File[2];
	int i = 1, option = 0, k = 0;
	int c;
	int status1, status2;
	struct stat st_buf1, st_buf2;
	while(argv[i] != NULL) {
		if(*argv[i] == '-') {
			Option[option] = argv[i];	
			option++;
		}
		else {
			if(k < 2)			
				File[k] = argv[i];
			else {
				printUsage();
				return -1;
			}
			k++;
		}
		i++;
	}
	if(option == 1 && argc == 2) {
		if(Option[0][1] == 'h') {
			usage();
			return 0;
		}
		else {
			printUsage();
			return 0;
		}
	}	
	else if((argc - option) != 3) {
		printUsage();
		return -1;
	}
	status1 = stat (File[0], &st_buf1);
	if (status1 != 0) {
		perror ("Error ");
		return errno;
	}
	if (S_ISREG (st_buf1.st_mode)) {
		int fd,flag=0;
		char ch;
		fd = open(File[0], O_RDONLY);
			if(fd == -1) 
				{
					perror("Error in open:");
					return (errno);
				}
			while(read(fd,&ch,1))
				{
					if(!(isprint(ch) || isspace(ch)))
						flag = 1;	
				}
			if(flag==1)
				binary_file1_flag = 1;
			else
				regular_file1_flag = 1;
		close(fd);

	}
	if (S_ISDIR (st_buf1.st_mode)) {
		directory1_flag = 1;
	}

	status2 = stat (File[1], &st_buf2);
	if (status2 != 0) {
		perror ("Error ");
		return errno;
	}
	if (S_ISREG (st_buf2.st_mode)) {
		int fd,flag=0;
		char ch;
		fd = open(File[1], O_RDONLY);
			if(fd == -1) 
				{
					perror("Error in open:");
					return (errno);
				}
			while(read(fd,&ch,1))
				{
					if(!(isprint(ch) || isspace(ch)))
						flag = 1;	
				}
			if(flag==1)
				binary_file2_flag = 1;
			else
				regular_file2_flag = 1;
		close(fd);
	}
	if (S_ISDIR (st_buf2.st_mode)) {
		directory2_flag = 1;
	}
	
	if(option != 0) {
		for(c = 0; c < option; c++) {
			if(Option[c][0] == '-') {
				switch(Option[c][1]) {
					case 'b':
					/* Ignore changes in amount of white space.  */
						ignore_space_change_flag = 1;
						break;

					case 'B':
					/* Ignore changes affecting only blank lines.  */
							ignore_blank_lines_flag = 1;
						break;

					case 'i':
					/* Ignore changes in case.  */
						ignore_case_flag = 1;
						break;

					case 'q':
					/*Show no details*/
						no_details_flag = 1;
	  					break;		

					case 't':
					/*ignore tabs*/
						ignore_all_tabs_flag = 1;
						break;

					case 'w':
					/* Ignore horizontal white space when comparing lines.  */
						ignore_all_space_flag = 1;
						break;

					case 'Z':
					/*Ignore trailing and leading spaces tabs and blank lines*/
						ignore_trailing_leading_space_blank_lines_flag = 1;
						break;

					case 'a':
					/*Treat all files as text files */
						str1 = numofLines(File[0]);
						str2 = numofLines(File[1]);	
						comparefiles(File[0], File[1], option);
						return 0;
						break;

					case 'r':
				  	/*Compares files found with similar names inside directory  */
						if((directory1_flag == 1) || (directory2_flag == 1)) {
							comparedirectory(File[0], File[1], option);
							return 0;
						}
						break;

					case 'h':
					/*Show Usage*/
						usage();
						return 0;
						break;

					default:
						printf("INVALID OPTION\n");
						exit(1);
				}
			}
		}

	}
	if(((binary_file1_flag == 1) && (binary_file2_flag == 1)) || ((regular_file1_flag == 1) && (binary_file2_flag == 1)) || ((binary_file1_flag == 1) && (regular_file2_flag == 1))) {
		str1 = numofLines(File[0]);
		str2 = numofLines(File[1]);	
		comparebinaryfiles(File[0], File[1]);
	}
	else if((regular_file1_flag == 1) && (regular_file2_flag == 1)) {
		str1 = numofLines(File[0]);
		str2 = numofLines(File[1]);	
		comparefiles(File[0], File[1], option);
	}
	else if((directory1_flag == 1) || (directory2_flag == 1))
		comparedirectory(File[0], File[1], option);
	return 0;
}
static char const * const option_help[] = {
"-i   Consider upper- and lower-case to be the same.",
"-w   Ignore all white space.",
"-b   Ignore changes in the amount of white space.",
"-B   Ignore changes whose lines are all blank.",
"-t   Ignore all tabs",
"-Z   Ignore all trailing and leading spaces, tabs and blank lines",
"Other options",
"-a   Treat all files as text files",
"-q   Output only whether files differ.",
"-r   Compares files found with similar names inside directory.",
"-h   Output this help.",
0
};

static void usage () {
	char const * const *p;
	printf ("Usage: ./project [OPTION]... FILE1 FILE2\n\n");
	for (p = option_help;  *p;  p++)
		printf ("  %s\n", *p);
}

int printUsage() {
	printf("Usage: ./project <options> <file1> <file2>\nTry (-h) option for more help\n");
	return EINVAL;
}

int displayError() {
	perror("error in open: ");
	return errno;
}

int numofLines(char File[]) {
	FILE *fp = NULL;
	int no_lines = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(File, "r");
	
    	while ((read = getline(&line, &len, fp)) != -1) {
        	no_lines=no_lines+1;
        }
	fclose(fp);
	return no_lines;
}

int comparefiles(char argv1[], char argv2[], int option) {

	FILE *fh1 = NULL;
	FILE *fh2 = NULL;
	char *line1 = NULL;
	char *line2 = NULL;
	size_t len1 = 0, len2 = 0;
	ssize_t read1, read2;
	char **a, **b;
	int size = 512;
	int i = 0, j = 0;

	if(strlen(argv1) <= 0 || strlen(argv2) <= 0) { 
		printUsage();
		return -1;
	}
	fh1 = fopen(argv1, "r");
	fh2 = fopen(argv2, "r");
	if((fh1) == NULL || (fh2) == NULL) {
		displayError();
		return -1;
	}

	a = (char **)malloc(sizeof(char *) * (str1));
	b = (char **)malloc(sizeof(char *) * (str2));

	while((read1 = getline(&line1, &len1, fh1)) != -1) { 
		a[i] = (char *)malloc(sizeof(char) * size);
		a[i] = strdup(line1);
		i++;
	}
	
	while((read2 = getline(&line2, &len2, fh2)) != -1) {
		b[j] = (char *)malloc(sizeof(char) * size);
		b[j] = strdup(line2);
		j++;
	}

	fclose(fh1);
	fclose(fh2);

	/*for -w*/
	if(ignore_all_space_flag == 1) {
		int  k, l, m;
		k = 0;
		while(k < str1) {
			m = 0;
			l = 0;
			while(a[k][l] != '\0') {
				if(!(a[k][l] == ' ')) {
					a[k][m] = a[k][l];
					m++;
				}
				l++;
			}
			a[k][m] = '\0';
			k++;
		}

		k = 0;
		while(k < str2) {
			m = 0;
			l = 0;
			while(b[k][l] != '\0') {
				if(!(b[k][l] == ' ')) {
					b[k][m] = b[k][l];
						m++;
				}
				l++;
			}
			b[k][m] = '\0';
			k++;
		}
	}

	/*for -i*/
	if(ignore_case_flag == 1) {
		int k = 0;
		int l = 0;
		while(k < str1) {
			l = 0;
			while(a[k][l] != '\0') {
				if(a[k][l] >= 'A' && a[k][l] <= 'Z') {
					a[k][l] = (a[k][l] + 32);
				}
				l++;
			}
			k++;
		}
		k = 0;
		l = 0;
		while(k < str2) {
			l = 0;
			while(b[k][l] != '\0') {
				if(b[k][l] >= 'A' && b[k][l] <= 'Z') {
					b[k][l] = (b[k][l] + 32);
				}
				l++;
			}
			k++;
	
		}
	}

	/*for -b*/
	if(ignore_space_change_flag == 1) {
		int k = 0;
		while(k < str1) {
			int c = 0, d = 0;
			while((a[k][c]) != '\0') {
				if(!(a[k][c] == ' ' && a[k][c + 1] == ' ')) {
					a[k][d] = a[k][c];
					d++;
			}
				c++;
			}
			a[k][d] = '\0';
			k++;
		}
	
		k = 0;
		while(k < str2) {
			int c = 0, d = 0;
			while((b[k][c]) != '\0') {
				if(!(b[k][c] == ' ' && b[k][c + 1] == ' ')) {
					b[k][d] = b[k][c];
					d++;
				}
				c++;
			}
			b[k][d] = '\0';
			k++;
		}
	
	}
	/*for -t*/
	if(ignore_all_tabs_flag == 1) {
		int  k, l, m;
		k = 0;
		while(k < str1) {
			m = 0;
			l = 0;
			while(a[k][l] != '\0') {
				if(!(a[k][l] == '\t')) {
					a[k][m] = a[k][l];
					m++;
				}
				l++;
			}
			a[k][m] = '\0';
			k++;
		}

		k = 0;
		while(k < str2) {
			m = 0;
			l = 0;
			while(b[k][l] != '\0') {
				if(!(b[k][l] == '\t')) {
					b[k][m] = b[k][l];
						m++;
				}
				l++;
			}
			b[k][m] = '\0';
			k++;
		}
	}
	/*for -B*/
	if(ignore_blank_lines_flag == 1) {
		int k = 0;
		int lastIndex = 0, lastspaceIndex = 0, i = 0;
		while(k < str1) {
			lastIndex = 0, lastspaceIndex = 0, i = 0;
			while(a[k][lastspaceIndex] == '\n')
				lastspaceIndex++;
			while(a[k][i + lastspaceIndex] != '\0') {
				a[k][i] = a[k][i + lastspaceIndex];
				i++;
			}
			a[k][i] = '\0';
			i = 0;
			while(a[k][i] != '\0') {
				if(a[k][i] == '\n')
					lastIndex = i;
				i++;
			}
			a[k][lastIndex + 1] = '\0';
			k++;
		}	

		k = 0;
		while(k < str2) {
			lastIndex = 0, lastspaceIndex = 0, i = 0;
			while(b[k][lastspaceIndex] == '\n')
				lastspaceIndex++;
			while(b[k][i + lastspaceIndex] != '\0') {
				b[k][i] = b[k][i + lastspaceIndex];
				i++;
			}
			b[k][i] = '\0';
			i = 0;
			while(b[k][i] != '\0') {
				if(b[k][i] == '\n')
					lastIndex = i;
				i++;
			}
			b[k][lastIndex + 1] = '\0';
			k++;
		}	

	}
	/*for -Z*/
	if(ignore_trailing_leading_space_blank_lines_flag == 1) {
		int k = 0;
		while(k < str1) {
			int lastIndex = 0, lastspaceIndex = 0, i = 0;
			while(a[k][lastspaceIndex] == ' ' || a[k][lastspaceIndex] == '\t' || a[k][lastspaceIndex] == '\n')
				lastspaceIndex++;
			while(a[k][i + lastspaceIndex] != '\0') {
				a[k][i] = a[k][i + lastspaceIndex];
				i++;
			}
			a[k][i] = '\0';
			i = 0;
			while(a[k][i] != '\0') {
				if(a[k][i] == ' ' || a[k][i] == '\t' || a[k][i] == '\n')
					lastIndex = i;
				i++;
			}
			a[k][lastIndex + 1] = '\0';
			k++;
		}	

		k = 0;
		while(k < str2) {
			int lastIndex = 0, lastspaceIndex = 0, i = 0;
			while(b[k][lastspaceIndex] == ' ' || b[k][lastspaceIndex] == '\t' || b[k][lastspaceIndex] == '\n')
				lastspaceIndex++;
			while(b[k][i + lastspaceIndex] != '\0') {
				b[k][i] = b[k][i + lastspaceIndex];
				i++;
			}
			b[k][i] = '\0';
			i = 0;
			while(b[k][i] != '\0') {
				if(b[k][i] == ' ' || b[k][i] == '\t' || b[k][i] == '\n')
					lastIndex = i;
				i++;
			}
			b[k][lastIndex + 1] = '\0';
			k++;
		}	

	}
	if(ignore_blank_lines_flag == 1 || ignore_trailing_leading_space_blank_lines_flag == 1) {
		/*removes blank array elements*/
		int k = 0, i = 0, c = str1;
		while(k < c) {
			if(a[k][0] == '\0') {
				for(i = k; i < str1; i++) {
					a[i] = a[i + 1];
				}	
				c--;
			}
			if((a[k][0] == '\0' && a[k + 1][0] == '\0'))
				continue;
			else {
				k++;
				if(k != 0 && (a[k - 1][0] == '\0' && a[k][0] != '\0'))
					k = k - 1;
			}
		}
		str1 = c;
		int l = 0, j = 0, d = str2;
		while(l < d) {
			if(b[l][0] == '\0') {
				for(j = l; j < str2; j++) {
						b[j] = b[j + 1];
				}	
				d--;
			}
			if((b[l][0] == '\0' && b[l + 1][0] == '\0'))
				continue;
			else {
				l++;
				if(l != 0 && (b[l - 1][0] == '\0' && b[l][0] != '\0'))
					l = l - 1;
			}
		}
		str2 = d;
	}
	/*for -q*/
	if(no_details_flag == 1) {
		int i = 0;
		while(i < str1) {
			if(strcmp(a[i], b[i]) == 0)
				i++;
			else {
				printf("Files %s and %s differ\n", argv1, argv2);
				return 0;
			}
		}
	}
	
	if(option != 0) {
		int i = 0;
		while(i < str1) {
			if(strcmp(a[i], b[i]) == 0)
				i++;
			else {
				free(a);
				free(b);
				str1 = numofLines(argv1);
				str2 = numofLines(argv2);	
				char **c, **d;
				fh1 = fopen(argv1, "r");
				fh2 = fopen(argv2, "r");
				if((fh1) == NULL || (fh2) == NULL) {
					displayError();
					return -1;
				}

				c = (char **)malloc(sizeof(char *) * (str1));
				d = (char **)malloc(sizeof(char *) * (str2));
				i = 0, j = 0;
				while((read1 = getline(&line1, &len1, fh1)) != -1) { 
					c[i] = (char *)malloc(sizeof(char) * size);
					c[i] = strdup(line1);
					i++;
				}
				
				while((read2 = getline(&line2, &len2, fh2)) != -1) {
					d[j] = (char *)malloc(sizeof(char) * size);
					d[j] = strdup(line2);
					j++;
				}
				LCS(d,c);
				free(c);
				free(d);
				fclose(fh1);
				fclose(fh2);
				return 0;
			}
		}
	}
	else {
		LCS(b,a);
		free(a);
		free(b);
	}
	if(line1)
		free(line1);
	if(line2)
		free(line2);
	return 0;
}

void LCS(char **a,char **b) {
	data d1, a1;
	queue qd, qa;
	int i = 0, j = 0;
	int m = 0, n = 0;
	m = str2;
	n = str1;
	int** S; 
	int** R; 
	S = (int **)malloc( (m+1) * sizeof(int *) ); 
	R = (int **)malloc( (m+1) * sizeof(int *) ); 
	for(i = 0; i < m + 1; i++) {
		S[i] = (int*) malloc( (n+1) * sizeof(int) ); 
		R[i] = (int*) malloc( (n+1) * sizeof(int) ); 
	}
	for(i = m; i >= 0; i--) {
		S[i][n] = 0;
		R[i][n] = DOWN;
	}
	for(j = n; j >= 0; j--) {
		S[m][j] = 0; 
		R[m][j] = RIGHT; 
	}
	for(i = m-1; i >= 0; i--) {
		for(j = n-1; j >= 0; j--) {
			if(strcmp(a[i],b[j]) == 0) { 
				S[i][j] = S[i+1][j+1] + 1;
				R[i][j] = DOWN_AND_RIGHT;
			} 
			else {
				S[i][j] = S[i+1][j+1] + 0;
				R[i][j] = NEITHER; 
			 
				if( S[i+1][j] >= S[i][j] ) { 
					S[i][j] = S[i+1][j]; 
					R[i][j] = DOWN; 
				} 
				if( S[i][j+1] >= S[i][j] ) {
					S[i][j] = S[i][j+1]; 
					R[i][j] = RIGHT; 
				}
			}
		}
	}
/*
LOGIC
if DOWN | add
if RIGHT < delete
if DOWN and RIGHT come simultaneously in forwardtracing then Change
*/
/* The length of the longest substring is S[n][m] */ 
	i = 0; 
	j = 0; 
	qinit(&qa);
	qinit(&qd);
/* Trace the forwardtracking matrix.   Printing the diff output */ 
	while(1) {
		if(i == m && j == n)
			break; 
		if( R[i][j] == DOWN_AND_RIGHT ) {
			if((!qempty(&qa) && !qempty(&qd))) {
				d1 = dequeue(&qd);
				a1 = dequeue(&qa);
				printf("%d", d1.j + 1);
				if(queuelen(qd) > 0)
					printf(",%d", d1.j + 1 + queuelen(qd));
				printf("c%d", a1.i + 1);
				if(queuelen(qa) > 0)
					printf(",%d", a1.i + 1 + queuelen(qa));
				printf("\n");
				printf("< %s", b[d1.j]);
				while(!qempty(&qd)) {
					d1 = dequeue(&qd);
					printf("< %s", b[d1.j]);
				}
				printf("- - -\n");
				printf("> %s", a[a1.i]);
				while(!qempty(&qa)) {
					a1 = dequeue(&qa);
					printf("> %s", a[a1.i]);
				}
				qinit(&qa);
				qinit(&qd);
			}
			else if(qempty(&qd) && !qempty(&qa)) {
				a1 = dequeue(&qa);
				printf("%d", a1.j);
				printf("a%d", a1.i + 1);
				if(queuelen(qa) > 0)
					printf(",%d", a1.i + 1 + queuelen(qa));
				printf("\n");
				printf("> %s", a[a1.i]);
				while(!qempty(&qa)) {
					a1 = dequeue(&qa);
					printf("> %s", a[a1.i]);
				}
				qinit(&qa);
			}
			else if((qempty(&qa) && !qempty(&qd))) {
				d1 = dequeue(&qd);
				printf("%d", d1.j + 1);
				if(queuelen(qd) > 0)
					printf(",%d", d1.j + 1 + queuelen(qd));
				printf("d%d\n", d1.i);
				printf("< %s", b[d1.j]);
				while(!qempty(&qd)) {
					d1 = dequeue(&qd);
					printf("< %s", b[d1.j]);
				}
				qinit(&qd);
			}
			i++;
			j++;
		}
		else if( R[i][j] == DOWN ) {
			a1.i = i;
			a1.j = j;
			enqueue(&qa, a1);
			i++; 
		} 
		else if( R[i][j] == RIGHT ) {
			d1.i = i;
			d1.j = j;
			enqueue(&qd, d1);
			j++; 
		}
	}

	if((!qempty(&qa) && !qempty(&qd))) {
		d1 = dequeue(&qd);
		a1 = dequeue(&qa);
		printf("%d", d1.j + 1);
		if(queuelen(qd) > 0)
			printf(",%d", d1.j + 1 + queuelen(qd));
		printf("c%d", a1.i + 1);
		if(queuelen(qa) > 0)
			printf(",%d", a1.i + 1 + queuelen(qa));
		printf("\n");
		printf("< %s", b[d1.j]);
		while(!qempty(&qd)) {
			d1 = dequeue(&qd);
			printf("< %s", b[d1.j]);
		}
		printf("- - -\n");
		printf("> %s", a[a1.i]);
		while(!qempty(&qa)) {
			a1 = dequeue(&qa);
			printf("> %s", a[a1.i]);
		}
		qinit(&qa);
		qinit(&qd);
	}
	else if(qempty(&qd) && !qempty(&qa)) {
		a1 = dequeue(&qa);
		printf("%d", a1.j);
		printf("a%d", a1.i + 1);
		if(queuelen(qa) > 0)
			printf(",%d", a1.i + 1 + queuelen(qa));
		printf("\n");
		printf("> %s", a[a1.i]);
		while(!qempty(&qa)) {
			a1 = dequeue(&qa);
			printf("> %s", a[a1.i]);
		}
		qinit(&qa);
	}
	else if((qempty(&qa) && !qempty(&qd))) {
		d1 = dequeue(&qd);
		printf("%d", d1.j + 1);
		if(queuelen(qd) > 0)
			printf(",%d", d1.j + 1 + queuelen(qd));
		printf("d%d\n", d1.i);
		printf("< %s", b[d1.j]);
		while(!qempty(&qd)) {
			d1 = dequeue(&qd);
			printf("< %s", b[d1.j]);
		}
		qinit(&qd);
	}
	for(i = 0; i < m + 1; i++ ) { 
		free(S[i]); 
		free(R[i]); 
	}
	free(S); 
	free(R);
	return; 
}
/*Binary files comparision*/
int comparebinaryfiles(char argv1[], char argv2[]) {
	FILE *fh1 = NULL;
	FILE *fh2 = NULL;
	int flag = 0;
	char c1, c2;

	if(strlen(argv1) <= 0 || strlen(argv2) <= 0) { 
		printUsage();
		return -1;
	}
	fh1 = fopen(argv1, "r");
	fh2 = fopen(argv2, "r");
	if((fh1) == NULL || (fh2) == NULL) {
		displayError();
		return -1;
	}
	while(((c1 = fgetc(fh1)) != EOF) && ((c2 = fgetc(fh2)) != EOF)) {
		if(c1 == c2) {
			flag = 1;
			continue;	
		}
		else {
			flag = 0;
			break;
		}
	}
	if(flag == 0)
		printf("Binary Files %s and %s differs\n", argv1, argv2);
	return 0;
}
/*Directories comparision*/
int comparedirectory(char argv1[], char argv2[], int option) {

	if ((directory1_flag == 1 && regular_file2_flag == 1) || (directory1_flag == 1 && binary_file2_flag == 1)) {
		struct dirent *pDirent1;
		DIR *pDir1;
		int flag = 0;
		char *g = NULL;
		g = (char *)malloc(sizeof(char)*50);
		pDir1 = opendir (argv1);
		if (pDir1 == NULL) {
			printf ("Cannot open directory '%s'\n", argv1);
			return 1;
		}
		while ((pDirent1 = readdir(pDir1)) != NULL) {
			if(strcmp(pDirent1->d_name, ".") == 0 || strcmp(pDirent1->d_name, "..") == 0) 
				continue;
			if(strcmp(pDirent1->d_name, argv2) == 0) {
				flag = 1;
				strcpy(g, argv1);
				strcat(g, "/");
				strcat(g, pDirent1->d_name);
				printf("./project %s %s\n", g, argv2);
				str1 = numofLines(g);
				str2 = numofLines(argv2);
				/*Check text file or binary file*/
				int fd,flag=0;
				char ch;
				fd = open(g, O_RDONLY);
				if(fd == -1) {
					perror("Error in open:");
					return (errno);
				}
				while(read(fd,&ch,1)) {
					if(!(isprint(ch) || isspace(ch)))
						flag = 1;	
				}
				if(flag==1)
					binary_file1_flag = 1;
				else
					regular_file1_flag = 1;
				close(fd);
				
				if(regular_file1_flag == 1 && regular_file2_flag == 1)
					comparefiles(g, argv2, option);
				else
					comparebinaryfiles(g, argv2);
				break;
			}
		}	
		if(flag == 0) {
			printf("%s : No such file in directory\n", argv2);
		}
		closedir(pDir1);
		free(g);
		return 0;
	}

	if ((directory2_flag == 1 && regular_file1_flag == 1) || (directory2_flag == 1 && binary_file1_flag == 1)) {
		struct dirent *pDirent2;
		DIR *pDir2;
		int flag = 0;
		char *h = NULL;
		h = (char *)malloc(sizeof(char)*50);
		pDir2 = opendir (argv2);
		if (pDir2 == NULL) {
			printf ("Cannot open directory '%s'\n", argv2);
			return 1;
		}

		while ((pDirent2 = readdir(pDir2)) != NULL) {
			if(strcmp(pDirent2->d_name, ".") == 0 || strcmp(pDirent2->d_name, "..") == 0)
				continue;
			if(strcmp(pDirent2->d_name, argv1) == 0) {
				flag = 1;
				strcpy(h, argv2);
				strcat(h, "/");
				strcat(h, pDirent2->d_name);
				printf("./project %s %s\n", argv1, h);
				str1 = numofLines(argv1);
				str2 = numofLines(h);	
				/*Check text file or binary file*/
				int fd,flag=0;
				char ch;
				fd = open(h, O_RDONLY);
				if(fd == -1) {
					perror("Error in open:");
					return (errno);
				}
				while(read(fd,&ch,1)) {
					if(!(isprint(ch) || isspace(ch)))
						flag = 1;	
				}
				if(flag==1)
					binary_file2_flag = 1;
				else
					regular_file2_flag = 1;
				close(fd);
				if(regular_file1_flag == 1 && regular_file2_flag == 1)
					comparefiles(argv1, h, option);
				else
					comparebinaryfiles(argv1, h);
				break;
			}
		}	
		if(flag == 0) {
			printf("%s : No such file in directory\n", argv1);
		}
		closedir(pDir2);
		free(h);
		return 0;
	}

	if (directory1_flag == 1 && directory2_flag == 1) {
		struct dirent *pDirent1, *pDirent2;
		DIR *pDir1, *pDir2;
		pDir1 = opendir (argv1);
		pDir2 = opendir (argv2);
		if (pDir1 == NULL) {
			printf ("Cannot open directory '%s'\n", argv1);
			return 1;
		}
		if (pDir2 == NULL) {
			printf ("Cannot open directory '%s'\n", argv2);
			return 1;
		}
		int i = 0, j = 0;
		filn1 = dirnum(argv1);
		filn2 = dirnum(argv2);
		char **t, **u;
		t = (char **)malloc(sizeof(char *) * (filn1));
		u = (char **)malloc(sizeof(char *) * (filn2));

		while ((pDirent1 = readdir(pDir1)) != NULL) {
			if(strcmp(pDirent1->d_name, ".") == 0 || strcmp(pDirent1->d_name, "..") == 0)
				continue;
			t[i] = (char *)malloc(sizeof(char) * 50);
			t[i] = strdup(pDirent1->d_name);
			i++;
		}
		while ((pDirent2 = readdir(pDir2)) != NULL) {
			if(strcmp(pDirent2->d_name, ".") == 0 || strcmp(pDirent2->d_name, "..") == 0) 
				continue;
			u[j] = (char *)malloc(sizeof(char) * 50);
			u[j] = strdup(pDirent2->d_name);
			j++;
		}
		int m = 0, n = 0;
		m = filn2;
		n = filn1;
		int** S; 
		int** R; 
		S = (int **)malloc( (m+1) * sizeof(int *) ); 
		R = (int **)malloc( (m+1) * sizeof(int *) ); 
		for(i = 0; i < m + 1; i++) {
			S[i] = (int*) malloc( (n+1) * sizeof(int) ); 
			R[i] = (int*) malloc( (n+1) * sizeof(int) ); 
		}
		for(i = m; i >= 0; i--) {
			S[i][n] = 0;
			R[i][n] = DOWN;
		}
		for(j = n; j >= 0; j--) {
			S[m][j] = 0; 
			R[m][j] = RIGHT; 
		}
		for(i = m-1; i >= 0; i--) {
			for(j = n-1; j >= 0; j--) {
				if(strcmp(u[i],t[j]) == 0) { 
					S[i][j] = S[i+1][j+1] + 1;
					R[i][j] = DOWN_AND_RIGHT;
				} 
				else {
					S[i][j] = S[i+1][j+1] + 0;
					R[i][j] = NEITHER; 
				 
					if( S[i+1][j] >= S[i][j] ) { 
						S[i][j] = S[i+1][j]; 
						R[i][j] = DOWN; 
					} 
					if( S[i][j+1] >= S[i][j] ) {
						S[i][j] = S[i][j+1]; 
						R[i][j] = RIGHT; 
					}
				}
			}
		}
		char *g = NULL, *h = NULL;
		g = (char *)malloc(sizeof(char)*50);
		h = (char *)malloc(sizeof(char)*50);
		i = 0; 
		j = 0; 
		while(1) {
			if(i == m && j == n)
				break;
			if( R[i][j] == DOWN_AND_RIGHT ) {
				strcpy(g, argv1);
				strcat(g, "/");
				strcat(g, t[j]);
				strcpy(h, argv2);
				strcat(h, "/");
				strcat(h, t[j]);
				printf("./project %s %s\n", g, h);
				str1 = numofLines(g);
				str2 = numofLines(h);
				/*Check text file or binary file*/
				int fd,flag=0;
				char ch;
				fd = open(g, O_RDONLY);
				if(fd == -1) {
					perror("Error in open:");
					return (errno);
				}
				while(read(fd,&ch,1)) {
					if(!(isprint(ch) || isspace(ch)))
						flag = 1;	
				}
				if(flag==1)
					binary_file1_flag = 1;
				else
					regular_file1_flag = 1;
				close(fd);

				flag=0;
				fd = open(h, O_RDONLY);
				if(fd == -1) {
					perror("Error in open:");
					return (errno);
				}
				while(read(fd,&ch,1)) {
					if(!(isprint(ch) || isspace(ch)))
						flag = 1;	
				}
				if(flag==1)
					binary_file2_flag = 1;
				else
					regular_file2_flag = 1;
				close(fd);
				
				if(regular_file1_flag == 1 && regular_file2_flag == 1)
					comparefiles(g, h, option);
				else
					comparebinaryfiles(g, h);
				i++;
				j++;
			}

			else if( R[i][j] == DOWN ) {
				printf("Only in %s : %s\n", argv2, u[i]);
				i++;
			} 
			else if( R[i][j] == RIGHT ) {
				printf("Only in %s : %s\n", argv1, t[j]);
				j++; 
			}
		}
		for(i = 0; i < m + 1; i++ ) { 
			free(S[i]); 
			free(R[i]); 
		}
		free(S); 
		free(R);
		closedir(pDir1);
		closedir(pDir2);
		free(t);
		free(u);
		free(g);
		free(h);
		return 0;
	}
	return 0;
}
/*Number of Files in Directory*/
int dirnum(char *argv) {
	int file_count = 0;
	DIR *dirp;
	struct dirent *entry;

	dirp = opendir(argv);	
	if (dirp == NULL) {
		printf ("Cannot open directory '%s'\n", argv);
		return 1;
	}
	while((entry = readdir(dirp)) != NULL) {
		file_count++;
	}
	closedir(dirp);
	return (file_count - 2);
}
