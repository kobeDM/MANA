#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
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
  double sum_threshold=200;
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
				 100, 100, 800, 800);
  TPad *pad0 = new TPad("pad0", "pad0", 0., 0.5, 0.5, 1);
  TPad *pad01 = new TPad("pad01", "pad01", 0, 0, 0.5, 0.5);
  TPad *pad1 = new TPad("pad1", "pad1", .5, .00, 1, .25);
  TPad *pad2 = new TPad("pad2", "pad2", .5, .25, 1, .50);
  TPad *pad3 = new TPad("pad3", "pad3", .5, .50, 1, .75);
  TPad *pad4 = new TPad("pad4", "pad4", .5, .75, 1, 1.0);
  PadSetup(pad0);
  PadSetup(pad01);
  PadSetup(pad1);
  PadSetup(pad2);
  PadSetup(pad3);
  PadSetup(pad4);

  //  TCanvas *ViewWinCut = new TCanvas("ViewWin (cut)", "Event Monitor ()",

				 
  TCanvas *SpecWin = new TCanvas("SpecWin", "Spectrum",
				 300, 300, 1000, 600);
  TPad *pads0 = new TPad("pads0", "pads0", 0, 0.75, 0.25, 1);
  TPad *pads1 = new TPad("pads1", "pads1", 0.25, 0.75, 0.5, 1);
  TPad *pads2 = new TPad("pads2", "pads2", 0.5, 0.75, 0.75, 1);
  TPad *pads3 = new TPad("pads3", "pads3", 0.75, 0.75,1, 1);
  TPad *pads4 = new TPad("pads4", "pads4", 0, 0.5, 0.25, 0.75);
  TPad *pads5 = new TPad("pads5", "pads5", 0.25, 0.5, 0.5, 0.75);
  TPad *pads6 = new TPad("pads6", "pads6", 0.5, 0.5, 0.75, 0.75);
  TPad *pads7 = new TPad("pads7", "pads7", 0.75, 0.5, 1, 0.75);
  TPad *pads8 = new TPad("pads8", "pads8", 0., 0., .5, 0.5);

  PadSetup(pads0);
  PadSetup(pads3);
  PadSetup(pads1);
  PadSetup(pads2);
  PadSetup(pads4);
  PadSetup(pads5);
  PadSetup(pads6);
  PadSetup(pads7);
  PadSetup(pads8);
  pads4->SetLogy(1);
  pads5->SetLogy(1);
  pads6->SetLogy(1);
  pads7->SetLogy(1);
  pads8->SetLogy(1);
  
  TLatex *te = new TLatex(15, 900, "Event ID: ");
  TLatex *tc = new TLatex(15, 800, "Clock : ");
  te->SetTextSize(.06);
  tc->SetTextSize(.06);
  char ch_th[128];
  sprintf(ch_th,"threshold=%.0lf",sum_threshold);
  TLatex *t_th = new TLatex(15, 900,ch_th);

  
  std::string imagetitle="Hit Image ("+filename+")";
  std::string spectrumtitle="specsrum ("+filename+")";
    //  TH2F *image = new TH2F("image", "Hit Image",
  TH2F *image = new TH2F("image", imagetitle.c_str(),
			 128, -0.5, 127.5, 1050, -0.5, 1049.5);
  TH2F *image_cut = new TH2F("image (selected)", "image (slelected)",
			 128, -0.5, 127.5, 1050, -0.5, 1049.5);
  TH2F *fadc0 = new TH2F("fadc0", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);
  TH2F *fadc1 = new TH2F("fadc1", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);
  TH2F *fadc2 = new TH2F("fadc2", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);
  TH2F *fadc3 = new TH2F("fadc3", "", 
			 1024, -0.5, 1023.5, 1050, -0.5, 1049.5);
  TH1F *fadc_spec_bl[4];
  TH1F *fadc_spec_max[4];
  TH1F *fadc_spec_min[4];
  TH1F *fadc_spec_ph[4];
  int fadc_spec_range[2];
  fadc_spec_range[0]=450;
  fadc_spec_range[1]=550;
  int fadc_spec_bin=50;
  for(int i=0;i<4;i++){
    //    std::string title="fadc baseline";
    char title[32];//="fadc baseline";
    sprintf(title,"ch%d",i);
  //+std::to_string(i);
    fadc_spec_bl[i]=new TH1F("fadc BL", title, fadc_spec_bin, fadc_spec_range[0], fadc_spec_range[1]);
    fadc_spec_max[i]=new TH1F("fadc min", "fadc min", fadc_spec_bin, fadc_spec_range[0], fadc_spec_range[1]);
    fadc_spec_min[i]=new TH1F("fadc max", "fadc max", fadc_spec_bin, fadc_spec_range[0], fadc_spec_range[1]);
    fadc_spec_ph[i]=new TH1F("fadc ph", title, fadc_spec_bin, 0, 200);
    fadc_spec_ph[i]->SetLineColor(2);
    fadc_spec_max[i]->SetLineColor(2);
    fadc_spec_min[i]->SetLineColor(4);
  }	 

  TH1F *fadc_spec_sumph=new TH1F("fadc ph (sum)","fadc ph (sum)", fadc_spec_bin, 0, 500);
    fadc_spec_sumph->SetLineColor(2);

  
  std::ifstream data_file(filename.c_str());
  short event_data=0;
  char data,data2;
  double fadc_bl[4];
  double fadc_max[4];
  double fadc_min[4];
  int view=1;
  int sampling=500;
  double textsize_spec=0.08;
  int select=0;
  double sumph=0;
  while(data_file){
    data_file.read(&data, 1);
    if(data_file.eof())
      break;    
    if((data & 0xff) == 0xeb){//new event
      data_file.read(&data, 1);
      //      if((data & 0xff) == 0x90){
      data_file.read(&data, 1);
      data_file.read(&data, 1); // header read (4 bytes)
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
	//	std::cout << "Trig: "<<EvtCount
	if(event_data%sampling==0){
	std::cout <<"event ID:"<<event_data<<"\t";
	std::cout<< "\tT=" << std::fixed<<std::setprecision(1)<<double(ClkCount)/10 << "ms\tsig ID: " << ClkCount2 << std::endl<<  std::flush;
	}
	bool end_flag=0;
	short FADC[4][1024];
	for(int i=0; i<4; i++){
	  fadc_bl[i]=0;
	  fadc_min[i]=1500;
	  fadc_max[i]=0;

	  data_file.read(&data, 1);
	  if((data & 0xf0) != (0x40 + (i << 4))){//FADC ch check (upper 4bit)
	    //	     std::cout<<"FADC incomplete data"<< std::endl;
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
	  for(int i=0;i<1023;i++){
	    for(int j=0;j<4;j++){
	      if(FADC[j][i]>fadc_max[j])fadc_max[j]=FADC[j][i];
	      if(FADC[j][i]<fadc_min[j])fadc_min[j]=FADC[j][i];
	      fadc_bl[j]+=(double)FADC[j][i]/1024;
	    }
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
	  sumph=0;
	  for(int i=0;i<4;i++){
	    //	    std::cout<<"ch"<<i<<" "<<fadc_bl[i]<<" "<<fadc_max[i]<<" "<<fadc_min[i]<<std::endl;
	    fadc_spec_bl[i]->Fill(fadc_bl[i]);
	    fadc_spec_ph[i]->Fill(fadc_max[i]-fadc_bl[i]);
	    fadc_spec_min[i]->Fill(fadc_min[i]);
	    fadc_spec_max[i]->Fill(fadc_max[i]);
	    sumph+=fadc_max[i]-fadc_bl[i];
	    }
	  fadc_spec_sumph->Fill(sumph);
	  if(sumph>sum_threshold)	  select=1;
	  else select=0;
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
		    if(select)image_cut->Fill(32*(3-id)+strip+8*(3-cc), drift);
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
	tc->SetText(15, 800, text);
	
	if(view&&event_data%sampling==0){
	  TLatex *tmin = new TLatex(460, (2.5*fadc_spec_bl[0]->GetEntries()/5.), "min");
	  TLatex *tmax = new TLatex(460, (3.5*fadc_spec_bl[0]->GetEntries()/5.), "max");
	  TLatex *tbl = new TLatex(460, (3*fadc_spec_bl[0]->GetEntries()/5.), "average");
	  tmin->SetTextSize(textsize_spec);
	  tmax->SetTextSize(textsize_spec);
	  tbl->SetTextSize(textsize_spec);
	  tmin->SetTextColor(4);
	  tmax->SetTextColor(2);
	SpecWin->cd();
	pads0->cd();
	fadc_spec_bl[0]->Draw();
	fadc_spec_min[0]->Draw("same");
	fadc_spec_max[0]->Draw("same");
	tmin->Draw();
	tmax->Draw();
	tbl->Draw();
	pads1->cd();
	fadc_spec_bl[1]->Draw();
	fadc_spec_min[1]->Draw("same");
	fadc_spec_max[1]->Draw("same");
	tmin->Draw();
	tmax->Draw();
	tbl->Draw();
	pads2->cd();
	fadc_spec_bl[2]->Draw();
	fadc_spec_min[2]->Draw("same");
	fadc_spec_max[2]->Draw("same");
	tmin->Draw();
	tmax->Draw();
	tbl->Draw();
	pads3->cd();
	fadc_spec_bl[3]->Draw();
	fadc_spec_min[3]->Draw("same");
	fadc_spec_max[3]->Draw("same");
	tmin->Draw();
	tmax->Draw();
	tbl->Draw();
	TLatex *tph = new TLatex(100, (fadc_spec_ph[0]->GetEntries()/5.), "pulse height");
	tph->SetTextSize(textsize_spec);
	tph->SetTextColor(2);
	pads4->cd();
	fadc_spec_ph[0]->Draw();
	tph->Draw();
	pads5->cd();
	fadc_spec_ph[1]->Draw();
	tph->Draw();
	pads6->cd();
	fadc_spec_ph[2]->Draw();
	tph->Draw();
	pads7->cd();
	fadc_spec_ph[3]->Draw();
	tph->Draw();
	pads8->cd();
	fadc_spec_sumph->Draw();
	tph->Draw();
	
	SpecWin->Update();
	ViewWin->cd();
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
	pad01->cd();
	image_cut->Draw("col");
	t_th->Draw();
	ViewWin->cd();
	ViewWin->Update();
	}
	//cout << data_file.tellg() << endl;

	//gSystem->Sleep(1000);
	//}
      //      gSystem->Sleep(0);
      event_data++;//counter}
    
    }//event end
  }
  
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
	return 0;
}

