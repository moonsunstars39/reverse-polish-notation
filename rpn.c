/*
Program: rpn.c
Author: LiKun Li
Purpose: Reverse Polish notation
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int flag = 0;

typedef struct node {
	char* string;
	struct node *next;
	struct node *prev;
}stack;

char* reverse(char *string, int length) {
	int i;
	for (i = 0; i < strlen(string) / 2; i++) {
		char temp;
		temp = string[i];
		string[i] = string[strlen(string) - i - 1];
		string[strlen(string) - i - 1] = temp;
	}
	string[length] = '\0';
	return string;
}

char* eraseZero(char *string) {
	while (string[0] == '0') {
		memmove(string, string + 1, strlen(string));
	}
	//If all characters are zero, then we should output at least one 0.
	if (string[0] == '\0') {
		string[0] = '0';
		string[1] = '\0';
	}
	return string;
}


int CommandCheck(char* command) {
	char* commandC = "c";
	char* commandU = "u";
	char* commandP = "p";
	char* commandA = "a";
	char* commandPlus = "+";
	char* commandMinus = "-";

	if (strcmp(command, commandC) != 0 && strcmp(command, commandU) != 0 && strcmp(command, commandP) != 0 && strcmp(command, commandA) != 0 && strcmp(command, commandPlus) != 0 && strcmp(command, commandMinus) != 0) {
		return 1;
	}
	return 0;
}

char* stringAdd(char* string1, char* string2) {

	int len1 = 0;
	int len2 = 0;
	int i = 0;
	int j = 0;
	char* result = NULL;
	int sum = 0;
	int index = 0;
	int carry = 0;

	len1 = strlen(string1);
	len2 = strlen(string2);

	//Reverse the string 
	reverse(string1, len1);
	reverse(string2, len2);

	//allocating the memory
	result = malloc(sizeof(char)*(len1 + len2) + 2);
	while (i < len1 && j < len2) {
		sum = string1[i] - '0' + string2[j] - '0' + carry;
		carry = sum / 10;
		sum = sum % 10;
		result[index] = sum + '0';
		index++;
		i++;
		j++;
	}
	//len1 might be larger than len2, so we should keep checking.
	while (i < len1) {
		sum = string1[i] - '0' + carry;
		carry = sum / 10;
		sum = sum % 10;
		result[index] = sum + '0';
		index++;
		i++;
	}
	//len2 might be larger than len1, so we should keep checking.
	while (j < len2) {
		sum = string2[j] - '0' + carry;
		carry = sum / 10;
		sum = sum % 10;
		result[index] = sum + '0';
		index++;
		j++;
	}
	//At last, if carry is still over 0, means the new digit should be add.
	if (carry > 0) {
		result[index++] = carry + '0';
		result[index++] = '\0';
	}
	//If not, then just end the string.
	else {
		result[index++] = '\0';
	}
	reverse(result, index);
	return(eraseZero(result));
}

char *stringDif(char *string1, char *string2) {
	int Dflag = 0;
	int lenp1 = 0;
	int lenp2 = 0;
	char* TempString1 = NULL;
	char* TempString2 = NULL;
	int len1 = 0;
	int len2 = 0;
	int i = 0;
	int j = 0;
	char* result = NULL;
	char* subString = NULL;
	int sublength = 0;
	int difference = 0;
	int index = 0;
	len1 = strlen(string1);
	len2 = strlen(string2);
	//I do sub in different way, I erase all the zero before the first integer, and get the new string length first.
	TempString1 = malloc(len1 * sizeof(char) + 1);
	TempString2 = malloc(len2 * sizeof(char) + 1);
	strcpy(TempString1, eraseZero(string1));
	strcpy(TempString2, eraseZero(string2));
	lenp1 = strlen(TempString1);
	lenp2 = strlen(TempString2);
	//flag0 : Zero
	//flag1 : Positive
	//flag2 : Negative
	//Then, I check which of them are longer. The longer one must be the bigger one.
	if (lenp1 > lenp2) {
		Dflag = 1;
	}
	else if (lenp1 < lenp2) {
		Dflag = 2;
	}
	//However, if they are same length, then we can check each digits.
	//once we find the difference, we can know which one is bigger.
	else if (lenp1 == lenp2) {
		for (i = 0; i < lenp1; i++) {
			if (TempString1[i] - TempString2[i] > 0) {
				Dflag = 1;
				break;
			}
			else if (TempString1[i] - TempString2[i] < 0) {
				Dflag = 2;
				break;
			}
			//If the all characters are same, then we can know this two numbers are same, flag = 0.
			else if (TempString1[i] - TempString2[i] == 0) {
				continue;
			}
		}
	}
	result = malloc(sizeof(char)*(len1 + len2) + 2);
	reverse(TempString1, lenp1);
	reverse(TempString2, lenp2);
	if (Dflag == 1) {
		i = 0;
		j = 0;
		//subtracting each integers and get the difference between them, and put it into result.
		//If their difference is smaller than 0, then we should let the digit in front of it minus 1, and so this digit can plus 10.
		while (i < lenp1 && j < lenp2) {
			difference = (TempString1[i] - '0') - (TempString2[j] - '0');
			if (difference < 0 && TempString1[i + 1] != '\0') {
				result[index] = difference + 10 + '0';
				TempString1[i + 1] = TempString1[i + 1] - 1;
			}
			else {
				result[index] = difference + '0';
			}
			index++;
			i++;
			j++;
		}
		while (i < lenp1) {
			difference = TempString1[i] - '0';
			if (difference < 0) {
				result[index] = difference + 10 + '0';
				TempString1[i + 1] = TempString1[i + 1] - 1;
			}
			else
			{
				result[index] = difference + '0';
			}
			i++;
			index++;
		}
		free(TempString1);
		free(TempString2);
		result[index] = '\0';
		//reverse the result
		reverse(result, index);
		return(eraseZero(result));
	}
	if (Dflag == 2) {
		i = 0;
		j = 0;
		while (i < lenp1 && j < lenp2) {
			difference = (TempString2[j] - '0') - (TempString1[i] - '0');
			if (difference < 0) {
				result[index] = difference + 10 + '0';
				TempString2[j + 1] = TempString2[j + 1] - 1;
			}
			else {
				result[index] = difference + '0';
			}
			index++;
			i++;
			j++;
		}
		while (j < lenp2) {
			difference = TempString2[j] - '0';
			if (difference < 0) {
				result[index] = difference + 10 + '0';
				TempString2[j + 1] = TempString2[j + 1] - 1;
			}
			else {
				result[index] = difference + '0';
			}
			index++;
			j++;
		}
		result[index + 1] = '\0';
		result[index] = '-';
		//reverse the result
		reverse(result, index + 1);
		if (result[0] == '-') {
			subString = malloc(sizeof(char) * index + 2);
			for (i = 1; i < index + 1; i++) {
				subString[i - 1] = result[i];
			}
			subString[i - 1] = '\0';
			sublength = strlen(subString);
			subString = eraseZero(subString);
			for (i = 1; i < sublength; i++) {
				result[i] = subString[i - 1];
			}
			result[i] = '\0';
			free(subString);
		}
		free(TempString1);
		free(TempString2);
		return(eraseZero(result));
	}
	else {
		free(TempString1);
		free(TempString2);
		result[0] = '0';
		result[1] = '\0';
		return result;
	}
}

stack *addNode(stack *list_hd, char* string) {
	stack *newNode = NULL;

	newNode = malloc(sizeof(stack));
	memset(newNode, 0, sizeof(stack));
	newNode->string = malloc(sizeof(char) * strlen(string) + 1);
	strcpy(newNode->string, string);

	if (list_hd == NULL) {
		list_hd = newNode;
	}
	else {
		newNode->prev = NULL;
		newNode->next = list_hd;
		list_hd->prev = newNode;
		list_hd = newNode;
	}
	return list_hd;
}

int stackSize(stack *list_hd) {
	stack *cur = NULL;
	int i = 0;
	cur = list_hd;
	if (cur == NULL) {
		return 0;
	}
	while (cur != NULL) {
		cur = cur->next;
		i++;
	}
	return i;
}

void freeStack(stack *list_hd) {
	stack *cur = NULL;
	while ((cur = list_hd) != NULL) {
		list_hd = list_hd->next;
		free(cur->string);
		free(cur);
	}
}

int main() {

	stack *list_hd = NULL;
	char *string = NULL;
	size_t len = 0;
	int i = 0;
	int Lflag = 0;
	int stacksize = 0;

	char* commandC = "c";
	char* commandU = "u";
	char* commandP = "p";
	char* commandA = "a";
	char* commandPlus = "+";
	char* commandMinus = "-";

	while (getline(&string, &len, stdin) != EOF) {
		if (string[strlen(string) - 1] == '\n') {
			string[strlen(string) - 1] = '\0';
		}
		if (CommandCheck(string) == 1) {
			for (i = 0; i < strlen(string); i++) {
				if (!isdigit(string[i])) {
					fprintf(stderr, "Illegal input %s\n", string);
					flag = 1;
					Lflag = 1;
					break;
				}
			}
		}
		if (string[0] == '\0') {
			fprintf(stderr, "Illegal input %s\n", string);
			flag = 1;
			Lflag = 1;
		}
		if (Lflag == 1) {
			Lflag = 0;
			continue;
		}
		else if (CommandCheck(string) == 1) {
			string = eraseZero(string);
			list_hd = addNode(list_hd, string);
		}
		else if (strcmp(string, commandPlus) == 0 || strcmp(string, commandMinus) == 0) {
			int Nflag1 = 0;
			int Nflag2 = 0;
			int i = 0;
			stacksize = stackSize(list_hd);
			if (stacksize < 2) {
				fprintf(stderr, "Not enough operators\n");
				flag = 1;
				continue;
			}
			if (list_hd->string[0] == '-') {
				Nflag1 = 1;
				memmove(list_hd->string, list_hd->string + 1, strlen(list_hd->string));
			}
			if (list_hd->next->string[0] == '-') {
				Nflag2 = 1;
				memmove(list_hd->next->string, list_hd->next->string + 1, strlen(list_hd->next->string));
			}
			if ((strcmp(string, commandPlus) == 0 && (Nflag1 == Nflag2)) || (strcmp(string, commandMinus) == 0 && (Nflag1 != Nflag2))) {
				stack *cur = NULL;
				cur = list_hd;
				char* AddResult = stringAdd(cur->next->string, cur->string);
				int AddLength = strlen(AddResult);
				char* result = NULL;
				result = malloc(sizeof(char) * AddLength + 2);
				if ((Nflag1 == 0 && Nflag2 == 1) || (Nflag1 == 1 && Nflag2 == 1)) {
					for (i = 1; i < AddLength + 1; i++) {
						result[i] = AddResult[i - 1];
					}
					result[i] = '\0';
					result[0] = '-';
				}
				else {
					for (i = 0; i < AddLength; i++) {
						result[i] = AddResult[i];
					}
					result[i] = '\0';
				}
				cur = cur->next;
				cur->string = realloc(cur->string, sizeof(char)*strlen(result) + 1);
				strcpy(cur->string, result);
				free(result);
				free(AddResult);
				if ((cur = list_hd) != NULL) {
					list_hd = list_hd->next;
					free(cur->string);
					free(cur);
				}
			}
			else if ((strcmp(string, commandMinus) == 0 && (Nflag1 == Nflag2)) || (strcmp(string, commandPlus) == 0 && (Nflag1 != Nflag2))) {
				stack *cur = NULL;
				cur = list_hd;
				char* DifResult = stringDif(cur->next->string, cur->string);
				int DifLength = strlen(DifResult);
				char* result = NULL;
				result = malloc(sizeof(char) * DifLength + 2);
				if ((Nflag1 == 0 && Nflag2 == 1) || (Nflag1 == 1 && Nflag2 == 1)) {
					if (DifResult[0] == '-') {
						for (i = 0; i < DifLength; i++) {
							result[i] = DifResult[i + 1];
						}
						result[i] = '\0';
					}
					else {
						result[0] = '-';
						for (i = 1; i < DifLength + 1; i++) {
							result[i] = DifResult[i - 1];
						}
						result[i] = '\0';
						if (result[1] == '0') {
							memmove(result, result + 1, strlen(result));
						}
					}
				}
				else {
					for (i = 0; i < DifLength; i++) {
						result[i] = DifResult[i];
					}
					result[i] = '\0';
				}
				cur = cur->next;
				cur->string = realloc(cur->string, sizeof(char)*strlen(result) + 1);
				strcpy(cur->string, result);
				free(result);
				free(DifResult);
				if ((cur = list_hd) != NULL) {
					list_hd = list_hd->next;
					free(cur->string);
					free(cur);
				}
			}
		}
		else if (strcmp(string, commandA) == 0) {
			stack *cur = NULL;
			cur = list_hd;
			if (cur == NULL) {
				printf("Stack Empty\n");
			}
			else {
				while (cur->next != NULL) {
					cur = cur->next;
				}
				while (cur != list_hd) {
					printf("%s\n", cur->string);
					cur = cur->prev;
				}
				printf("%s\n", list_hd->string);
			}
		}
		else if (strcmp(string, commandC) == 0) {
			stack *cur = NULL;
			if (list_hd == NULL) {
				continue;
			}
			while ((cur = list_hd) != NULL) {
				list_hd = list_hd->next;
				free(cur->string);
				free(cur);
			}
		}
		else if (strcmp(string, commandU) == 0) {
			stack *cur = NULL;
			if (list_hd == NULL) {
				fprintf(stderr, "Nothing to undo\n");
				flag = 1;
				continue;
			}
			cur = list_hd;
			if ((cur = list_hd) != NULL) {
				list_hd = list_hd->next;
				free(cur->string);
				free(cur);
			}
		}
		else if (strcmp(string, commandP) == 0) {
			if (list_hd == NULL) {
				printf("Stack Empty\n");
			}
			else {
				printf("%s\n", list_hd->string);
			}
		}
	}
	free(string);
	freeStack(list_hd);
	if (flag == 1) {
		return 1;
	}
	return 0;
}
