/* 
 * Project: mp4c dll 
 * Purpose: for python to get mp4 duration info
 * Author: Fluke <fluke.l@gmail.com>
 * Date: 2008-4-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once

//int __declspec(dllexport)GetMP4Duration(char *); 

unsigned long GetLong(const unsigned char *buf);

void GoIntoAtom(FILE *fp, unsigned long atom_size, unsigned long *offset);

void SkipAtom(FILE *fp, unsigned long atom_size, unsigned long *offset);

//int GetMP4Duration(char *filepath);