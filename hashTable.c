#define HASH_TABLE_LEN 600

//Node structure, will hold our sensor ready packets
typedef struct _Node
{
   unsigned int id;
   unsigned int arrNodeCapacity;
   unsigned int flag;
   unsigned int arrNodeCount;
   Dummy * stuff;
}Node;

//hashtable, made of a one dimensional array of nodes, the 
//sensor ready packet array will be pointed to by the 
//stuff pointer
typedef struct _hashTable
{
   unsigned int counter;
   unsigned int capacity;
   Node * table;
   double loadFactor;
   
}hashTable;

unsigned int hash(const int key)
{

}

Node * rehash(hashTable * hTable)
{
   if(hTable->loadFactor > 0.70)
   {
      
      int c = 0;
      Node * newTable = NULL;
      newTable = malloc(sizeof(Node) * hTable->capacity * 2);  
      newTable->capacity = hTable->capacity *2;
      newTable->counter = hTable->counter 
      newTable->loadFactor = newTable->counter / (double)newTable->capacity;
     i 
         
      for(;c < capacity; c++)
         newTable->table[c] = hTable->table[c];
      
      free(hTable->table);
      return newTable;
   }
   
   printf("The hashtable did not reach a value of 70 percent load factor \n");
   return NULL;
}

void hashDefault(hashTable * hTable)
{
   unsigned int c = 0;
   for(;c < hTable->capacity; c++)
      nodeDefault(&hTable->table[c]);
   hTable->counter = 0;
   hTable->capacity = 0;
   hTable->loadFactor = 0;
}

void nodeDefaullt(Node* curNode) 
{
   curNode->id = 0;
   curNode->arrNodeSize = 0;
   curNode->flag = 0;
   curNode->arrNodeCount = 0;
   curNode->stuff = NULL;
}

Node* nodeArr(Node * curNode)
{
   if(curNode->arrNodeCount + 1 >= curNode->arrNodeSize)
   {
      unsigned int newId = curNode->id;
      unsigned int newArrNodeSize = curNode arrNodeSize;
      unsigned int newFlag = curNode->flag;
      unsigned int newArrNodeCount = arrNodeCount;
      
   }

}
