#include "StdAfx.h"
#include "patient.h"
#include "ZNDLoader.h"
#include "ZNDData.h"


int g_B_SIZE[PART_MAX]={29,10,10,7, 6, 7,6, 7, 2,1,1};
int g_SIZE[PART_MAX]={29,10,10,7,8,8,8,8,2,1,1};

int ZNDLoader::load(CFile * file, ZndFileBuffer * pzndFileBuffer )
{
	memset(pzndFileBuffer, 0, sizeof(ZndFileBuffer));
	if(file->Read(pzndFileBuffer->buffer, ZND_FILE_LENGTH)!=ZND_FILE_LENGTH)
	{
		return -2;
	}
	return setupPointer(pzndFileBuffer);
}

int ZNDLoader::load( const TCHAR * fileName, ZndFileBuffer * pzndFileBuffer )
{	
	memset(pzndFileBuffer, 0, sizeof(ZndFileBuffer));

	FILE * fp;
	_tfopen_s(&fp,fileName, _T("rb"));
	if(!fp)
	{
		return -1;
	}
	if(fread(pzndFileBuffer->buffer, 1, ZND_FILE_LENGTH, fp)!=ZND_FILE_LENGTH)
	{
		fclose(fp);
		return -2;
	}
	fclose(fp);
	return setupPointer(pzndFileBuffer);
}
int ZNDLoader::setupPointer(ZndFileBuffer * pzndFileBuffer)
{
	pzndFileBuffer->PZndHeader=(ZND_HEADER*)pzndFileBuffer->buffer;
	if(memcmp(pzndFileBuffer->PZndHeader->zndId, "ZND", 4)!=0)
	{
		return -3;
	}
	char *pPartOffset;
	for(int i=0; i< PART_MAX; i++)
	{
		if(pzndFileBuffer->PZndHeader->offset[i])
		{
			pPartOffset = pzndFileBuffer->buffer + pzndFileBuffer->PZndHeader->offset[i];
			pzndFileBuffer->zndData[i].sampleInterval=*((double *)pPartOffset);
			pPartOffset +=sizeof(double);
			pzndFileBuffer->zndData[i].pMark=(short *)pPartOffset;
			pPartOffset += sizeof(short) * ZND_MARK_MAX;
			pzndFileBuffer->zndData[i].pWave=(float*)pPartOffset;
			pPartOffset += sizeof(float) * ZND_DATA_SIZE;
			if(i==HEART)
			{
				pzndFileBuffer->zndData[i].pHeartECG=(float*)pPartOffset;
				pPartOffset += sizeof(float) * ZND_DATA_SIZE;
			}
			pzndFileBuffer->zndData[i].pB_buffer=(double *) pPartOffset;
			pPartOffset +=sizeof(double) * g_SIZE[i];
			pzndFileBuffer->zndData[i].pC_buffer=(double *) pPartOffset;
		}
	}
	return 0;
}
