#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType* next;
};

// Fill out this structure
struct HashType
{
	struct RecordType** Arr;
	int size;
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	
	struct RecordType *print;

	for (int i=0; i<hashSz; ++i)
	{
		print = pHashArray->Arr[i];
		if (print != NULL)
		{
			printf("index %d", i);
			while (print != NULL)
			{
				printf(" -> %d, %c, %d", print->id, print->name, print->order);
				print = print->next;
			}
			printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType hashArray;
	hashArray.size = recordSz;
	hashArray.Arr = (struct RecordType**)malloc(hashArray.size * sizeof(struct RecordType*));

	for (int i = 0; i < hashArray.size; i++) {
		hashArray.Arr[i] = NULL;
	}

	if(hashArray.Arr == NULL)
	{
		printf("Memory not allocated");
		return -1;
	}

	for (int i = 0; i < recordSz; ++i)
	{
		int index = hash(pRecords[i].id);
		struct RecordType* temp = (struct RecordType*)malloc(sizeof(struct RecordType));
		if (temp == NULL)
		{
			printf("Memory not allocated");
			return -1;
		}
		*temp = pRecords[i];
		temp->next = hashArray.Arr[index];
		hashArray.Arr[index] = temp;
	}
	
	displayRecordsInHash(&hashArray, hashArray.size);

	for (int i = 0; i < hashArray.size; ++i)
	{
		struct RecordType* current = hashArray.Arr[i];
		while (current != NULL)
		{
			struct RecordType* next = current->next;
			free(current);
			current = next;
		}
	}
	free(hashArray.Arr);
	free(pRecords);

	return 0;
	
}