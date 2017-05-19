/*
 * Sync_long.c
 *
 *  Created on: Apr 16, 2017
 *      Author: Samer
 */

#include "Sync_long.h"

int min(int a, int b)
{
	return !(b<a)?a:b;
}

void search_frame_start(struct d_corr  cor[],int SYNC_length ,int *frame_start,float *freq_offset) {

	// sort list (highest correlation first)
	//d_cor.sort(compare_abs);
	int i=0;
	int j=0;
	int k=0;
	float z,r;
	struct d_corr temp;

		for(i=0;i<SYNC_length-1;i++)
		{
		for(j=i+1;j<SYNC_length;j++)
		{
			z=sqrt((cimag(cor[i].value)*cimag(cor[i].value))+(creal(cor[i].value)*creal(cor[i].value)));
			r=sqrt((cimag(cor[j].value)*cimag(cor[j].value))+(creal(cor[j].value)*creal(cor[j].value)));
		if(r>z)
		{
		temp=cor[j];
		cor[j]=cor[i];
		cor[i]=temp;
		}

		}

		}



	// copy list in vector for nicer access
	//vector<pair<gr_complex, int> > vec(d_cor.begin(), d_cor.end());
	//d_cor.clear();

	// in case we don't find anything use SYNC_length
	*frame_start = SYNC_length;

	for( i = 0; i < 3; i++) {
		for( k = i + 1; k < 4; k++) {
			float complex first;
			float complex second;
			if(cor[i].d_offset>cor[k].d_offset) {
				first = cor[k].value;
				second = cor[i].value;
			} else {
				first = cor[i].value;
				second = cor[k].value;
			}
			int diff=cor[i].d_offset-cor[k].d_offset;
			if(diff<0)
			{
			diff=diff*-1;
			}
			if(diff == 64) {
				*frame_start=min(cor[i].d_offset,cor[k].d_offset) -63;
				*freq_offset = cargf(first * conj(second))/64;

				// nice match found, return immediately
				return;

			} else if(diff == 63) {
				*frame_start=min(cor[i].d_offset,cor[k].d_offset)-63;
				*freq_offset = cargf(first * conj(second))/63;
			} else if(diff == 65) {
				*frame_start=min(cor[i].d_offset,cor[k].d_offset)-63;
				*freq_offset = cargf(first * conj(second))/65;
			}
		}
	}
}

float complex polarToComplex( float mag, float phase )
{

	float real,imag;
	real = mag * cos(phase);
	imag = mag * sin(phase);

	float complex temp = real + imag*I;

	return temp;


}

struct d_corr* defineList(int size)
{
	//struct d_corr* ptr=(struct d_corr*)malloc(size*sizeof(ptr));  //zenaib
	struct d_corr* ptr=(struct d_corr*)malloc(size*sizeof(struct d_corr));
	return ptr;
}

void ComplexToFloat(float complex* in ,int size,float*out)
{


	int i,k;
	k=0;
	for(i=0;i<size;i++)
	{
		out[k]=creal(in[i]);
		out[k+1]=cimag(in[i]);
		k+=2;
	}


}

void FloatToComplex (float*in , int size,float complex* out)
{

	float x,y;
	int i,k;
	k=0;

	for(i=0;i<size/2;i++)
	{
		x=in[k];
		y=in[k+1];
		out[i]=x+y*I;
		k+=2;
	}
}


float complex *  Sync_long_general_work(float complex* in,int ninput,short int* OutputSize)
{
	     float complex * out;
		 enum state d_state=Sync;
		 struct d_corr* List=NULL;
		 float* afterFloat;  //zenaib 3ayza as2l fe deh
		 //float complex* after; //zenaib (deh ana na2ltha t7t 3shan al size aly al mafrod ha7gz beh ha3rfoh t7t
		 //float* inFloat=(float*) malloc(2*ninput); //zenaib
		 float* inFloat=(float*) malloc(2*ninput*sizeof(float));
		 //float* LTFloat =(float*)malloc(2*64*sizeof(float)); //zenaib
		 float* LTFloat =(float*)malloc(2*64*sizeof(float));

		 int SYNC_LENGTH=320;

		 //float complex* inDelayed=NULL;
		 //delayy(in,inDelayed,320,ninput);
		 int rel,i,o,size,offsetInList,key,noutput;
		 float complex temp = 0+0*I;
		 int d_frame_start;
		 float d_freq_offset=0;

		 float complex LT[]=
		 	{-0.0455-1.0679*I,	0.3528-0.9865*I,0.8594+0.7348*I,0.1874+0.2475*I,0.5309-0.7784*I,-1.0218-0.4897*I,-0.3401-0.9423*I,0.8657-0.2298*I,0.4734+0.0362*I,0.0088-1.0207*I,-1.2142-0.4205*I,0.2172-0.5195*I,0.5207-0.1326*I,-0.1995+1.4259*I,1.0583-0.0363*I,0.5547-0.5547*I,0.3277+0.8728*I,-0.5077+0.3488*I,-1.1650+0.5789*I,0.7297+0.8197*I,0.6173+0.1253*I,-0.5353+0.7214*I,-0.5011-0.1935*I,-0.3110-1.3392*I,-1.0818-0.1470*I,-1.1300-0.1820*I,0.6663-0.6571*I,-0.0249+0.4773*I,-0.8155+1.0218*I,0.8140+0.9396*I,0.1090+0.8662*I,
		 	-1.3868-0.0000*I,0.1090-0.8662*I,0.8140-0.9396*I,-0.8155-1.0218*I,-0.0249-0.4773*I,0.6663+0.6571*I,-1.1300+0.1820*I,-1.0818+0.1470*I,-0.3110+1.3392*I,-0.5011+0.1935*I,-0.5353-0.7214*I,
		 	0.6173-0.1253*I,0.7297-0.8197*I,-1.1650-0.5789*I,-0.5077-0.3488*I,0.3277-0.8728*I,0.5547+0.5547*I,1.0583+0.0363*I,-0.1995-1.4259*I,0.5207+0.1326*I,0.2172+0.5195*I,-1.2142+0.4205*I,0.0088+1.0207*I,0.4734-0.0362*I,0.8657+0.2298*I,-0.3401+0.9423*I,-1.0218+0.4897*I,0.5309+0.7784*I,0.1874-0.2475*I,0.8594-0.7348*I,0.3528+0.9865*I,-0.0455+1.0679*I,1.3868-0.0000*I};


		noutput = ninput;
		key=1;

	while (key)
	{
		switch(d_state){


		 	case Sync:
		 	{
		 						///////////////// definitions of sync case ///////////////////
		 		List=defineList(ninput);

		 		ComplexToFloat(in,ninput,inFloat);

		 		ComplexToFloat(LT,64,LTFloat);

		 						///////////////// fire filtering ////////////////////////////////
		 		if(SYNC_LENGTH > (ninput-63 ) && (ninput-63)>0 )
		 		{
		 			size=ninput-63+1; //leah +1
		 			afterFloat=(float*)malloc(2*size*sizeof(float));
		 			DSPF_sp_fir_cplx(inFloat,LTFloat,afterFloat,64,ninput-63+1);
		 			// after= here contains the result of the Correlation of input with the LT seq
		 			//fe al function deh ahna b3ten pointer mesh bayshwara 3ala haga after float we dah maynf3sh fa ana 3ayza 27gzlh makan 2bl
		 			//ma ab3th le al function we al makan dah bay5talf al size bata3h 3ala 7asb ana da5lt fe any if so ana ha7gz gawa
		 		}

		 		else if (SYNC_LENGTH > (ninput-63) && (ninput-63 )<0 )
		 		{
		 			size=0;
		 			afterFloat=(float*)malloc(2*size*sizeof(float));
		 			DSPF_sp_fir_cplx(inFloat,LTFloat,afterFloat,64,0);
		 		}

		 		else
		 		{
		 			size=SYNC_LENGTH;
		 			afterFloat=(float*)malloc(2*size*sizeof(float));
		 			DSPF_sp_fir_cplx(inFloat,LTFloat,afterFloat,64,SYNC_LENGTH);
		 			// after= here contains the result of the Correlation of input with the LT seq
		 		}

		 		float complex* after=(float complex*) malloc(size*sizeof(float complex));
		 		FloatToComplex(afterFloat,2*size,after);

		 		i=0;
		 		o=0;
		 		offsetInList=0;
		 		while(i+63 < ninput)
		 		{
		 			List[i].value=after[i];
		 			List[i].d_offset =offsetInList;
		 			i++;
		 			offsetInList++;

		 			if(offsetInList == SYNC_LENGTH)
		 			{
		 				search_frame_start(List,SYNC_LENGTH,&d_frame_start,&d_freq_offset);
		 				offsetInList=0;
		 				d_state=Copy;
		 				//noutput = ninput - d_frame_start;
		 				break;

		 			}
		 		}
		 		free(after);
		 		free(afterFloat);
		 		free(List);
				free(LTFloat);
				free(inFloat);
		 		break;
		 	}
		 	case Copy:
		 	{
		 		i=0;
		 		o=0;
		 		out = (float complex*) calloc(noutput,sizeof(float complex));
		 		while(i<ninput && o<noutput)
		 		{
		 			rel = offsetInList - d_frame_start;

		 			if((rel >= 0) && (rel < 128 || ((rel-128) % 80) >15))
		 					{
		 						temp =polarToComplex(1, offsetInList*d_freq_offset );

		 						out[o] = in[i] * temp;
		 						o++;
		 					}
		 			i++;
		 			offsetInList++;


		 		}

		 		d_state=Reset;
		 		break;
		 	}
		 	case Reset:
		 	{
		 		key=0;
		 		break;
		 	}

		}

	}
	*OutputSize = o;
	return out;

}

