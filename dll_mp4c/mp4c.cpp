/* 
 * Project: mp4c dll 
 * Purpose: for python to get mp4 duration info
 * Author: Fluke <fluke.l@gmail.com>
 * Date: 2008-4-23
 */

#include "mp4c.h"


unsigned long size_stack[100];
int top;

unsigned long GetLong(const unsigned char *buf)
{
	int ret = 0;
	for(int i=0;i<4;++i) {
		ret *= 256;
		ret += buf[i];
	}
	return ret;
}

void GoIntoAtom(FILE *fp, unsigned long atom_size, unsigned long *offset)
{
	// no seek , already in 
	// unused fp
	*offset += 8;   // len( size + 4cc )
	size_stack[++top] = atom_size;
}

void SkipAtom(FILE *fp, unsigned long atom_size, unsigned long *offset)
{
	fseek(fp, atom_size - 8, SEEK_CUR);
	*offset += atom_size;
}

extern "C" {
__declspec( dllexport ) int GetMP4Duration(char *filepath)
{
	FILE *fp;
	if( (fp = fopen(filepath, "rb")) == NULL ) {
		fprintf(stderr, "Error opening the file\n");
		return -1;
	}

	unsigned long filesize = 0;
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	//printf("filesize: %d\n", filesize);
	rewind(fp);

	// init the atom size stack
	top = 0;
	size_stack[top] = filesize;

	unsigned long duration = 0;

	char buf[5];
	unsigned long offset = 0;
	int deep = 0;
	while(1) {
		if(++deep > 100) {
			//printf("reach max depth, abort\n");
			break;
		}
		if(offset >= size_stack[top]) {
			if(--top < 0) {
				break;
			}
		}
		//printf("\noffset: %d\n", offset);
		fread(buf, 1, 4, fp);
		unsigned long atom_size = GetLong((unsigned char*)buf); buf[4] = 0;
		//printf("%X %X %X %X \n", buf[0], buf[1], buf[2], buf[3]);
		fread(buf, 1, 4, fp);
		//printf("Atom size: %d, name: %c%c%c%c\n", atom_size, buf[0], buf[1], buf[2], buf[3]);

		if(!strncmp("moov", buf, 4)) {
			//printf(">> Find moov\n");

			GoIntoAtom(fp, atom_size, &offset);
			continue;
		}

		// get mvhd, then we go back to the moov atom
		if(!strncmp("mvhd", buf, 4)) {
			//printf(">> Find mvhd\n");

			// get mvhd version
			fread(buf, 1, 4, fp);
			if( 0 != GetLong((unsigned char *)buf) ) {
				fprintf(stderr, "We only accept movie that mvhd version == 0, abort\n");
				break;
			}
			fseek(fp, 8, SEEK_CUR);  // skip the creation an monification time
			fread(buf, 1, 4, fp);   // get timescale
			unsigned long timescale = GetLong((unsigned char *)buf);
			fread(buf, 1, 4, fp);   // get duration
			duration = GetLong((unsigned char *)buf);
			duration /= timescale;


			// we break here because this time we only need the duration from mvhd, but not track->mdhd
			fclose(fp);
			return duration;

			// seek back to the beginning of this atom in order ot let the SkipAtom work correctly
			fseek(fp, -20, SEEK_CUR);

			SkipAtom(fp, atom_size, &offset);
			continue;
		}

		// default skip
		SkipAtom(fp, atom_size, &offset);
	}
	
	fclose(fp);
	return -1;
}

}