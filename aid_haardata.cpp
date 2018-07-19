// aid_haardata.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "adi_haar_structure.h"
using namespace std;

/*=============  D A T A  =============*/
int _tmain(int argc, _TCHAR* argv[])
{
	FILE                    *pInfile,*pOutfile;
	char                    caCascadeName[] = "haarcascade_frontalface_default.xml";
	CvHaarClassifierCascade *pCascade = 0;
	int                     j, l, p, temp, cntr;
	CvHaarFeature           *feature;
	int                     nFixedPointValue;
	p = 0;
	cntr = 0;

	cvReleaseHaarClassifierCascade(&pCascade);	//加上这句代码，错误和崩溃就解决了，感觉真的是莫名其妙，

	pCascade = (CvHaarClassifierCascade *)cvLoad(caCascadeName, 0, 0, 0);
	//pInfile = fopen("haarparams.txt", "wb+");
	pInfile = fopen("haarparams_adi.bin", "wb");
	if (pInfile == NULL)
	{
		printf("Unable to open the file \n");
	}

	pOutfile = fopen("parase_result.txt","wb+");
	if(pOutfile == NULL)
	{
		printf("Unable to open the file \n");
	}

	cntr += (int)fwrite(&pCascade->count, 1, 4, pInfile);
	fprintf(pOutfile,"%d,\n",pCascade->count);
	//cout << "Stages num = " << pCascade->count << endl;

	int stage_count = pCascade->count;
	int classifier_count = 0;
	int node_count = 0;
	

	for (temp = 0; temp < pCascade->count; temp++)
	{
		cntr += (int)fwrite(&pCascade->stage_classifier[temp].count, 1, 4, pInfile);
		fprintf(pOutfile,"%d,\n",pCascade->stage_classifier[temp].count);
		//cout << "Classifier num = " << pCascade->stage_classifier[temp].count << endl;

		for (j = 0; j < pCascade->stage_classifier[temp].count; j++)
		{
			classifier_count++;

			for(l = 0; l < pCascade->stage_classifier[temp].classifier[j].count; l++)
			{
				node_count++;

				cntr += (int)fwrite(&pCascade->stage_classifier[temp].classifier[j].count, 1, 4, pInfile);
				fprintf(pOutfile,"%d,\n",pCascade->stage_classifier[temp].classifier[j].count);
				//cout << "Nodetree num = " << pCascade->stage_classifier[temp].classifier[j].count << endl;

				feature = &pCascade->stage_classifier[temp].classifier[j].haar_feature[l];
				cntr += (int)fwrite(&feature->tilted, 1, 4, pInfile);
				fprintf(pOutfile,"%d,\n",feature->tilted);
				//cout << "tilted = " << feature->tilted << endl;

				for (p = 0; p < 3; p++)
				{
					cntr += (int) fwrite(&feature->rect[p].r.x, 1, 4, pInfile);
					cntr += (int) fwrite(&feature->rect[p].r.y, 1, 4, pInfile);
					cntr += (int) fwrite(&feature->rect[p].r.width, 1, 4, pInfile);
					cntr += (int) fwrite(&feature->rect[p].r.height, 1, 4, pInfile);
					nFixedPointValue = (int)(feature->rect[p].weight * 65536);
					cntr += (int)fwrite(&nFixedPointValue, 1, 4, pInfile);

					fprintf(pOutfile,"%d,\n%d,\n%d,\n%d,\n%d,\n",
						feature->rect[p].r.x,
						feature->rect[p].r.y,
						feature->rect[p].r.width,
						feature->rect[p].r.height,
						(int)(feature->rect[p].weight * 65536));

					/*cout << "rect : " << "x = " << feature->rect[p].r.x
					<< ",y = " << feature->rect[p].r.y
					<< ",width = " << feature->rect[p].r.width
					<< ",height = " << feature->rect[p].r.height
					<< ",weight = " << (int)(feature->rect[p].weight * 65536)
					<< endl;*/
				}

				nFixedPointValue = (int)(*pCascade->stage_classifier[temp].classifier[j].threshold *65536);
				cntr += (int) fwrite(&nFixedPointValue, 1, 4, pInfile);
				//cout << "Classifier threshold = " << (int)(*pCascade->stage_classifier[temp].classifier[j].threshold *65536) << endl;
				fprintf(pOutfile,"%d,\n",(int)(*pCascade->stage_classifier[temp].classifier[j].threshold *65536));

				cntr += (int) fwrite(pCascade->stage_classifier[temp].classifier[j].left, 1, 4, pInfile);
				//cout << "left = " << *pCascade->stage_classifier[temp].classifier[j].left << endl;
				fprintf(pOutfile,"%d,\n",*pCascade->stage_classifier[temp].classifier[j].left);

				cntr += (int) fwrite(pCascade->stage_classifier[temp].classifier[j].right, 1, 4, pInfile);
				//cout << "right = " << *pCascade->stage_classifier[temp].classifier[j].right << endl;
				fprintf(pOutfile,"%d,\n",*pCascade->stage_classifier[temp].classifier[j].right);

				nFixedPointValue = (int)(*pCascade->stage_classifier[temp].classifier[j].alpha * 65536);
				cntr += (int) fwrite(&nFixedPointValue, 1, 4, pInfile);
				//cout << "alpha = " << (int)(*pCascade->stage_classifier[temp].classifier[j].alpha * 65536);
				fprintf(pOutfile,"%d,\n",(int)(*pCascade->stage_classifier[temp].classifier[j].alpha * 65536));

				nFixedPointValue = (int)(*(pCascade->stage_classifier[temp].classifier[j].alpha + 1) * 65536);
				cntr += (int) fwrite(&nFixedPointValue, 1, 4, pInfile);
				//cout << ", " << (int)(*(pCascade->stage_classifier[temp].classifier[j].alpha + 1) * 65536) << endl;
				fprintf(pOutfile,"%d,\n",(int)(*(pCascade->stage_classifier[temp].classifier[j].alpha + 1) * 65536));
			}
		}

		nFixedPointValue = pCascade->stage_classifier[temp].threshold * 65536;
		cntr += (int)fwrite(&nFixedPointValue, 1, 4, pInfile);
		//cout << "Stage threshold = " << (int)(pCascade->stage_classifier[temp].threshold * 65536) << endl;
		fprintf(pOutfile,"%d,\n",(int)(pCascade->stage_classifier[temp].threshold * 65536));
	}

	cout << "Total stage num = " << stage_count
		<< ", Total classifier num = " << classifier_count
		<< ", Total node num = " << node_count
		<< endl;

	int total_data_bytes = 4 * (1 + stage_count * 2 + classifier_count * 22);
	cout << "ADI_TRAINED_FILE_SIZE = " << total_data_bytes << endl;

	int total_structure_memory = sizeof(ADI_HAARCLASSIFIERCASCADE)
		+ sizeof(ADI_HAARSTAGECLASSIFIER) * stage_count
		+ sizeof(ADI_HAARCLASSIFIER) * classifier_count
		+ sizeof(ADI_HAARFEATURE) * node_count
		+ sizeof(int32_t *) * (node_count * 5)
		+ sizeof(ADI_PVT_CLASSIFIERCASCADE)
		+ sizeof(ADI_PVT_HAARSTAGECLASSIFIER) * stage_count
		+ sizeof(ADI_PVT_HAARCLASSIFIER) * classifier_count
		+ sizeof(ADI_PVT_HAARTREENODE) * node_count
		+ sizeof(void *) * (node_count + classifier_count);

	cout << "ADI_MEMORYFOR_TRAINEDDATA = " << total_structure_memory << endl;

	fclose(pInfile);
	fclose(pOutfile);
	printf("Declare memory for file as %d bytes\n", cntr);
}







