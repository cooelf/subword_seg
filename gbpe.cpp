#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>//for gcc 4.3
//#include <dir.h> for directory pass, invalid for vc

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cctype>
//#include <sstream>


#ifndef CLK_TCK
#define CLK_TCK 1000
#endif

using namespace std;

#define ppmmINeed std::map<std::string, std::vector<std::string> >

std::string mysyspath="c:\\curs\\project\\wq";
const char vers[256]="\nsubword segmentation, Version 1.0\n";
const int _folder=3;
const char _tempfolder[512]="__temp_20180210__",_tempname[512]="___TEMP_2018021035__";

const int _IT_NUM=0,_IT_LETTER=1;
unsigned  char _utf8_sz[3]={0xef,0xbb,0xbf};

class CMiniStringBag
{
public:

	virtual ~CMiniStringBag(){};

#ifdef USE_HASH_MAP
	  typedef __gnu_cxx::hash_map<std::string, int, hashfun_str> map_type;
#else
	  typedef std::map<std::string, int> map_type;
#endif

	  int _size;
	  map_type str2id;


	  CMiniStringBag() : _size(0) {}

	  std::vector<std::string> id2str;

	  int Put(const std::string & i)
	  {
		  map_type::const_iterator j = str2id.find(i);
		  if (j == str2id.end())
		  {
			  int id = (int)id2str.size();
			  id2str.push_back(i);
			  str2id[i] = id;
			  _size++;
			  return id;
		  }
		  return j->second;
	  }

	  int isIn(const std::string & i)
	  {
		  map_type::const_iterator j = str2id.find(i);
		  if (j == str2id.end())
		  {
			  return -1;
		  }
		  return j->second;
	  }

	  std::string Str(const int id) const
	  {
		  assert(id >= 0 && id < (int)id2str.size());
		  return id2str[id];
	  }

	  int Id(const std::string & i) const
	  {
		  map_type::const_iterator j = str2id.find(i);
		  if (j == str2id.end())  return -1;
		  return j->second;
	  }

	  int Size() const { return _size; }
	  int size() const { return _size; }

	  void Clear() { str2id.clear();id2str.clear(); _size = 0; }

	  map_type::const_iterator begin() const { return str2id.begin(); }

	  map_type::const_iterator end()   const { return str2id.end(); }



};



class CMicroStringBag :
	public CMiniStringBag
{
public:
	CMicroStringBag() : _size(0) {};
	~CMicroStringBag(void){};

#ifdef USE_HASH_MAP
	  typedef __gnu_cxx::hash_map<std::string, int, hashfun_str> map_type;
#else
	  typedef std::map<std::string, int> map_type;
#endif

	  std::vector<std::string> id2str;
	  int _size;
	  map_type str2id;
	  std::vector<int> typeCounter;
	  std::vector<std::string> mapProperty;

	  int Counter(const std::string & i)
	  {
		  map_type::const_iterator j = str2id.find(i);
		  if (j == str2id.end())
		  {
			  return 0;
		  }
		  return typeCounter[j->second];
	  }
	  int isIn(const std::string & i)
	  {
		  map_type::const_iterator j = str2id.find(i);
		  if (j == str2id.end())
		  {
			  return -1;
		  }
		  return j->second;
	  }
	  void Clear()
	  {
		  str2id.clear();
		  id2str.clear();
		  _size = 0;
		  typeCounter.clear();
		  mapProperty.clear();
	  }
	    void clear()
	  {
		  str2id.clear();
		  id2str.clear();
		  _size = 0;
		  typeCounter.clear();
		  mapProperty.clear();
	  }
		  void clearmap()
	  {
		  str2id.clear();
	  }
	   void cleararray()
	  {
		  id2str.clear();
	  }
      int Put(const std::string & i)
	  {
		  map_type::const_iterator j = str2id.find(i);
		  if (j == str2id.end())
		  {
			  int id = (int)id2str.size();
			  id2str.push_back(i);
			  str2id[i] = id;
			  _size++;
			  typeCounter.push_back(1);
			  return id;
		  }
		  else
			  typeCounter[j->second]++;
		  return j->second;
	  }
	  int Put(const std::string & i, const std::string &iproperty)
	  {
		  map_type::const_iterator j = str2id.find(i);
		  if (j == str2id.end())
		  {
			  int id = (int)id2str.size();
			  id2str.push_back(i);
			  str2id[i] = id;
			  _size++;
			  mapProperty.push_back(iproperty);
			  return id;
		  }

		  return j->second;
	  }
	  int Size() const { return _size; }
	  int size() const
	  {
		  return _size;
	  }
	  int sizeTrue() const
	  {
		  //2018.02.09
		  int s=0;
		  for(int i=0;i<_size;i++)
			  if(this->typeCounter[i]>0)
				  s++;
		  return s;
	  }
};

long getFileLen(FILE *fp)
{
	//get file length
	if(fseek(fp, 0, SEEK_END)!=0)
		printf("ft!\n");
	long pt = ftell(fp);
	rewind(fp);
	return pt;
}

void readLine2VectorString(string &inputline,vector<string> &vs,char cc)
{
	vs.clear();
	if(inputline.size()<1)
		return;

	int i,len=(int)inputline.size(),j;

	i=0;
	while(i<len && (inputline[i]==0x20 ||inputline[i]==cc))
		i++;

	if(i>=len)
		return;

	j=len-1;
	while(j>=0 && (inputline[j]==0x20 ||inputline[j]==cc))
		j--;

	string realstr=inputline.substr(i,j+1-i);
	len=(int)realstr.size();

	i=0;
	while(i<len)
	{
		j=i;
		while(j<len && !(realstr[j]==0x20 ||realstr[j]==cc))
			j++;
		vs.push_back(realstr.substr(i,j-i));
		i=j;

		while(i<len && (realstr[i]==0x20 ||realstr[i]==cc))
			i++;
	}
	return;
}

void readLine2VectorString(string &inputline,vector<string> &vs)
{
	vs.clear();
	if(inputline.size()<1)
		return;

	int i,len=(int)inputline.size(),j;

	i=0;
	while(i<len && (inputline[i]==0x20 ||inputline[i]==0x09))
		i++;

	if(i>=len)
		return;

	j=len-1;
	while(j>=0 && (inputline[j]==0x20 ||inputline[j]==0x09))
		j--;

	string realstr=inputline.substr(i,j+1-i);
	len=(int)realstr.size();

	i=0;
	while(i<len)
	{
		j=i;
		while(j<len && !(realstr[j]==0x20 ||realstr[j]==0x09))
			j++;
		vs.push_back(realstr.substr(i,j-i));
		i=j;

		while(i<len && (realstr[i]==0x20 ||realstr[i]==0x09))
			i++;
	}
	return;
}



inline

void line2hanziSeqUTF8(std::string str,std::vector<std::string> &hzsq)
{
    hzsq.clear();
    size_t i=0;
    while(i<str.size())
    {
		unsigned char s1=str[i];
        if(s1 <0x80)
        {
            hzsq.push_back(str.substr(i,1));
            i++;
        }
		else if(s1>=0xe0)
		{
            hzsq.push_back(str.substr(i,3));
            i+=3;
        }
        else
        {
            hzsq.push_back(str.substr(i,2));
            i+=2;
        }
    }
    return;
}



bool readTemplate( std::vector<std::string> &temps)
{

	std::string strline;
	int linecounter=0;

    FILE *fp=stdin;
	char *tBuf;//[257];
	tBuf =(char *)malloc(sizeof(char)*2000000L);
	temps.clear();
	while(fgets(tBuf, 2000000L,fp) != NULL)
    {
        linecounter++;
        if(strlen(tBuf)<1) continue;
        strline = tBuf;
        temps.push_back(strline);
    }
    free(tBuf);

	return true;
}



bool readTemplate(char *filelist, std::vector<std::string> &temps)
{
	ifstream fin;
	fin.open(filelist, ifstream::in);

	if(!fin.good())
	{
		cout << "Unable to open "<< filelist << " for reading.\n";
		return false;
	}

	std::string strline;
	int linecounter=0;

	temps.clear();
	while(getline(fin, strline, '\n'))
	{
		linecounter++;
		if(strline.length()==0)
			continue;
		temps.push_back(strline);
	}
	fin.close();
	fprintf(stderr,"FILE %s loading done.\n",filelist);//2012.07.10

	return true;
}


void sortFromKey(vector<double> &key, vector<int> &sortPos)
{
	//sortpos list min->max id list
	int i, j, tmpInt, size=(int)key.size();
	sortPos.resize(size);

	for(i=0;i<size;i++)
		sortPos[i]=i;
	for(i=0;i<size-1;i++)
	{
		for(j=i+1;j<size;j++)
		{
			if( key[sortPos[i]] > key[sortPos[j]] )
			{
				tmpInt = sortPos[i];
				sortPos[i]=sortPos[j];
				sortPos[j]=tmpInt;
			}
		}
	}
	return;
}

void sortFromKey(vector<int> &key, vector<int> &sortPos)
{
	//sortpos list min->max id list
	int i, j, tmpInt, size=(int)key.size();
	sortPos.resize(size);

	for(i=0;i<size;i++)
		sortPos[i]=i;
	for(i=0;i<size-1;i++)
	{
		for(j=i+1;j<size;j++)
		{
			if( key[sortPos[i]] > key[sortPos[j]] )
			{
				tmpInt = sortPos[i];
				sortPos[i]=sortPos[j];
				sortPos[j]=tmpInt;
			}
		}
	}
	return;
}

void sortFromKey( std::vector<std::string> & key, std::vector<int> & sortPos )
{
	//sortpos list MIN->MAX id list
	int i, j, tmpInt, size=(int)key.size();
	sortPos.resize(size);

	for(i=0;i<size;i++)
		sortPos[i]=i;
	for(i=0;i<size-1;i++)
	{
		for(j=i+1;j<size;j++)
		{
			if( strcmp( key[sortPos[i]].c_str(),key[sortPos[j]].c_str() )>0 )
			{
				tmpInt = sortPos[i];
				sortPos[i]=sortPos[j];
				sortPos[j]=tmpInt;
			}
		}
	}
	return;
}


std::string mergestring(std::vector<std::string> &hzsq,size_t a,size_t len )
{
	std::string out1="";
	for(size_t i=a;i<a+len;i++)
		out1=out1+hzsq[i];
	return out1;
}

void mergestring(std::vector<std::string> &hzsq,size_t a,size_t len,std::string &out1)
{
	out1="";
	for(size_t i=a;i<a+len;i++)
		out1=out1+hzsq[i];
	return;
}


size_t extractWordCountList(char *in1, std::vector<std::string> &vocablist, std::vector<double> &vocabcount)
{
	size_t maxSize=0,i;
	std::vector<std::string> seqs,vs;
	readTemplate(in1,seqs);
    CMicroStringBag ms;

	ms.Clear();
	for(i=0;i<seqs.size();i++)
	{
		if(i==0) seqs[i]=seqs[i].substr(3,seqs[i].size()-3);//remove utf8 bom head
		readLine2VectorString(seqs[i],vs);
		for(size_t j=0;j<vs.size();j++)
		{
			ms.Put(vs[j]);
			if(maxSize<vs[j].size())
				maxSize=vs[j].size();
		}
	}
	fprintf(stderr,"#lex=%d maxsize=%d\n",ms.Size(),maxSize);
	vocablist.clear();vocabcount.clear();
	for(i=0;i<ms.size();i++)
	{
		vocablist.push_back(ms.id2str[i]);
		vocabcount.push_back((double)(ms.typeCounter[i]));
	}
	return maxSize;
}

inline
void bigramUpdate(std::string &bistr,size_t ii,size_t jj,size_t maxSize,
	std::vector<double>  &vocabCounter,
	CMicroStringBag &bigram, std::vector<std::string> vobseg,
	std::vector<CMicroStringBag> &bigramAV1,std::vector<CMicroStringBag> &bigramAV2,
	std::vector<std::vector<size_t> > &bigramPosList,std::vector<double> &bigramWeight,int unsupType)
{
	int id2=bigram.Put(bistr);//frq update

	if(id2>=0)
    {
		if( unsupType%10==1||unsupType%10==4)//av init begin
		{			
					if(jj==0)
						bigramAV1[id2].Put("__PREV");
					else
						bigramAV1[id2].Put(vobseg[jj-1]);

					if(jj+2>=vobseg.size())
						bigramAV2[id2].Put("__NEXT");
					else
						bigramAV2[id2].Put(vobseg[jj+2]);			
		}//av init end
		
		bigramPosList[id2].push_back(jj+ii*maxSize);

		bigramWeight[id2] += vocabCounter[ii];
	}
	else
		fprintf(stderr,"impossible=4848\n");

	return;
}

void deleteMicroStringBag(CMicroStringBag &bi, std::string &cc)
{
	int id=bi.isIn(cc);
	if(id>=0)
	bi.typeCounter[id]--;

	return;
}

double calDLG(CMicroStringBag &bigram,CMicroStringBag &unigram, size_t &sum,std::string &ustr1,std::string &ustr2)
{
	double dlg=0,prob=0;
	int id1,id2,dlgit;

	id1=unigram.isIn(ustr1);
	id2=unigram.isIn(ustr2);
	dlgit=bigram.isIn(ustr1+ustr2);

	size_t X=sum-bigram.typeCounter[dlgit],ii;

	for(ii=0;ii<unigram.size();ii++)
	{
		if(ii!=(size_t)id1&&ii!=(size_t)id2)
			prob=(double)(unigram.typeCounter[ii])/(double)(X);
		else
			prob=(double)(unigram.typeCounter[ii]-bigram.typeCounter[dlgit])/(double)(X);
		
		if(prob>0)
		dlg+=prob*log(prob);
	}
	prob=(double)(bigram.typeCounter[dlgit])/(double)(X);
	if(prob>0)
	dlg+=prob*log(prob);

	return -dlg*(double)(X);
}

int maxBPE(CMicroStringBag &bigram,std::vector<CMicroStringBag> &bigramAV1,std::vector<CMicroStringBag> &bigramAV2,
	std::vector<double> &bigramWeight,std::vector<double> &bigramDLG,std::vector<double> &bigramPPMI,//CMicroStringBag &unigram,
	int unsupType)
{
	//max
	size_t score1=0,score=0,ii;
	double scoredlg=-1000000.0,scoredlg1=-1000000.0,
		scoreppmi=-2000000.0,scoreppmi1=-100000.0,
		lamda=0.5,weight;//DLG part for AV+DLG weighting
	int id2=-1;
	if(unsupType>20)
		lamda=(double)(unsupType)/100.0-0.14;

	for(ii=0;ii<bigram.Size();ii++)
	{
		weight=bigramWeight[ii];//word frq weight
		if(unsupType>=10) weight=1.0;//no weight

		if(unsupType%10==0)//org BPE
			score = bigram.typeCounter[ii]*(size_t)weight;
		if(unsupType%10==1 )//AV-BPE
		{
			score=bigramAV1[ii].sizeTrue();
			if(score>bigramAV2[ii].sizeTrue())
				score=bigramAV2[ii].sizeTrue();
			score = score*weight;
		}
		if(unsupType%10==2)//DLG BPE
		{
			scoredlg = bigramDLG[ii]* weight;
		}

		if(score1<score||scoredlg1<scoredlg||scoreppmi1<scoreppmi)
		{
			score1=score;
			scoredlg1=scoredlg;
			scoreppmi1=scoreppmi;
			id2=(int)ii;
		}
	}
	return id2;
}

size_t readVocab(char *in1, std::vector<std::string> &vocablist, std::vector<double> &vocabcount)
{
	std::vector<std::string> vss,vs;
	readTemplate(in1,vss);
	size_t ii,mss=0;
	vocablist.clear();vocabcount.clear();
	for(ii=0;ii<vss.size();ii++)
	{
		if(ii==0) vss[ii]=vss[ii].substr(3,vss[ii].size()-3);//remove utf8 bom head
		readLine2VectorString(vss[ii],vs);
		vocablist.push_back(vs[0]);
		vocabcount.push_back((size_t)atof(vs[1].c_str()));
		if(mss<vs[0].size()) mss=vs[0].size();
	}
	return mss;
}

void updateDLG(CMicroStringBag &unigramDLG,CMicroStringBag &bigram,
	std::string &unigram1, std::string &unigram2,
	std::vector<double> &unigramDLGscore,std::vector<double> &bigramDLGscore)
{
	int id3,id1,id2;
	std::string bistr=unigram1+unigram2;

	id1=unigramDLG.isIn(unigram1);
	id2=unigramDLG.isIn(unigram2);

	unigramDLG.typeCounter[id1]--;
	unigramDLG.typeCounter[id2]--;
	bigram.Put(bistr);
	id3=bigram.isIn(bistr);

	size_t sum=0;
	for(size_t ii=0;ii<unigramDLG.size();ii++)
		if(unigramDLG.typeCounter[ii]>=0)
		sum+=unigramDLG.typeCounter[ii];

	bigramDLGscore[id3] = //unigramDLGscore[id1]+unigramDLGscore[id2]-calLG(bigram, bistr);
		calDLG(bigram,unigramDLG,sum,unigram1,unigram2);
	return;
}

void updatePPMI(CMicroStringBag &unigramDLG,CMicroStringBag &bigram,
	std::string &unigram1, std::string &unigram2,
	std::vector<double> &bigramPPMI)
{
	int id3,id1,id2;
	std::string bistr=unigram1+unigram2;

	id1=unigramDLG.isIn(unigram1);
	id2=unigramDLG.isIn(unigram2);

	bigram.Put(bistr);
	id3=bigram.isIn(bistr);
	bigramPPMI[id3] = log((double)(unigramDLG.typeCounter[id1]))
		+log((double)(unigramDLG.typeCounter[id2]))-log((double)(bigram.typeCounter[id3]));

	return;
}

void BPETrain(char *in1, int a, int unsupType, size_t lexSize, char *out1)
{
	//input as plain text a==0, lexcon: a==1
	//unsupType=0: frq(i.e., BPE org), =1: AV, =2: DLG
	//lexSize: the expected output vocabulary size

	if(unsupType%10==0) fprintf(stderr,"FRQ BPE ...\n");
	if(unsupType%10==1) fprintf(stderr,"AV BPE ...\n");
	if(unsupType%10==2) fprintf(stderr,"DLG BPE ...\n");
	if(unsupType>=10 && unsupType<20) fprintf(stderr,"No ");
	fprintf(stderr,"Word count weights for ranking\n");

	std::vector<std::vector<std::string> > vobseglist;
	std::vector<std::string> vobseg,vocab,vsh;
	size_t ii,jj,orgsize,maxSize,kk,mm;
	int id2,id3,ida,idb;
	CMicroStringBag bigram,unigramDLG;
	std::vector<CMicroStringBag> bigramAV1,bigramAV2;
	std::vector<double> unigramDLGScore,bigramDLGScore,bigramPPMI,vocabcount,bigramWeight;
	std::vector<std::vector<size_t> > bigramPosList;
	std::string bistr,unistr,ccstr,ccc;
	double scoremax,weight;
	unsigned char buf[3] ={0xef,0xbb,0xbf};

	//extract word list from lexcon or plain text
	if(a==1)
	    maxSize = readVocab(in1,vocab,vocabcount);
	else
		extractWordCountList(in1,vocab,vocabcount);
	orgsize=vocab.size()*4;//set more if overflow
	fprintf(stderr,"\n");
	FILE *fp;
	fp=fopen("vocab.txt","wb");
	fwrite(buf,3,1,fp);//utf8 head
	for(size_t i=0;i<vocab.size();i++)
	{
		//if(vocabcount[i]>2)
		fprintf(fp,"%s %d\n",vocab[i].c_str(),vocabcount[i]);
	}
	fclose(fp);

	//init bigram
	vobseglist.clear();
	bigram.Clear();unigramDLG.Clear();
	bigramAV1.resize(orgsize);bigramAV2.resize(orgsize);
	unigramDLGScore.resize(orgsize);bigramDLGScore.resize(orgsize);
	bigramPPMI.resize(orgsize);
	bigramPosList.resize(orgsize);
	bigramWeight.resize(orgsize);
	std::fill(bigramWeight.begin(),bigramWeight.end(),0);
	for(ii=0;ii<vocab.size();ii++)
	{
		line2hanziSeqUTF8(vocab[ii],vobseg);//to line2hanziSeqUTF8
		/*vobseg.clear();
		for(jj=0;jj<vocab[ii].size();jj++)
			vobseg.push_back(vocab[ii].substr(jj,1));*/

		if(ii%100==0)
			fprintf(stderr,"vocab=%d\r",ii);
		for(jj=0;jj<vobseg.size();jj++)
		{
			unigramDLG.Put(vobseg[jj]);
			if(jj+1<vobseg.size())
			{
				bistr=vobseg[jj]+vobseg[jj+1];
				bigramUpdate(bistr,ii,jj,maxSize,vocabcount,bigram,vobseg,bigramAV1,bigramAV2,bigramPosList,bigramWeight,unsupType);
			}
		}
		vobseglist.push_back(vobseg);
	}
	fprintf(stderr,"\nbigram=%d\n",bigram.Size());

	//dlg part
	//std::fill(unigramDLGScore.begin(),unigramDLGScore.end(),0);
	std::fill(bigramDLGScore.begin(),bigramDLGScore.end(),0);
	size_t sumdlg=0;
	for(ii=0;ii<unigramDLG.Size();ii++)
		sumdlg+=unigramDLG.typeCounter[ii];
	for(ii=0;ii<bigram.Size();ii++)
	{
		line2hanziSeqUTF8(bigram.id2str[ii],vsh);// to line2hanziSeqUTF8
		bigramDLGScore[ii]=calDLG(bigram,unigramDLG,sumdlg,vsh[0],vsh[1]);
		ida=unigramDLG.isIn(vsh[0]);
		idb=unigramDLG.isIn(vsh[1]);
		bigramPPMI[ii]=log((double)(unigramDLG.typeCounter[ida]))
			+log((double)(unigramDLG.typeCounter[idb]))-log((double)(bigram.typeCounter[ii]));
	}

	//max
	id2=maxBPE(bigram,bigramAV1,bigramAV2,bigramWeight,bigramDLGScore,bigramPPMI,unsupType);
	fprintf(stderr,"\n");
	//merge lexSize times
	std::vector<size_t> mvc,mvccc;
	
	fp=fopen("vocab.substring.txt","wb");
	fwrite(buf,3,1,fp);//utf8 head

	for(ii=0;ii<lexSize;ii++)
	{
		if(ii%100==0)
			fprintf(stderr,"merge=%d,bigram=%d\r",ii,bigram.Size());
			
		unistr=bigram.id2str[id2];
		
		weight= bigramWeight[id2];
		if(unsupType>=10) weight=1.0;

		if(unsupType%10==0) scoremax=(double)(bigram.typeCounter[id2]);
		if(unsupType%10==1||unsupType%10==4)
		{
			scoremax=(double)bigramAV1[id2].sizeTrue();
			if(scoremax>(double)(bigramAV2[id2].sizeTrue()))
			     scoremax=(double)bigramAV2[id2].sizeTrue();
			if(unsupType%10==4)
			scoremax+=bigramDLGScore[id2];
		}
		if(unsupType%10==2) scoremax = bigramDLGScore[id2];
		
		scoremax=scoremax*weight;
		fprintf(fp,"%s %f\n",unistr.c_str(),scoremax);

		bigramAV1[id2].Clear();//delete max id2's AV
		bigramAV2[id2].Clear();

		bigramDLGScore[id2]=-10000000.0;
		unigramDLG.Put(bigram.id2str[id2]);//move dlg from bigram to unigram
		id3=unigramDLG.isIn(bigram.id2str[id2]);
		unigramDLG.typeCounter[id3]=bigram.typeCounter[id2];
		unigramDLGScore[id3]=bigramDLGScore[id2];
		
		bigramPPMI[id2]=-10000.0;

		bigram.typeCounter[id2]=0;//delete max id2's bigram

		//merge max
		jj=0;
		bool doit=false;
		while(jj<bigramPosList[id2].size())
		{
			//update seg
			kk=bigramPosList[id2][jj]/maxSize;

			vobseg.clear();
			mvc.clear();mvccc.clear();
			mm=0;
			doit=false;
			while(mm<vobseglist[kk].size())
			{
				if(mm+1<vobseglist[kk].size() && unistr==vobseglist[kk][mm]+vobseglist[kk][mm+1] &&
				//	mm==bigramPosList[id2][jj]%maxSize &&
					bigramPosList[id2][jj]/maxSize==kk)
				{
					vobseg.push_back(unistr);
					mvc.push_back(vobseg.size()-1);
					mvccc.push_back(mm);
					mm+=2;
					jj++;
					doit=true;
				}
				else
				{
					vobseg.push_back(vobseglist[kk][mm]);
					mm++;
				}
			}	
			
			//update lex weights
			for(mm=0;mm<mvc.size();mm++)
			{
				//update related bigram frq
				if(mvccc[mm]>0)
				{
					ccc=vobseglist[kk][mvccc[mm]-1]+vobseglist[kk][mvccc[mm]];
					id3=bigram.isIn(ccc);
					bigram.typeCounter[id3]--;
					//update av
					if(mvccc[mm]>1) ccstr=vobseglist[kk][mvccc[mm]-2];
					else ccstr="__PREV";
					deleteMicroStringBag(bigramAV1[id3],ccstr);
					ccstr=vobseglist[kk][mvccc[mm]+1];
					deleteMicroStringBag(bigramAV2[id3],ccstr);
				}
				if(mvccc[mm]+2<vobseglist[kk].size())
				{
					ccc=vobseglist[kk][mvccc[mm]+1]+vobseglist[kk][mvccc[mm]+2];
					id3=bigram.isIn(ccc);
					bigram.typeCounter[id3]--;
					//update av
					ccstr=vobseglist[kk][mvccc[mm]];
					deleteMicroStringBag(bigramAV1[id3],ccstr);
					if(mvccc[mm]+3<vobseglist[kk].size()) ccstr=vobseglist[kk][mvccc[mm]+3];
					else ccstr="__NEXT";
					deleteMicroStringBag(bigramAV2[id3],ccstr);
				}
			    //update related bigram av

				//add new bigram
				if(mvc[mm]>0)
				{
					if(unsupType%10==2) 
						updateDLG(unigramDLG,bigram,vobseg[mvc[mm]-1],vobseg[mvc[mm]],unigramDLGScore,bigramDLGScore);
					bistr=vobseg[mvc[mm]-1]+vobseg[mvc[mm]];
					bigramUpdate(bistr,kk,mvc[mm]-1,maxSize,vocabcount,bigram,vobseg,bigramAV1,bigramAV2,bigramPosList,bigramWeight,unsupType);
					if(mvc[mm]>1)
				    {
					    bistr=vobseg[mvc[mm]-2]+vobseg[mvc[mm]-1];
					    bigramUpdate(bistr,kk,mvc[mm]-2,maxSize,vocabcount,bigram,vobseg,bigramAV1,bigramAV2,bigramPosList,bigramWeight,unsupType);
				    }
				}
				if(mvc[mm]+1<vobseg.size())
				{
					if(unsupType%10==2||unsupType%10==4) 
						updateDLG(unigramDLG,bigram,vobseg[mvc[mm]],vobseg[mvc[mm]+1],unigramDLGScore,bigramDLGScore);
					bistr=vobseg[mvc[mm]]+vobseg[mvc[mm]+1];
					bigramUpdate(bistr,kk,mvc[mm],maxSize,vocabcount,bigram,vobseg,bigramAV1,bigramAV2,bigramPosList,bigramWeight,unsupType);
					if(mvc[mm]+2<vobseg.size())
				    {
					    bistr=vobseg[mvc[mm]+1]+vobseg[mvc[mm]+2];
					    bigramUpdate(bistr,kk,mvc[mm]+1,maxSize,vocabcount,bigram,vobseg,bigramAV1,bigramAV2,bigramPosList,bigramWeight,unsupType);
				    }
				}
			}
			
			if(!doit) jj++;
			else vobseglist[kk]=vobseg;
		}

		//max
		id2=maxBPE(bigram,bigramAV1,bigramAV2,bigramWeight,bigramDLGScore,bigramPPMI,unsupType);
		if(id2<0)
			break;
	}
	fclose(fp);

	int count=0;
	std::map<std::string, size_t> mapNMTback;
	fp=fopen(out1,"wb");
	fwrite(buf,3,1,fp);//utf8 head
	if(!(out1[0]=='N'||out1[0]=='M'))
	{
	for(size_t i=0;i<vobseglist.size();i++)
	{
		for(size_t j=0;j+1<vobseglist[i].size();j++)
		    fprintf(fp,"%s@@ ",vobseglist[i][j].c_str());
		fprintf(fp,"%s\n",vobseglist[i][vobseglist[i].size()-1].c_str());
	}
	}
	else//to write segmentation back org file
	{
		for(ii=0;ii<vocab.size();ii++)
			mapNMTback[vocab[ii]]=ii;
		readTemplate(in1,vobseg);
		for(ii=0;ii<vobseg.size();ii++)
		{
			if(ii==0) vobseg[ii]=vobseg[ii].substr(3,vobseg[ii].size()-3);//remove utf8 bom head
			readLine2VectorString(vobseg[ii],vsh);
			for(jj=0;jj<vsh.size();jj++)
			{
				std::map<std::string, size_t>::iterator it1=mapNMTback.find(vsh[jj]);
				if(it1==mapNMTback.end())
				{
					count++;
					fprintf(fp,"%s",vsh[jj].c_str());
				}
				else
				{
					kk=it1->second;
					for(mm=0;mm<vobseglist[kk].size();mm++)
					{
						fprintf(fp,"%s",vobseglist[kk][mm].c_str());
						if(mm+1<vobseglist[kk].size())
						{
							if(out1[0]=='N') fprintf(fp,"@@");
							fprintf(fp," ");
						}
					}
				}
				if(jj+1<vsh.size()) fprintf(fp," ");
				else fprintf(fp,"\n");
			}
		}
		fprintf(stderr,"%d words not in substring dict\n",count);
	}
	fclose(fp);
	return;
}

void BPESeg(char *in1, char *in2, char *out1)
{
	//input in1: input plain text to be segmented
	//      in2: substring dict generated by BPETrain()
	std::vector<std::vector<std::string> > vsegall;
	std::vector<std::string> vobseg,vocab,vocabtobe,vsh,vclist,vobseg1;
	size_t ii,jj,maxSize,kk,mm,ngram,minSize;
	CMicroStringBag bigram,unigramDLG;
	std::vector<double> vocabcount,vocabcountobe;
	std::vector<int> sortpos,vccountlist;
	std::string ccstr;
	unsigned char buf[3] ={0xef,0xbb,0xbf};

	//extract word list from lexcon
	maxSize = readVocab(in2,vocab,vocabcount);
	std::map<std::string, size_t> mapNMTback;
	for(ii=0;ii<vocab.size();ii++)
		mapNMTback[vocab[ii]]=ii;

	//seg each vocab tobe
	extractWordCountList(in1,vocabtobe,vocabcountobe);
	vsegall.clear();
	for(jj=0;jj<vocabtobe.size();jj++)
	{
		if(jj==0)//vobseg.size()==4 && vobseg[0]=="i" && vobseg[1]=="r" && vobseg[2]=="o" && vobseg[3]=="n")
						{
							int pp=1;
							pp++;
						}
		line2hanziSeqUTF8(vocabtobe[jj],vsh);
		minSize=maxSize;
		if(minSize>vocabtobe[jj].size()) minSize=vocabtobe[jj].size();

		vclist.clear();
		vccountlist.clear();
		for(ngram=2;ngram<=minSize;ngram++)
		{
			for(ii=0;ii+ngram-1<vsh.size();ii++)
			{
				mergestring(vsh,ii,ngram,ccstr);
				std::map<std::string, size_t>::iterator it1=mapNMTback.find(ccstr);
				if(it1!=mapNMTback.end())
				{
					kk=it1->second;
					vclist.push_back(vocab[kk]);
					vccountlist.push_back((int)vocabcount[kk]);
				}
			}
		}
		if(vclist.size()<1)
			vsegall.push_back(vsh);
		else
		{
			sortFromKey(vccountlist,sortpos);		
			vobseg.assign(vsh.begin(), vsh.end());
			
			for(mm=0;mm<sortpos.size();mm++)
			{		
				for(ngram=2;ngram<minSize;ngram++)
				{
					vobseg1.clear();
					ii=0;
					while( ii+ngram-1<vobseg.size() )
					{
						
						mergestring(vobseg,ii,ngram,ccstr);
						if(ccstr==vclist[sortpos[sortpos.size()-1-mm]])
						{
							vobseg1.push_back(ccstr);
							ii+=ngram;
						}
						else
						{
							vobseg1.push_back(vobseg[ii]);
							ii++;
						}
					}
					while( ii < vobseg.size() )
					{
							vobseg1.push_back(vobseg[ii]);
							ii++;
					}
					vobseg.assign(vobseg1.begin(), vobseg1.end());
				}
			}
			vsegall.push_back(vobseg);
		}
	}

	mapNMTback.clear();
	for(ii=0;ii<vocabtobe.size();ii++)
		mapNMTback[vocabtobe[ii]]=ii;

	FILE *fp;

	int count=0;
	
	fp=fopen(out1,"wb");
	fwrite(buf,3,1,fp);//utf8 head
	
	readTemplate(in1,vobseg);
	for(ii=0;ii<vobseg.size();ii++)
	{
		if(ii==0) vobseg[ii]=vobseg[ii].substr(3,vobseg[ii].size()-3);//remove utf8 bom head
		readLine2VectorString(vobseg[ii],vsh);
		for(jj=0;jj<vsh.size();jj++)
		{
			std::map<std::string, size_t>::iterator it1=mapNMTback.find(vsh[jj]);
			if(it1==mapNMTback.end())
			{
					count++;
					fprintf(fp,"%s",vsh[jj].c_str());
			}
			else
			{
					kk=it1->second;
					for(mm=0;mm<vsegall[kk].size();mm++)
					{
						fprintf(fp,"%s",vsegall[kk][mm].c_str());
						if(mm+1<vsegall[kk].size())
						{
							if(out1[0]=='N') fprintf(fp,"@@");
							fprintf(fp," ");
						}
					}
			}
			if(jj+1<vsh.size()) fprintf(fp," ");
			else fprintf(fp,"\n");
		}
	}
	fprintf(stderr,"%d words not in substring dict\n",count);
	fclose(fp);
	return;
}

int main(int argc, char* argv[])
{
	fprintf(stderr,"%s",vers);
	time_t starttime,endtime,elapsetime;
	time(&starttime);

	clock_t start,end;
	start=clock();

	if(argc<2)
		return 1;//printUsage();
	else
	{
		if(argv[1][0]=='-')//
		{		
			if(strcmp(argv[1],"-bpe")==0  && argc>=3)
		    {
				BPETrain( argv[2],  atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
		    }
			if(strcmp(argv[1],"-bpeseg")==0  && argc>=3)
		    {
				BPESeg( argv[2], argv[3], argv[4] );
		    }
		}
	}

	end = clock();
	unsigned long TotalTime = (unsigned long)((double)(end-start)*1000/CLK_TCK) ;

	time(&endtime);
	elapsetime=endtime-starttime;

	fprintf(stderr,"%dms %ds\n",TotalTime,elapsetime);

	return 1;
}

