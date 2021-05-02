#ifndef _INODE_H_
#define _INODE_H_

#include <cstdint>
#include "iobuf.h"
#include "fs.h"
#include "ucoredef.h"

UCORE_NAMESPACE_START

class fs;

class inode
{
private:
	int refCount;   // ���ü���
	int openCount;  // �򿪼���
	fs* in_fs;

public:
	inode(fs* in_fs)
		: refCount(0), openCount(0), in_fs(in_fs)
	{};
	virtual  ~inode() = default;

	// ʡ�Կ�������Ϳ�����ֵ

	virtual int open(uint32_t open_flags) = 0;
	virtual int close() = 0;
	virtual int read(iobuf* iob) = 0;
	virtual int write(iobuf* iob) = 0;

	/*ʡ����������*/
};

class device {
public:
	// ֱ�� public ����
	size_t d_blocks;
	size_t d_blocksize;

	device(size_t blocks, size_t blocksize) :
		d_blocks(d_blocks), d_blocksize(blocksize) {}

	int d_open(uint32_t open_flags) {};
	int d_close() {};
	int d_io(class iobuf* iob, bool write) {};
	int d_ioctl(int op, void* data) {};
};

class sfs_disk_inode {
	uint32_t size;
	uint16_t type;
	uint16_t nlinks;
	uint32_t blocks;
	uint32_t direct[12];
	uint32_t indirect;
};
class sfs_inode {
	sfs_disk_inode* din;
	uint32_t ino;
	bool dirty;
	int reclaim_count;
};

class DeviceInode :public inode
{
private:
	device _dev;	// ���device��

public:
	DeviceInode(device &dev, fs *fs):
		_dev(dev),inode(fs){}

	// ��װdevice�ķ���
	int d_open(uint32_t open_flags) { this->_dev.d_open(open_flags); };
	int d_close() { this->_dev.d_close(); };
	int d_io(iobuf* iob, bool write) { this->_dev.d_io(iob, write); };
	int d_ioctl(void* data) { this->d_ioctl(data); };

	// ʵ�ֳ����ඨ��Ľӿں���,ʡ�Ծ���ʵ��
	int open(uint32_t open_flags) {};
	int close() {};
	int read(iobuf* iob) {};
	int write(iobuf* iob) {};
};

class SfsInode : public inode
{
private:
	// ���sfs_inode
	sfs_inode _sfs_inode;
public:
	SfsInode(sfs_inode &sfs_inode, Sfs* in_fs) :
		inode(in_fs), _sfs_inode(sfs_inode)
	{};

	// ���麯����ʡ�Ծ���ʵ��
	int open(uint32_t open_flags) { return 0; };
	int close() { return 0; };
	int read(iobuf* iob) { return 0; };
	int write(iobuf* iob) { return 0; };
};



UCORE_NAMESPACE_END
#endif