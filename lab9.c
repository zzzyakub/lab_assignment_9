#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// HashNode structure for separate chaining
struct HashNode
{
    struct RecordType data;
    struct HashNode* next;
};

// HashType with separate chaining
struct HashType
{
    struct HashNode* head;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    // We will use a simple modulo-based hash function to distribute data evenly
    return x % hashSz;
}

// Function to insert a record into the hash table
void insertRecord(struct HashType* pHashArray, int hashSz, struct RecordType data)
{
    int index = hash(data.id, hashSz);

    struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newNode->data = data;
    newNode->next = NULL;

    if (pHashArray[index].head == NULL)
    {
        // No collision, insert at the head
        pHashArray[index].head = newNode;
    }
    else
    {
        // Collision, insert at the end of the chain
        struct HashNode* current = pHashArray[index].head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);

        struct HashNode* current = pHashArray[i].head;
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }

        printf("NULL\n");
    }
}

// Function to free the memory allocated for the hash table
void freeHash(struct HashType* pHashArray, int hashSz)
{
    int i;
    for (i = 0; i < hashSz; ++i)
    {
        struct HashNode* current = pHashArray[i].head;
        while (current != NULL)
        {
            struct HashNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pHashArray);
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

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    int hashSz = 10; // You can adjust the hash table size as needed

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create the hash table with separate chaining
    struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
        printf("Memory allocation failed!\n");
        free(pRecords);
        return 1;
    }

    // Initialize the hash table
    int i;
    for (i = 0; i < hashSz; ++i)
    {
        pHashArray[i].head = NULL;
    }

    // Insert records into the hash table
    for (i = 0; i < recordSz; ++i)
    {
        insertRecord(pHashArray, hashSz, pRecords[i]);
    }

    // Display records stored in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    // Clean up and free memory
    freeHash(pHashArray, hashSz);
    free(pRecords);

    return 0;
}