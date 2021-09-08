#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
//#include <string>
#include <vector>
#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TH2F.h>
#include <TLatex.h>
#include "TApplication.h"


void PadSetup(TPad *pad){
  pad->SetGridx();
  pad->SetGridy();
  pad->Draw();
}

//void View(string filename){
//void View(int aa){
//void View(int argc, char **argv){
int main(int argc, char **argv){
  gROOT->Reset();
  std::string filename;
  //  string filename="FILENAME";
  if(argc<2) filename="GBIP_016_0000.raw";
  else filename=argv[1];
  //  cout <<"filename: "<<filename<<endl;
  //  cout <<"filename: "<<aa<<endl;
  //  string filename="ahio";
  gStyle->SetOptStat(0);
  //Application
  TApplication app("app",&argc,argv);

  int integ=1;
  TCanvas *ViewWin = new TCanvas("ViewWin", "Event Monitor",
				 100, 100, 1000, 600);
  TPad *pad0 = new TPad("pad0", "pad0", 0, 0, 0.75, 1);
  TPad *pad1 = new TPad("pad1", "pad1", .75, .00, 1, .25);
  TPad *pad2 = new TPad("pad2", "pad2", .75, .25, 1, .50);
  TPad *pad3 = new TPad("pad3", "pad3", .75, .50, 1, .75);
  TPad *pad4 = new TPad("pad4", "pad4", .75, .75, 1, 1.0);
  PadSetup(pad0);
  PadSetup(pad1);
  PadSetup(pad2);
  PadSetup(pad3);
  PadSetup(pad4);

  TLatex *te = new TLatex(15, 900, "Event ID: ");
  TLatex *tc = new TLatex(15, 850, "Clock : ");
  te->SetTextSize(.04);
  tc->SetTextSize(.04);
  std::string imagetitle="Hit Image ("+filename+")";
    //  TH2F *image = new TH2F("image", "Hit Image",
    TH2F *image = new TH2F("image", imagetitle.c_str(),
			 128, -0.5, 127.5, 1050, -0.5, 1049.5);
  TH2F *fadc0 = new TH2F("fadc0", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);
  TH2F *fadc1 = new TH2F("fadc1", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);
  TH2F *fadc2 = new TH2F("fadc2", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);
  TH2F *fadc3 = new TH2F("fadc3", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);

  std::ifstream data_file(filename.c_str());
  short event_data=0;
  char data,data2;
  while(data_file){
    data_file.read(&data, 1);
    if(data_file.eof())
      break;    
    if((data & 0xff) == 0xeb){//new event
      data_file.read(&data, 1);
      //      if((data & 0xff) == 0x90){
      data_file.read(&data, 1);
      data_file.read(&data, 1); // header read (4 bytes)
      std::cout <<"event number:"<<event_data<<"\t";
      if(!integ){
      image->Reset("ICES");
      fadc0->Reset("ICES");
      fadc1->Reset("ICES");
      fadc2->Reset("ICES");
      fadc3->Reset("ICES");
      }
	int EvtCount=0;
	for(int i=0; i<4; i++){
	  data_file.read(&data, 1);
	  EvtCount = ((EvtCount & 0xffffff) << 8) + (data & 0xff); //Evt count (4byte)
	}

	int ClkCount=0;
	for(int i=0; i<4; i++){
	  data_file.read(&data, 1);
	  ClkCount = ((ClkCount & 0xffffff) << 8) + (data & 0xff);
	}

	int ClkCount2=0;
	for(int i=0; i<4; i++){
	  data_file.read(&data, 1);
	  ClkCount2 = ((ClkCount2 & 0xffffff) << 8) + (data & 0xff);
	}
	 std::cout << "Trig: "<<EvtCount << "\tClock: " << ClkCount << "\tClock2: " << ClkCount2 << std::endl<<  std::flush;

	bool end_flag=0;
	short FADC[4][1024];
	for(int i=0; i<4; i++){
	  data_file.read(&data, 1);
	  if((data & 0xf0) != (0x40 + (i << 4))){//FADC ch check (upper 4bit)
	     std::cout<<"FADC incomplete data"<< std::endl;
	    //    cout << "version(Y)"<<dec << (unsigned)(data & 0xff);
	    //data_file.read(&data, 1);
	    // cout << " "<<(unsigned)(data & 0xff);
	    //data_file.read(&data, 1);
	    //cout << " version(M)"<<dec << (unsigned)(data & 0xff);
	    //data_file.read(&data, 1);
	    //cout << " "<<(unsigned)(data & 0xff)<<endl;
	    end_flag = 1;
	    //	    break;
	  }
	  FADC[i][0] = ((data & 0x03) << 8);
	  data_file.read(&data, 1);
	  FADC[i][0] += (data & 0xff);//first FADC points
	}
	if(!end_flag){
	  //complete FADC data
	  //	  for(int i=0; i<511; i++){
	  for(int i=0; i<1023; i++){ //another 1023 points
	    data_file.read(&data, 1);
	    FADC[0][i+1] = (data & 0x03) << 8;
	    data_file.read(&data, 1);
	    FADC[0][i+1] += (data & 0xff);
	    fadc0->Fill(i, FADC[0][i]);	    
	    data_file.read(&data, 1);
	    FADC[1][i+1] = (data & 0x03) << 8;
	    data_file.read(&data, 1);
	    FADC[1][i+1] += (data & 0xff);
	    fadc1->Fill(i, FADC[1][i]);	    
	    data_file.read(&data, 1);
	    FADC[2][i+1] = (data & 0x03) << 8;
	    data_file.read(&data, 1);
	    FADC[2][i+1] += (data & 0xff);
	    fadc2->Fill(i, FADC[2][i]);	    
	    data_file.read(&data, 1);
	    FADC[3][i+1] = (data & 0x03) << 8;
	    data_file.read(&data, 1);
	    FADC[3][i+1] += (data & 0xff);
	    fadc3->Fill(i, FADC[3][i]);
	  }
	
	  //complete FADC to here

	  //version check
	  /**	  data_file.read(&data, 1);
	  cout << "version(Y)"<<dec << (unsigned)(data & 0xff);
	  data_file.read(&data, 1);
	  cout << (unsigned)(data & 0xff);
	  data_file.read(&data, 1);
	  cout << " version(M)"<<dec << (unsigned)(data & 0xff);
	  data_file.read(&data, 1);
	  cout << (unsigned)(data & 0xff);
**/
	  //end_flag = 1;
	   
	//0x1018-
	  
	  data_file.read(&data, 1);
	  //cout << " version(sub)"<<dec << (unsigned)((data & 0xff)>>4)<<endl;
	  data_file.read(&data, 1);
	  unsigned depth=(unsigned)((data & 0x07));
	  data_file.read(&data, 1);
	  depth=(depth<<8)+(unsigned)(data&0xff);
	  data_file.read(&data, 1);
	  depth=(depth<<8)+(unsigned)(data&0xff);
	  //cout << "version="<<dec << depth<<endl;
	  //	  cout << "version=0x"<<hex << depth<<endl;
	  
	  //cout<<dec<<data;
	  for(;;){
	    data_file.read(&data, 1);
	    if((data & 0xff) != 0x80)
	      break;
	    
	    data_file.read(&data, 1);
	    data_file.read(&data, 1);
	    unsigned drift = ((data & 0x7) << 8);
	    data_file.read(&data, 1);
	    drift += (data & 0xff);
	    
	    for(int id=0; id<4; id++){
	      //if((hit_flag >> (3-id)) & 0x1){
	      for(int cc=0; cc<4; cc++){
		data_file.read(&data, 1);
		for(int strip=0; strip<8; strip++)
		  if((data >> strip) & 0x1){
		    image->Fill(32*(3-id)+strip+8*(3-cc), drift);
		  }
	      }
	    }
	  }
	}//end of imcomplete FADC data

	data_file.read(&data, 1);
	unsigned flag=(unsigned)(data&0xff);
	//	while(flag != 0x75504943){//footer check
	while(flag != 0x50){//footer check
	  data_file.read(&data, 1);
	  flag=data;
	}
	while(flag != 0x49){//footer check
	  data_file.read(&data, 1);
	  flag=data;
	}
	while(flag != 0x43){//footer check
	  data_file.read(&data, 1);
	  flag=data;
	}
	//event end
	//}
    
	char text[200];
	//	sprintf(text, "Event ID : %d", EvtCount);
	sprintf(text, "Event ID : %d", event_data);
	te->SetText(15, 900, text);
	sprintf(text, "Clock : %f", (double)ClkCount/10000.);
	tc->SetText(15, 850, text);

	pad4->cd();
	fadc0->Draw("col");
	pad3->cd();
	fadc1->Draw("col");
	pad2->cd();
	fadc2->Draw("col");
	pad1->cd();
	fadc3->Draw("col");
	pad0->cd();
	image->Draw("col");
	tc->Draw();
	te->Draw();

	ViewWin->cd();
	ViewWin->Update();
	//cout << data_file.tellg() << endl;

	//gSystem->Sleep(1000);
	//}
      //      gSystem->Sleep(0);
      event_data++;//counter}
    
    }//event end
  }
	ViewWin->cd();
	ViewWin->Update();
	return 0;
}

