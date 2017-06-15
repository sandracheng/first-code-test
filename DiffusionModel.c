/*
SNO+ New Diffusion Model 

Written by Sandra Cheng (10184319) <14wlsc@queensu.ca> 


*/


#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

// ROOT
#include "TCanvas.h"
#include "TH1.h"


/*
imagine the spherical AV; outside we have a layer of water, and  inside we have LAB. Separating the two is an acrylic layer.
we can ignore the effect of gravity on the diffusion; top and bottom can be modelled as the same
this model is 1D. The code is for a particle-wide section of sphere. We drip many particles onto.....
the same spot (continuous dripping), and see the final positions of all of them.
the sphere has symmetry about it, so we can use this code for one particle-wide section to extrapolate for 
the rest of the sphere 

 
 
a particle-wide section of sphere

  <--- particle width---->
    _ _ _ _ _ _ _ _ _ _ __
   /                     /|                        |
  /       H20           /x|                  H20   |    H20
 /_____________________/xx|              ----------|------- x = 0  
 |xxxxxxxxxxxxxxxxxxxxx|xx| acrylic                |
 |xxxxxxxxxxxxxxxxxxxxx|xx|                        | 
 |xxxxxxxxxxxxxxxxxxxxx|xx|            acrylic     |           acrylic
 |xxxxxxxxxxxxxxxxxxxxx|x/                         | x-axis
 |xxxxxxxxxxxxxxxxxxxxx|/                          |
 ----------------------                           \|/  
          LAB                             ----------------- x = thickness of acrylic, t

                                                  LAB
                     

*/

int DiffusionModel() {  
                         
  TCanvas *c1 = new TCanvas;
  Int_t nBins = 600;  
  Double_t lowlim = -300;  
  Double_t uplim = 300;
                             
  TH1I *h1 = new TH1I("Legend","Title", nBins,lowlim,uplim); 
  h1->SetFillColor(kGreen);            
               
 
  const int totParticle = 9999;    // total number of particles that drip down
  int finalDist[totParticle];        // An array to collect the final displacement of each particle

                                        
  /* Need random number r, generated between 0 and 1, to get probability of the walker moving forward or backward.  
  For now, this model uses prob=0.5 of moving forward and backward, but these numbers should actually change as the..... 
  particle moves further into the acrylic
  At the beginning, a water molecule is more likely to move forward than backward.
  As x approaches the end of the acrylic, it is equally likely for  molecule to move forward or backward.
  */
  srand(time(NULL));
  int pCount;
                           
   for (int pCount = 0; pCount<totParticle; pCount++) { //loop through total number of particles,Yan..
    int dist=0;                         //counter for the displacement of a single particle. We reset to 0 for each particle.
    for (int step = 0; step < 1000; step++){  //loop thru steps. Generate random number 0-100 to determine how the particle moves 
	                                      //at each step                                                                    
       int r = rand()% 100;              // if r less than  50, walker moves forward 1 space, otherwise it moves back 1 space
         //printf("same  %d ", r);
         if (r < 50){
          dist= dist+1;   
          //printf("small %d\n",r);
         }
	 else {
          dist= dist-1;    
          //printf("big %d\n",r);
         }
         //printf("%d\n", finalDist[totParticle])
    }  // the step loop end curly bracket 
    finalDist[pCount] = dist;
    h1->Fill(dist);
    //printf("counter:%d; final dist:%d\n", pCount, finalDist[pCount]);
    // sort out the data in a histogram here, since the above comment works. Do it with root commands.
               h1->Draw("hist");  
         }  // the total particle loop end curly bracket
  h1->Fit("gaus");                   
  Double_t mean =  gaus->GetParameter(1);
  Double_t sigma =  gaus->GetParameter(2);
  Double_t FWHM = 2*sqrt(2*log(2))*sigma;
  cout << FWHM << endl;
  h1->Draw();                              
}   // the DiffusionModel() end curly bracket

                             
