// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO 1  // root i-number 根目录的inode number
#define BSIZE 512  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // Block number of first inode block
  uint bmapstart;    // Block number of first free map block
};

#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure
struct dinode {//16bytes
  short type;           // File type
  short major;          // Major device number (T_DEV only) 这是干什么的 使用的驱动类型
  short minor;          // Minor device number (T_DEV only) 具体的设备
  short nlink;          // Number of links to inode in file system 引用计数
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses 为什么要多开一个 因为最后一个指向INDIRECT的块
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.是文件名的长度
#define DIRSIZ 14

struct dirent {//16bytes fixme 所以说，每个inode都有一个entry存在硬盘里？除了root？
  ushort inum;
  char name[DIRSIZ];
};
//gerw 看完了
