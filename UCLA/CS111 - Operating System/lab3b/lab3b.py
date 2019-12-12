#Since we are flexible  in terms the  language  of choice, I  picked Python
# Every block pointer in an I-node or indirect block should be valid (a legal data block, within the file system) or zero. 
# Examine every single block pointer in every single I-node, direct block, indirect block, double-indirect block, and 
# triple indirect block to ascertain that this is true. But, remember that for symbolic links with a short length 
# (60 bytes or less) the i-node block pointer fields do not contain block numbers, and so should not be analyized.
import sys, string, locale, csv
#sets of variables, arrays, sets   to  be used in  the code
totalNumberOfBlocks = 0
totalNumberOfInodes = 0
#will be used to check if a  block exist in this list
reservedBlocksSet = set([0,1,2,3,4,5,6,7,64])
linkCount = {}
usedBlocks = {} 
linkChecker = {}
directoryInodeInfo = {}
parentChildInodes = {}
childParentInodes = {}
freeBlocks = set() 
freeInodes = set()
exitCode=0
#An INVALID block is one whose number is less than zero or greater than the highest block in the file system.
#A RESERVED block is one that could not legally be allocated to any file because it should be reserved for file system metadata 
if __name__ == '__main__':
    #reading the file
    try:
        #open, readonly
        input = open(sys.argv[1] , "r")
    #if it  wasn't successful
    except:
        #output  in  stderr
        sys.stderr.write("The file doesn't exist\n")
        exit(1)
    #iterate through  the file, lines
    #then have  if statements to check the  first  elemntof the array 
    #it will be either SUPPERBLOCK,  BFREE, IFREE, DIRENT, INODE, INDIRECT
    #for each do their own thing
    for line in input:
        #then create an  array  for each line
        rowArray = line.split(',')
        #if  it's a SIPERBLOCK
        if rowArray[0] == "SUPERBLOCK":
            #then grab the  number of  blocks and Inodes
            #rowArray[0] is "SUPERBLOCK"
            totalNumberOfBlocks = int(rowArray[1])
            totalNumberOfInodes = int(rowArray[2])
            
        #if the first element is  BFREE
        elif rowArray[0] == "BFREE":
            #then push it the freeBlocks array
            freeBlocks.add(int(rowArray[1]))
            
        #if the first element is IFREE
        elif rowArray[0] == "IFREE":
            #then push the second element in the freeInodes array
            freeInodes.add(int(rowArray[1]))
            
        #if the first element is DIRENT
        elif rowArray[0] == "DIRENT":
            #parent inode number (decimal) ... the I-node number of the directory that contains this entry
            parentInodeNumber = int(rowArray[1]) 
            #inode number of the referenced file (decimal); current Inode
            inodeNumberOfRefrencedFile = int(rowArray[3]) 
            #name of the file, directory
            nameOfInode = rowArray[6]
            directoryInodeInfo[inodeNumberOfRefrencedFile] = nameOfInode 
            if inodeNumberOfRefrencedFile not in linkChecker: 
                linkChecker[inodeNumberOfRefrencedFile] = 1
            else:
                linkChecker[inodeNumberOfRefrencedFile] += 1
             # An INVALID I-node is one whose number is less than 1 or greater than the last I-node in the system
            if inodeNumberOfRefrencedFile < 1 or inodeNumberOfRefrencedFile > totalNumberOfInodes: 
                sys.stdout.write("DIRECTORY INODE " + str(parentInodeNumber) + " NAME " + nameOfInode[0:len(nameOfInode)-2] + "' INVALID INODE " + str(inodeNumberOfRefrencedFile) + '\n')
                exitCode = 2
                continue
            # check for '.'
            elif nameOfInode[0:3] == "'.'" : 
                #'.' means the current directory so it should match with its parent
                if parentInodeNumber != parentInodeNumber:
                    sys.stdout.write("DIRECTORY INODE " + str(parentInodeNumber) + " NAME " + nameOfInode[0:3] + " LINK TO INODE " + str(inodeNumberOfRefrencedFile) + " SHOULD BE " + str(parentInodeNumber) + '\n')
                    exitCode = 2
                else:
                    continue  
            #check for '..', parent directory
            #basically since this referes to the previous dir, we keep track of it in a an array to do a check later
            elif nameOfInode[0:4] == "'..'":
                parentChildInodes[parentInodeNumber] = inodeNumberOfRefrencedFile
            else:
                childParentInodes[inodeNumberOfRefrencedFile] = parentInodeNumber    
        #if  the first elemnt is INODE
        elif rowArray[0] == "INODE":
            #grab the inode number
            currentInodeNumber= int(rowArray[1])
            #grab the  link count: indicating how many times this particular inode is linked
            #Symbolic links do not affect the link count of an inode. When the link count reaches 0 the inode and all its associated blocks are freed.
            linkCount[currentInodeNumber]=int(rowArray[6])
            #by looking at the CSV file, i realized I  need to start reading from the 12th element(12 to 27) because those are the pointers to the blocks
            #12 +15=27
            for ptr in range(12,27):
                currentBlock= int(rowArray[ptr])
                inodeOffset=0
                levelNumber=0
                levelName=""
                #current block is not  being used so  contunie
                if currentBlock== 0:
                    continue
                #if the first indirect is being used
                elif ptr==24:
                    inodeOffset=12
                    levelName=" INDIRECT"
                    levelNumber=1
                #if the doubly indirect is being used
                elif ptr==25:
                    inodeOffset=  12 +256
                    levelName=" DOUBLE INDIRECT"
                    levelNumber=2
                #if the triply indirect is being used
                elif ptr==26:
                    inodeOffset= 12 + 256 + 65536
                    levelName=" TRIPLE INDIRECT"
                    levelNumber=3
                #An INVALID block is one whose number is less than zero or greater than the highest block in the file system.
                if currentBlock< 0  or currentBlock > totalNumberOfBlocks:
                    sys.stdout.write("INVALID" + levelName + " BLOCK " + str(currentBlock) + " IN INODE " + str(currentInodeNumber) + " AT OFFSET " + str(inodeOffset) + '\n') 
                    exitCode = 2
                 #A RESERVED block is one that could not legally be allocated to any file because it should be reserved for file system metadata 
                elif currentBlock in reservedBlocksSet and currentBlock!=0:
                    sys.stdout.write("RESERVED" + levelName + " BLOCK " + str(currentBlock) + " IN INODE " + str(currentInodeNumber) + " AT OFFSET " + str(inodeOffset) + '\n')
                    exitCode = 2 
                 #use dic of dics to keep track of used blocks
                check = [currentInodeNumber, levelNumber, inodeOffset]
                if currentBlock in usedBlocks:
                    usedBlocks[currentBlock].append(check)
                else:
                    usedBlocks[currentBlock] = [check]               
        #if  the first elemnt is INDIRECT
        elif rowArray[0] == "INDIRECT":
            #I-node number of the owning file (decimal)
            inodeNumberOfOwningFile= int(rowArray[1])
            #block number of the referenced block (decimal)
            blockNumber= int(rowArray[5])
            #(decimal) level of indirection for the block being scanned; grab the level number
            levelNumber=int(rowArray[2])
            if levelNumber==1:
                inodeOffset=12
                levelName="INDIRECT"
            #if the doubly indirect is being used
            elif levelNumber==2:
                inodeOffset=  12 +256
                levelName="DOUBLE INDIRECT"
            #if the triply indirect is being used
            elif levelNumber==3:
                inodeOffset= 12 + 256 + 65536
                levelName="TRIPLE INDIRECT"
            #check  for invalid or reversed
            if currentBlock < 0 and currentBlock > totalNumberOfBlocks:
                sys.stdout.write("INVALID"+ levelName +" BLOCK "+ str(currentBlock) + " IN NODE "+ str(currentInodeNumber)+ " AT OFFSET "+ str(inodeOffset)+ "\n")
                exitCode=2
            #A RESERVED block is one that could not legally be allocated to any file because it should be reserved for file system metadata 
            elif currentBlock in reservedBlocksSet and currentBlock!=0:
                sys.stdout.write("RESERVED" + levelName + " BLOCK " + str(currentBlock) + " IN INODE " + str(currentInodeNumber) + " AT OFFSET " + str(inodeOffset) + '\n')
                exitCode = 2
            #use dic of dics to keep track of used blocks
            check = [inodeNumberOfOwningFile, levelNumber, inodeOffset]
            if blockNumber in usedBlocks:
                usedBlocks[blockNumber].append(check)
            else:
                usedBlocks[blockNumber] = [check]
    #now after stroting the needed variables, now we do some other checks
    #  I-node Allocation Audits
    #  An allocated I-node will have some valid type (e.g., file or directory). Unallocated I-nodes (whose type should be zero) should not appear in INODE 
    #  summaries, but should be in the free list. Scan through all of the I-nodes to determine which are valid/allocated.
    #  Every unallocated I-Node should be on a free I-node list.
    for i in range(1, totalNumberOfInodes+1):
        #check for allocated link
        if i in linkCount and i in freeInodes:
            #ALLOCATED INODE 2 ON FREELIST
            sys.stdout.write("ALLOCATED INODE "+  str(i) +" ON FREELIST")
            exitCode=2
        #check for unallocated
        elif i not in linkCount and i not in childParentInodes and  i not in freeInodes and i not in (1, 3, 4, 5, 6, 7, 8, 9, 10):
            #UNALLOCATED INODE 17 NOT ON FREELIST
            sys.stdout.write("UNALLOCATED INODE " + str(i) + " NOT ON FREELIST")
            exitCode = 2
    #Every legal data block (every block between the end of the I-nodes and the start of the next group) should appear on on the free block list, 
    # or be allocated to exactly one file. Examine the free list to determine whether or not this is the case. If a block is not referenced by any file and 
    # is not on the free list, a message like the following should be generated to stdout:
        #UNREFERENCED BLOCK 37
    #A block that is allocated to some file might also appear on the free list. In this case a message like the following should be generated to stdout:
        #ALLOCATED BLOCK 8 ON FREELIST
    for i in range (1, totalNumberOfBlocks+1):
        if i not in  usedBlocks and i not in reservedBlocksSet and i not in freeBlocks:
            sys.stdout.write("UNREFERENCED BLOCK "+ str(i))
            exitCode=2
        elif  i in usedBlocks and i in freeBlocks:
            sys.stdout.write("ALLOCATED BLOCK "+ str(i)+ " ON FREELIST")
            exitCode=2
    #If a legal block is referenced by multiple files (or even multiple times in a single file), messages like the following (depending on precisely where 
    # the references are) should be generated to stdout for each reference to that block:
    # I used some sort of hash map, found online, no credit for me
    for i in usedBlocks:
        if len(usedBlocks[i]) > 1:
            exitCode =2 
            for j in usedBlocks[i]:
                #[currentInode, levelNumber, offset]
                levelNumber = int(j[1])
                levelName = ""
                if levelNumber == 1:
                    levelName = " INDIRECT"
                elif levelNumber == 2:
                    levelName = " DOUBLE INDIRECT"
                elif levelNumber == 3:
                    levelName = " TRIPLE INDIRECT"
                sys.stdout.write("DUPLICATE" + levelName + " BLOCK " + str(i) + " IN INODE " + str(j[0]) + " AT OFFSET " + str(j[2]) + '\n')
    #Directory Consistency Audits
    #For any allocated I-node whose reference count does not match the number of discovered links, an error message like the following should be 
    # generated to stdout:
        #INODE 2 HAS 4 LINKS BUT LINKCOUNT IS 5
    #basically we check for linkCount, directly from  inode, and the linkCheker, from DIRENT where inodes would  be used; inode number of the referenced file (decimal)
    for i in linkCount:
        linkNumberfromTheCounter= linkCount[i]
        if i in linkChecker:
            linkNumberFromChecker= linkChecker[i]
        else:
            linkNumberFromChecker=0
        if linkNumberFromChecker != linkNumberfromTheCounter:
            sys.stdout.write("INODE " + str(i)+ " HAS " + str(linkNumberFromChecker) + " LINKS BUT LINKCOUNT IS "+ str(linkNumberfromTheCounter)+ '\n')
            exitCode=2
    #Directory entries should only refer to valid and allocated I-nodes. An INVALID I-node is one whose number is less than 1 or greater than the last I-node
    #  in the system. While scanning the directory entries, check the validity and allocation status of each referenced I-node. For any reference to an 
    # invalid or unallocated I-node, an error message like the following should be generated to stdout:
        #DIRECTORY INODE 2 NAME 'nullEntry' UNALLOCATED INODE 17
        #DIRECTORY INODE 2 NAME 'bogusEntry' INVALID INODE 26
    #this one specifically checks for unallocated directories that are being refrenced from child-parent
    for i in directoryInodeInfo:
        if i in childParentInodes and i  in freeInodes:
            parent = childParentInodes[i]
            nameOfDirectory = directoryInodeInfo[i]
            sys.stdout.write("DIRECTORY INODE " + str(parent) + " NAME " + nameOfDirectory[0:len(nameOfDirectory)-2] + "' UNALLOCATED INODE " + str(i) + '\n')
            exitCode = 2
    #finally check links from the directory inodes match
    # We also know that every directory should begin with two links, one to itself (.) and one to its parent (..). While scanning each directory,
    #  check for the correctness of these two links and, for each detected inconsistency, a message like one of the following should be generated to stdout:
        # DIRECTORY INODE 2 NAME '..' LINK TO INODE 11 SHOULD BE 2
        # DIRECTORY INODE 11 NAME '.' LINK TO INODE 2 SHOULD BE 11
    for i in parentChildInodes:
        if i == 2:
            if parentChildInodes[i] == 2:
                continue
            else:
                sys.stdout.write("DIRECTORY INODE " + str(i) + " NAME '..' LINK TO INODE " + str(parentChildInodes[i]) + " SHOULD BE " + str(i) + '\n')
                exitCode = 2
        elif i not in childParentInodes:
            sys.stdout.write("DIRECTORY INODE " + str(i) +" NAME '..' LINK TO INODE " + str(i) + " SHOULD BE " + str(childParentInodes[i]) + '\n')
            exitCode = 2
        elif parentChildInodes[i] != childParentInodes[i]:
            sys.stdout.write("DIRECTORY INODE " + str(i)+ " NAME '..' LINK TO INODE " + str(i) + " SHOULD BE " + str(childParentInodes[i]) + '\n')
            exitCode = 2
#exit
exit(exitCode)

        
        
        


        
        



       
