#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <getopt.h>
#include "ext2_fs.h"
//declaring functions
void checkInodeEntries(int groupNumber, unsigned int inodeBlockNumber, unsigned int inodeTable);
void freeBlockEntries(int groupNumber , unsigned int blockNumber);
void inodeProcess(unsigned int inodeTable, unsigned int offsetStart, unsigned int inodeNumber );
void superBlockProcess();
void blockGroupProcess(int groupNumber, int total);
void directoryProcess(unsigned int inodeNumber , unsigned int inodeBlock);
//some global variables to be used in the code
#define SUPER_BLOCK_OFFSET 1024 
//this is the disc file descriptor that is an integer describing where to start looking
//assigning to -1 in the begining
int fileDescriptor = -1; 
//defining the superblock structure
struct ext2_super_block superBlock; 
//will be used for the block size
unsigned int blockSize;
//to format the time as desired
void formatTheTime(time_t time, char* buf) {
  time_t epoch = time;
  struct tm ts = *gmtime(&epoch);
  strftime(buf, 80, "%m/%d/%y %H:%M:%S", &ts);
}
//process inode
void inodeProcess(unsigned int inodeTable, unsigned int offsetStart, unsigned int inodeNumber ){
  struct ext2_inode inode;
  unsigned long offset = (offsetStart * sizeof(inode)) + (SUPER_BLOCK_OFFSET + ((inodeTable - 1) * blockSize));
  //if pread() failed
  if(pread(fileDescriptor, &inode, sizeof(inode), offset) < 0){
    fprintf(stderr , "reading the file failed\n");
    exit(2);
  }
  //from spec; non-zero link and mode are expected
  if(inode.i_mode == 0 || inode.i_links_count == 0){ 
    return;
  }
  //discovered this from searchng online
  unsigned int numberOFBlocks = (inode.i_blocks / (2 << superBlock.s_log_block_size)) * 2; 
  //for storing the typefile
  char typeOfTheFile;
  //only want top 4 bits, so shift by 12
  __u16 valOfTopFile = (inode.i_mode >> 12) << 12;
  //assigning the type file
  //file type ('f' for file, 'd' for directory, 's' for symbolic link, '?" for anything else)
  if(valOfTopFile == 0xa000){
    typeOfTheFile = 's';
  }
  else if(valOfTopFile == 0x8000){
    typeOfTheFile ='f';
  }
  else if(valOfTopFile == 0x4000){
    typeOfTheFile ='d';
  }
  else{
    typeOfTheFile ='?';
  }
  //format the time as desired, call the helper function to print to stdout
  char c_buf[20];
  char m_buf[20];
  char a_buf[20];
  formatTheTime(inode.i_ctime, c_buf);
  formatTheTime(inode.i_mtime, m_buf);
  formatTheTime(inode.i_atime, a_buf);
  /*
    INODE
    inode number (decimal)
    file type ('f' for file, 'd' for directory, 's' for symbolic link, '?" for anything else)
    mode (low order 12-bits, octal ... suggested format "%o")
    owner (decimal)
    group (decimal)
    link count (decimal)
    time of last I-node change (mm/dd/yy hh:mm:ss, GMT)
    modification time (mm/dd/yy hh:mm:ss, GMT)
    time of last access (mm/dd/yy hh:mm:ss, GMT)
      file size (decimal)
      number of (512 byte) blocks of disk space (decimal) taken up by this file
     */
  printf("INODE,%d,%c,%o,%d,%d,%d,%s,%s,%s,%d,%d", inodeNumber, typeOfTheFile, inode.i_mode & 0xfff, inode.i_uid, inode.i_gid, inode.i_links_count, c_buf, m_buf, a_buf, inode.i_size, numberOFBlocks);
  //printing out block addr's
  //For ordiary files (type 'f') and directories (type 'd') the next fifteen fields are block addresses
  unsigned int r;
  for (r = 0; r < 15; r++) {
    printf(",%d", inode.i_block[r]);
  }
  printf("\n");
  //processing directory entries
  //The I-node summary contains a list of all 12 blocks
  for(r = 0; r < 12; r++){
    if(inode.i_block[r] != 0 && typeOfTheFile == 'd'){
      directoryProcess(inodeNumber , inode.i_block[r]);
    }
  }
  //regular indirect inodes
  /*
    INDIRECT
    I-node number of the owning file (decimal)
    (decimal) level of indirection for the block being scanned ... 1 for single indirect, 2 for double indirect, 3 for triple
    logical block offset (decimal) represented by the referenced block. If the referenced block is a data block, this is the logical block offset of that block within the file. If the referenced block is a single- or double-indirect block, this is the same as the logical offset of the first data block to which it refers.
    block number of the (1, 2, 3) indirect block being scanned (decimal) . . . not the highest level block (in the recursive scan), but the lower level block that contains the block reference reported by this entry.
    block number of the referenced block (decimal)
  */
  if (inode.i_block[12] != 0) {
    __u32* blockPointers = malloc(blockSize);
    __u32 ptrs = blockSize / sizeof(__u32);
    unsigned long inodeOffsetRegular = SUPER_BLOCK_OFFSET + ((inode.i_block[12] - 1)*blockSize);
    pread(fileDescriptor, blockPointers, blockSize, inodeOffsetRegular);
    unsigned int i;
    for (i = 0; i < ptrs; i++) {
      if (blockPointers[i] != 0) {
	//if a directory
	if (typeOfTheFile == 'd') {
	  directoryProcess(inodeNumber, blockPointers[i]);
	}
	//write to the std out as desired
	printf("INDIRECT,%d,%d,%d,%d,%d\n",inodeNumber,1,12 + i,inode.i_block[12],blockPointers[i]);
      }
    }
    free(blockPointers);
  }
  //doubly indirect inodes
  if (inode.i_block[13] != 0) {
    __u32* inodeBlockPointers = malloc(blockSize);
    __u32 numberOfPointers = blockSize / sizeof(__u32);
    unsigned long inodeOffestForDoubly = SUPER_BLOCK_OFFSET + ((inode.i_block[13] - 1) * blockSize);
    pread(fileDescriptor, inodeBlockPointers, blockSize, inodeOffestForDoubly);
    unsigned int k;
    unsigned int logOffset = 256 + 12;
    for (k = 0; k < numberOfPointers; k++) {
      if (inodeBlockPointers[k] != 0) {
	printf( "INDIRECT,%d,%d,%d,%d,%d\n",inodeNumber,2,logOffset + k,inode.i_block[13],inodeBlockPointers[k]);
	//search through this indirect block to find its directory entries
	__u32* blockPtrs = malloc(blockSize);
	unsigned long indir_offset = SUPER_BLOCK_OFFSET + ((inodeBlockPointers[k] - 1) * blockSize);
	pread(fileDescriptor, blockPtrs, blockSize, indir_offset);
	unsigned int e;
	for (e = 0; e < numberOfPointers; e++) {
	  if (blockPtrs[e] != 0) {
	    //if a directory
	    if (typeOfTheFile == 'd') {
	      directoryProcess(inodeNumber, blockPtrs[e]);
	    }
	    printf("INDIRECT,%d,%d,%d,%d,%d\n",inodeNumber,1,logOffset + e,inodeBlockPointers[k],blockPtrs[e]);
	  }
	}
	free(blockPtrs);
      }
    }
    free(inodeBlockPointers);
  }
  //triply indirect inodes
  if (inode.i_block[14] != 0) {
    __u32* inode2BlockPointers = malloc(blockSize);
    __u32 numberOfPointers = blockSize / sizeof(__u32);
    unsigned long inodeOffsetTriple = SUPER_BLOCK_OFFSET + ((inode.i_block[14] - 1) * blockSize);
    pread(fileDescriptor, inode2BlockPointers, blockSize, inodeOffsetTriple);
    unsigned int j;
    unsigned int logOffset = 65536 + 256 + 12;
    for (j = 0; j < numberOfPointers; j++) {
      if (inode2BlockPointers[j] != 0) {
	printf("INDIRECT,%d,%d,%d,%d,%d\n",inodeNumber,3,logOffset + j,inode.i_block[14],inode2BlockPointers[j]);
	__u32* inodeBlockPtrs = malloc(blockSize);
	unsigned long inodeOffestForDoubly = SUPER_BLOCK_OFFSET + ((inode2BlockPointers[j] - 1) * blockSize);
	pread(fileDescriptor, inodeBlockPtrs, blockSize, inodeOffestForDoubly);
	unsigned int k;
	for (k = 0; k < numberOfPointers; k++) {
	  if (inodeBlockPtrs[k] != 0) {
	    printf("INDIRECT,%d,%d,%d,%d,%d\n",inodeNumber,2,logOffset + k,inode2BlockPointers[j], inodeBlockPtrs[k]);
	    __u32* blockPtrs = malloc(blockSize);
	    unsigned long indir_offset = SUPER_BLOCK_OFFSET + ((inodeBlockPtrs[k] - 1) * blockSize);
	    pread(fileDescriptor, blockPtrs, blockSize, indir_offset);
	    unsigned int l;
	    for (l = 0; l < numberOfPointers; l++) {
	      if (blockPtrs[l] != 0) {
		//if a directory
		if (typeOfTheFile == 'd') {
		  directoryProcess(inodeNumber, blockPtrs[l]);
		}
		printf("INDIRECT,%d,%d,%d,%d,%d\n",inodeNumber,1,logOffset + l,inodeBlockPtrs[k],blockPtrs[l]);
	      }
	    }
	    free(blockPtrs);
	  }
	}
	free(inodeBlockPtrs);
      }
    }
    free(inode2BlockPointers);
  }
}
void directoryProcess(unsigned int inodeNumber , unsigned int inodeBlock){
  //creating a struct
  struct ext2_dir_entry directory;
  unsigned long offset = SUPER_BLOCK_OFFSET + ((inodeBlock - 1)*blockSize);
  unsigned int bytes = 0;
  //loop as long as the bytes are less the block size
  while(bytes < blockSize){
    memset(directory.name , 0 , 256);
    //read for dir
    pread(fileDescriptor, &directory, sizeof(directory), offset + bytes);
    //if the directory inode is not zero, the write in stdout
    if(directory.inode != 0){
      memset(&directory.name[directory.name_len], 0, 256 - directory.name_len);
      /* 
            DIRENT
            parent inode number (decimal) ... the I-node number of the directory that contains this entry: inodeNumber
            logical byte offset (decimal) of this entry within the directory: bytes
	 inode number of the referenced file (decimal):  directory.inode
	 entry length (decimal): directory.rec_len
	 name length (decimal): directory.name_len
	 name (string, surrounded by single-quotes).: directory.name
            */ 
      printf("DIRENT,%d,%d,%d,%d,%d,'%s'\n",inodeNumber, bytes, directory.inode, directory.rec_len, directory.name_len, directory.name);
    }
    bytes += directory.rec_len;
  }
}
void freeBlockEntries(int groupNumber , unsigned int blockNumber){
  //byte to be freed
  char* freeingBytes = (char*) malloc(blockSize);
  //equation is from the offset of the blocks starting with super block plus the number of blocks till you get to the one is desired to be removed
  unsigned long blockOffset = SUPER_BLOCK_OFFSET + ((blockNumber - 1)*blockSize); 
  //this would give data block you want to start removal with
  //s_first_data_block: 32bit value identifying the first data block, in other word the id of the block containing the superblock structure
  //s_blocks_per_group:32bit value indicating the total number of blocks per group. This value in combination with s_first_data_block can be used to determine the block groups boundaries
  unsigned int tracker = superBlock.s_first_data_block +(superBlock.s_blocks_per_group * groupNumber); 
  if(pread(fileDescriptor,freeingBytes, blockSize, blockOffset) < 0){
    fprintf(stderr , "Failed in freeing the byte\n");
    exit(2);
  }
  unsigned int i;
  unsigned int j;
  for(i = 0;i < blockSize; i++){ //outer thing is an array of block size values
    char t = freeingBytes[i];
    //each one is 8 long
    for(j = 0;j < 8; j++){ 
      int status = t & 1;
      if(!status){
	/*
               BFREE
                number of the free block (decimal)
	*/
	printf("BFREE,%d\n" , tracker);
      }
      t >>= 1;
      tracker++;
    }
  }
  free(freeingBytes);
}
//this function is pretty similar to one one above however since this would check for inodes for processing
void checkInodeEntries(int groupNumber, unsigned int inodeBlockNumber, unsigned int inodeTable){
  int bytesInode = superBlock.s_inodes_per_group / 8;
  char* freeingBytes = (char*) malloc(bytesInode);
  unsigned long inodesOffset = SUPER_BLOCK_OFFSET + ((inodeBlockNumber - 1) * blockSize);
  unsigned int tracker = (superBlock.s_inodes_per_group * groupNumber) + 1;
  unsigned int start = tracker;
  //in case of failure
  if(pread(fileDescriptor, freeingBytes, bytesInode, inodesOffset) < 0){
    fprintf(stderr , "Failed in freeing the byte\n");
    exit(2);
  }
  int i;
  int j;
  //loop[ for bytes inode
  for(i = 0; i < bytesInode; i++){
    char t = freeingBytes[i];
    //8: each are 8 long
    for(j = 0; j < 8; j++){
      int status = t & 1;
      //if the inode's bitmap value is 0;inode free
      if(!status){
	/*
                IFREE
                number of the free I-node (decimal)
	*/
	printf("IFREE,%d\n" , tracker);
      }
      //if the inode's bitmap value is 1
      else if(status){
	inodeProcess(inodeTable, tracker - start, tracker);
      }
      t >>= 1;
      tracker++;
    }
  }
  free(freeingBytes);
}
//get the group info
//The block group descriptor table is an array of block group descriptor, used to define parameters of all the block groups. 
//It provides the location of the inode bitmap and inode table, block bitmap, number of free blocks and inodes, and some other useful information.
void blockGroupProcess(int groupNumber, int total){
  //vars to be used
  struct ext2_group_desc groupBlock;
  __u32 descriptionBlock = 0;
  //if the block size is 1024, then des is 2, otherwise 1
  if(blockSize == 1024){
    descriptionBlock = 2;
  }
  else{
    descriptionBlock = 1;
  }
  unsigned long offsetForGroup = descriptionBlock * blockSize + 32 * groupNumber;
  //check if pread() fails or not
  if(pread(fileDescriptor, &groupBlock, sizeof(groupBlock), offsetForGroup)< 0){
    fprintf(stderr, "Reading group file descriptor block failed\n");
    exit(2);
  }
  //number of blocks for each group 
  unsigned int blocksInEachGroup = superBlock.s_blocks_per_group; 
  if(groupNumber == total-1){
    blocksInEachGroup = superBlock.s_blocks_count - (superBlock.s_blocks_per_group * (total - 1));
  }
  //number of inodes for group
  unsigned int inodesInEachGroup = superBlock.s_inodes_per_group; 
  if(groupNumber == total-1){
    inodesInEachGroup = superBlock.s_inodes_count - (superBlock.s_inodes_per_group * (total - 1));
  }
  //stdout for the GROUP
  //bg_free_blocks_count: 16bit value indicating the total number of free blocks for the represented group.
  //g_block_bitmap: 32bit block id of the first block of the “block bitmap” for the group represented
  //bg_inode_table: 32bit block id of the first block of the “inode table” for the group represented.
  /*
    GROUP
    group number (decimal, starting from zero)
    total number of blocks in this group (decimal)
    total number of i-nodes in this group (decimal)
    number of free blocks (decimal)
    number of free i-nodes (decimal)
    block number of free block bitmap for this group (decimal)
    block number of free i-node bitmap for this group (decimal)
    block number of first block of i-nodes in this group (decimal)
  */
  printf("GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n" , groupNumber, blocksInEachGroup, inodesInEachGroup, groupBlock.bg_free_blocks_count, groupBlock.bg_free_inodes_count, groupBlock.bg_block_bitmap, groupBlock.bg_inode_bitmap, groupBlock.bg_inode_table);
  //to free the block entries, need the bitmap
  unsigned int blockBitmap = groupBlock.bg_block_bitmap;
  freeBlockEntries(groupNumber , blockBitmap);
  //get inode bitmap and table to check the entries
  unsigned int inodeBitmap = groupBlock.bg_inode_bitmap;
  unsigned int inodeTable = groupBlock.bg_inode_table;
  checkInodeEntries(groupNumber, inodeBitmap, inodeTable);
}
//process the super block
void superBlockProcess(){
  //if failed in reading
  //ssize_t pread(int fd, void *buf, size_t count, off_t offset)
  //pread() reads up to count bytes from file descriptor fd at offset
  //offset (from the start of the file) into the buffer starting at buf.
  //The file offset is not changed.
  if(pread(fileDescriptor, &superBlock, sizeof(superBlock), SUPER_BLOCK_OFFSET) < 0){
    fprintf(stderr, "Reading the super block failed\n");
    exit(2);
  }
  //check the magic number in which defines the file system.
  //s_magic: 16bit value identifying the file system as Ext2: The value is currently fixed to EXT2_SUPER_MAGIC of value 0xEF53.
  if(superBlock.s_magic != EXT2_SUPER_MAGIC){
    fprintf(stderr, "The block magic's number is not EXT2 val; the expected\n");
    exit(2);
  }
  //First check that the log of the block size is reasonable
  //The block size is computed using this 32bit value as the number of bits to shift left the value 1024. 
  //This value may only be non-negative.
  if(superBlock.s_log_block_size > 64){
    fprintf(stderr, "The block size passed is not valid\n");
    exit(2);
  }
  blockSize = SUPER_BLOCK_OFFSET << superBlock.s_log_block_size;
  //print to std out the super block vars
  //s_blocks_count: 32bit value indicating the total number of blocks in the system including all used, free and reserved. 
  //s_inodes_count: 32bit value indicating the total number of inodes, both used and free, in the file system
  // s_first_ino:   32bit value used as index to the first inode useable for standard files
  /*
      SUPERBLOCK
        total number of blocks (decimal)
        total number of i-nodes (decimal)
        block size (in bytes, decimal)
        i-node size (in bytes, decimal)
        blocks per group (decimal)
        i-nodes per group (decimal)
        first non-reserved i-node (decimal)    
  */
  printf("SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n" , superBlock.s_blocks_count, superBlock.s_inodes_count,
	 blockSize, superBlock.s_inode_size, superBlock.s_blocks_per_group, superBlock.s_inodes_per_group,
	 superBlock.s_first_ino);
}

//main
int main(int argc, char* argv[]) {
  //first a check for argc
  if(argc!=2){
    fprintf(stderr, "Invalud args!\n");
    exit(1);
  }
  //options
  struct option options[] = {
    {0,0,0,0}
  };
  //getopt fails
  if (getopt_long(argc, argv, "", options, NULL) != -1) {
    fprintf(stderr, "Invalud args!\n");
    exit(1);
  }
  //after passing the basic checks
  //check the files image for reading
  if((fileDescriptor = open(argv[1],O_RDONLY)) == -1){
    fprintf(stderr, "Reading the file image failed\n");
    exit(1);
  }
  //adter all checking then call a function to:
  //process the superblock in the passed file system image
  superBlockProcess();
  //getting number of blocks 
  int groupsNumber= 1 +  ((superBlock.s_blocks_count-1) / superBlock.s_blocks_per_group);
  int i ;
  //go through each one; I know that the assignment says only one group is given in the file system image, but since not sure about
  //the test cases, I do a loop to go through all, if there is more than one
  for(i=0 ; i < groupsNumber; i++){
    blockGroupProcess(i , groupsNumber);
  }
  return 0;
}
