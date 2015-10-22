#include "common.h"

#ifndef GDT_FILE_DEFINED
	#define GDT_FILE_DEFINED 1
	typedef struct{
		unsigned short limit_low, base_low;
		unsigned char base_mid, access_right;
		unsigned char limit_high, base_high;
	}  __attribute__ ((packed)) SegmentDescriptor;


	typedef struct{
		unsigned short gdtSize:16;
		SegmentDescriptor *baseAddress;
	} __attribute__ ((packed))  GdtToc;

	#define GDT_COUNT 1024
	void initGdt(void);

	#define AR_TSS32		0x0089

	typedef unsigned short GdtIndex;
	
	/*
		y==1
			����(�z��ԍ�)��GdtIndex
		y==0
			����(�擪����̃A�h���X)��GdtIndex
		
	*/
	#define getGdtIndex(x, y)	(y?(GdtIndex)(x):(GdtIndex)(x/sizeof(SegmentDescriptor)))
	/* */
	#define convertGdtIndex(x)	(x * sizeof(SegmentDescriptor))

	/*
		���ݎg���Ă��Ȃ�GDT���擾���܂��B
		�߂�l��0�̏ꍇ�̓G���[
	*/
	GdtIndex getEmptySegmentDescriptorIndex();
	void setSegmentDescriptor(GdtIndex index, unsigned int limit, ADDRESS base, unsigned int ar);
	void* getBaseAddressFromSegmentDescriptor(GdtIndex);
#endif