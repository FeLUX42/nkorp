// written by FeLUX
#include "./Vektor.h"
#include "./mapu.h"
#include "./treeclear3d.h"

#include <math.h>
#include <iostream>
#include <fstream> 
#include <sstream>
#include <string.h>
#include <time.h> 
#include <stdlib.h>
#include <omp.h>
//#define __FAST_MATH__

#define BarnesHut // verwende den Baum
//#define Npow2 // jeden einzeln berechnen

#define maxthreads 4

//#define HERZ
//#define HerzRand
#ifdef HerzRand
	#ifndef HERZ
		#define HERZ
	#endif
#endif

#define ver 1.6



#define MODUS 1 // 0 sonnensystem, 1 Galaxie

//#define menge 100 // nicht mehr benötigt


using namespace std;

static long double g = 6.673e-11;

static double AE = 1.49598e11 ;
static double TAG = 60*60*24;
unsigned int menge = 2;
long double theta = 0.7;

mapu * korp = NULL; 



long double step = 0;			// Simulations relevate Parameter
unsigned long long displim = 0;
unsigned long long stepmax = 0;

unsigned long display = 0;
unsigned int periods = 9999; // perioden anzahl
unsigned int period = 0;
unsigned long long n = 0;
unsigned long long temp_n = 0;


int readData(long int f_periods, long int f_n, bool read_ver){
	char filename[100] = {0};
	sprintf(filename, "./data/results%08li-%08li",f_periods, f_n);
	ifstream input;
	//input.open(filename);
	//cout << "offne " << filename << " lesen beginne" << endl;
	string disgard = "";
	double rev;
	input.open("./version/ver.txt");
	if(!input.is_open()){
		cout << "\nEs konnte keine Versionsdatei gefunden werden. Stellen Sie sicher, dass eine Versionsdatei in ./version/ liegt\n" << endl;
		return 3;
	}
	input >> disgard >> rev;
	if((disgard == "version:" && rev < ver) || disgard != "version:" ){
		cout << "\nDie informationen können möglicherweise nicht verstanden werden!\nversuhce zu lesen\n" << "gefundene Version: " << rev << " verwende Version: " << ver <<endl;
		if(disgard != "version:"){
			cout << "Ich bin mir nicht sicher ob das eine richtige versionsdatei ist\n";
		} 
		cout << "fortsetzen? dennoch versuchen? (j,n)" << endl;
		char inpt;
		cin.get(inpt);
		if(!( (inpt == 'j') || (inpt == 'J') || (inpt == 'y') || (inpt == 'Y')) ){
			return 2;
		}		
	}
	
	unsigned int meng = 0;
	input >> disgard >> meng;
	menge = meng;
	korp = new mapu[menge];
	for(unsigned int i = 0; i< menge; i++){
		korp[i] = mapu(1,i, Vektor(0,0,0),Vektor(0,0,0));
	}
	if(meng != menge){
		cout << "gefundene Menge an Sternen ist ungleich der eingestellten bitte anpassen oder das array variabel gestalten." << endl;
	} 
	
	if(read_ver){
		input >> disgard >> periods;
		cout << disgard << " " << periods << endl;
		input >> disgard >> period;
		cout << disgard << " " << period << endl;
		input >> disgard >> temp_n;
		cout << disgard << " " << temp_n << endl;
		input >> disgard >> stepmax;
		cout << disgard << " " << stepmax << endl;
		input >> disgard >> step;
		cout << disgard << " " << step << endl;
		input >> disgard >> displim;
		cout << disgard << " " << displim << endl;
		input >> disgard >> theta;
		cout << disgard << " " << theta << endl;
		sprintf(filename, "./data/results%08u-%08llu",period, temp_n);
	}

	input.close();
	
	cout << "offne " << filename << " lesen beginne" << endl;
	input.open(filename);
	unsigned int i = 0;
	//long double tmp = 0;
	long double rx,ry,rz,vx,vy,vz, ma = 0;
	rx = ry = rz = vx = vy = vz = ma;
	if(input.is_open()){
		
		do{
			input >> rx >> ry >> rz >> vx >> vy >> vz >> ma;// = atof(data);
			korp[i] = mapu(ma, i, Vektor(rx,ry,rz), Vektor(vx,vy,vz));
			
			//cout << korp[i].r.x << endl;
			
			i++;	
						
		}while((input.good() )&& (i < menge )   );
		cout << "\nlesen von " << i << " Sternen erfolgreich"<<endl;
		menge = i-1;
		cout << korp[8].v.B << endl;
		cout << korp[8].r.B << endl;
		display = 0;
		
	}else{cout << "datei konnte nicht gelesen werden." << endl; 
		return 1;
	}
	input.close();
	return 0;
}

void writeData(char* filename, long int f_n){
	ofstream output;
	//output.open(filename);
	output.open("./version/ver.txt");
	output << "version: " <<  ver << endl;
	output << "menge: " << menge << endl;
	output << "periods:	" << periods << endl;
	output << "periode: " << period << endl;
	output << "N: " << f_n << endl;
	output << "stepmax: " << stepmax << endl;
	output << "step: " << step << endl;
 	output << "displim: " << displim << endl;
 	output << "theta: " << theta << endl;
	
	output.close();
	output.open(filename);
	for(unsigned int i = 0; i < menge; i++){
		if(korp[i].m != 0){
			output << korp[i].r.x << "\t" << korp[i].r.y << "\t"  << korp[i].r.z << "\t\t" << korp[i].v.x << "\t" << korp[i].v.y << "\t" << korp[i].v.z << "\t\t" << korp[i].m << "\n";
    	}
    }
    output << "\n";
    output.close();	
    
	output.open("plotme.dat", ofstream::app);
	for(unsigned int i = 0; i < menge; i++){
			output << korp[i].r.x << "\t" << korp[i].r.y << "\t"  << korp[i].r.z << "\t";
    	
    }
    output << "\n";
    output.close();	
}

void writePlot(char* plotfile){
	ofstream output;
	output.open(plotfile, ofstream::app);
	
	/*
	for(int i = 0; i < menge; i++){
		if(korp[i].m){
			output << korp[i].r.x*cos(arc) + korp[i].r.y*sin(arc) << "\t" << korp[i].r.x*(-sin(arc)) +  korp[i].r.y*cos(arc) << "\t" << korp[i].r.z   << "\t";
    	}
    }*/
	
	for(unsigned int i = 0; i < menge; i++){
		if(korp[i].m){
			output << korp[i].r.x << "\t" << korp[i].r.y << "\t"  << korp[i].r.z << "\t";
    	}
    }
    output << "\n";
    output.close();	
}


void Baumout(tree * baum, string name){
	ofstream output;
	output.open("baum.txt", ios::app);
//	output << "\t\t" << name << "\n";
	
	
	for(int x = 1; x >= 0; --x)
		for(int y = 1; y >= 0; --y)
			for(int z = 1; z >= 0; --z){
				if(baum->child[x][y][z] != 0){
					stringstream sstr;
					//int tmp = i;
					sstr << name << x << y << z;
					string out;
					sstr >> out;
					Baumout((baum->child[x][y][z]), out );
				}
	}
	for(int x = 1; x >= 0; --x)
		for(int y = 1; y >= 0; --y)
			for(int z = 1; z >= 0; --z){
				if(baum->stern[x][y][z] != 0){
					stringstream sstr;
					//int tmp = i;
					sstr << name << x << y << z ;
					string out;
					sstr >> out;
					output << "" << out << "\t" << baum->stern[x][y][z]->r.x << "\t" << baum->stern[x][y][z]->r.y << "\t\t" << baum->stern[x][y][z]->m << "\n"; //;<< "\t\t" << korp[i].v.x << "\t" << korp[i].v.y << "\t" << korp[i].v.z << "\n";
				}
			
	}
   	//output << "\n\n";
   	output.close();		
}

void Energie(void){
	
	long double Epot = 0;
	long double Ekin = 0;
	long double Eges = 0;
	for(unsigned int i = 0; i < menge; i++){
		if(korp[i].m != 0){
			Ekin += 1/2. * korp[i].m * korp[i].v.B * korp[i].v.B;
			for(unsigned int j = 0; j < menge; j++){
				if( (i != j) && (korp[j].m != 0)){
					Vektor dir = (korp[i].r - korp[j].r);
					Epot += korp[i].m * g * korp[i].m / dir.B; 
				}
			}
		}
		
	}
	Eges = Ekin + Epot;
	
	ofstream output;
	output.open("Energie.txt", ios::app);
	output << Ekin << " \t<- kin | pot -> "  << Epot << " \t |ges -> " << Eges<< "\n";
	output.close();	
	
}

long double deltaE(mapu kor, unsigned int j, bool out){
	Vektor Epot = Vektor();
	long double Ekin = 0;
	long double Eges = 0;
	long double Edif = 0;
	Ekin = 1/2. * kor.m * kor.v.B * kor.v.B;
	for(unsigned int i = 0; i < menge; i++){
				if( (i != j) && (korp[i].m != 0)){
					Vektor dir = (korp[i].r - kor.r);
					Epot = Epot + (dir/dir.B) * korp[i].m * g * kor.m / dir.B; 
				}
	}
	Eges = Ekin + Epot.B;
	Edif = Ekin - Epot.B;
	if(out){
		cout << Ekin << " \t<- kin | pot -> "  << Epot.B << " \t " << Edif << " \t<- dif (k-p) | ges -> " << Eges<< "\n";
	}
	return (Epot.B/Ekin);
}

double myrandom(){
	double dist = 0;
	do { 
		dist = ((double) rand() / (double)RAND_MAX);
	}while (fabs(dist) <= 0.02); //(dist < 0.01 && dist > -0.01);
	return dist;
}

bool herzU(double a, double x, double y){
	#ifndef HERZ
		 return true;
	#endif
	bool res = false;
	if(abs(x) <= a*sqrt(2)){
		res = (y > (-sqrt(2)*(sqrt(-2*pow(fabs(x),2)+pow(a,2./3)*pow(2,1./3)*pow(fabs(x),4./3)+2*pow(a,2))-pow(a,1./3)*pow(2,1./6)*pow(fabs(x),2./3) ) / 2.)    );
	}else return false;
	//cout << (-sqrt(2)*(sqrt(-2*pow(x,2)+pow(a,2/3)*pow(2,1/3)*pow(x,4/3)+2*pow(a,2))-pow(a,1/3)*pow(2,1/6)*pow(x,2/3) ) / 2)  << "\t"; 
	return res;
}

bool herzO(double a, double x, double y){
	#ifndef HERZ
		 return true;
	#endif
	bool res = false;
	if(abs(x) <= a*sqrt(2)){
		res = (y < (sqrt(2)*(sqrt(-2*pow(fabs(x),2)+pow(a,2./3)*pow(2,1./3)*pow(fabs(x),4./3)+2*pow(a,2))+pow(a,1./3)*pow(2,1./6)*pow(fabs(x),2./3) ) / 2.)   );
	}else return false;
	
	return res;
}

long double herzUy(double a, double x){
	long double y = 0;
	if(abs(x) <= a*sqrt(2)){
		y = ((-sqrt(2)*(sqrt(-2*pow(fabs(x),2)+pow(a,(2./3))*pow(2,(1./3))*pow(fabs(x),(4./3))+2*pow(a,2))-pow(a,1./3)*pow(2,(1./6))*pow(fabs(x),(2./3)) ) )/ 2. )   ;
	}
	return y;
}

long double herzOy(double a, double x){
	long double y = 0; 
	if(abs((int)x) <= a*sqrt(2)){
		y = (sqrt(2)*(sqrt(-2*pow(fabs(x) ,2)+pow(a,2./3)*pow(2,1./3)*pow(fabs(x) ,4./3)+2*pow(a,2))+pow(a,1./3)*pow(2,1./6)*pow(fabs(x) ,2./3) ) / 2.)   ;
	} 
	return y;
}

long double maxRB(){
	long double maxrb = 0;
	for(unsigned int i = 0; i <  menge; i++){
		if(korp[i].r.B > maxrb) maxrb = korp[i].r.B;
	}
	return maxrb * 1.1;
}

#ifdef BarnesHut 
Vektor calca(tree * baum, mapu objekt){
	//cout << "calc a\n";
	Vektor baumdir = Vektor();
	//cout << baum->rm.B << "\n";
	baumdir = ( baum->rm - objekt.r );
	//cout << baumdir.B << "\n";
	VektorA a = VektorA();
	if(baumdir.B != 0){
		if(baum->width / baumdir.B <= theta){
			//a = ( baumdir * (( baum->m  * g) / (baumdir.B*baumdir.B*baumdir.B)) );
			return ( baumdir * (( baum->m  * g) / (baumdir.B*baumdir.B*baumdir.B)) );
		}else{
			for(int x = 1; x >= 0; --x)
				for(int y = 1; y >= 0; --y)
					for(int z = 1; z >= 0; --z){
						if(baum->child[x][y][z] != 0){
							a = a + calca((baum->child[x][y][z]), objekt);
					//		a = a + (((dir / dir.B) / pow(dir.B,2) ) *  korp[j].m  * g);
						}else if( (baum->stern[x][y][z] != 0) && (baum->stern[x][y][z]->name != objekt.name) ){
							Vektor sterndir = Vektor();
							sterndir = ( baum->stern[x][y][z]->r - objekt.r ); 
							a = a + ( sterndir * ( (baum->stern[x][y][z]->m  * g) / (sterndir.B*sterndir.B*sterndir.B) ) );
						}
					}
				//if(baum->star!= 0){		
		}
	}
	return Vektor(a.x,a.y,a.z);
}

Vektor calcag(tree * baum, mapu objekt){
	//cout << "calc a\n";
	Vektor baumdir = Vektor();
	//cout << baum->rm.B << "\n";
	baumdir = ( baum->rm - objekt.r );
	//cout << baumdir.B << "\n";
	VektorA a = VektorA();
	if(baumdir.B != 0){
		if(baum->width / baumdir.B <= theta){
			//a = ( baumdir * (( baum->m  * g) / (baumdir.B*baumdir.B*baumdir.B)) );
			return ( baumdir * (( baum->m) / (baumdir.B*baumdir.B*baumdir.B)) );
		}else{
			for(int x = 1; x >= 0; --x)
				for(int y = 1; y >= 0; --y)
					for(int z = 1; z >= 0; --z){
						if(baum->child[x][y][z] != 0){
							a = a + calcag((baum->child[x][y][z]), objekt);
					//		a = a + (((dir / dir.B) / pow(dir.B,2) ) *  korp[j].m  * g);
						}else if( (baum->stern[x][y][z] != 0) && (baum->stern[x][y][z]->name != objekt.name) ){
							Vektor sterndir = Vektor();
							sterndir = ( baum->stern[x][y][z]->r - objekt.r ); 
							a = a + ( sterndir * ( (baum->stern[x][y][z]->m ) / (sterndir.B*sterndir.B*sterndir.B) ) );
						}
					}
				//if(baum->star!= 0){		
		}
	}
	return Vektor(a.x, a.y, a.z);
}
#endif

#ifdef Npow2
Vektor calca(tree * baum, mapu objekt){

	Vektor a = Vektor();
	
	//a = (((baumdir / baumdir.B) / pow(baumdir.B,2) ) *  baum->m  * -g);
	for(unsigned int j = 0; j < menge; j++){	
		Vektor dir = (korp[j].r - objekt.r);
		if(dir.B != 0){
			a = a + (((dir / dir.B) / pow(dir.B,2.) ) *  korp[j].m  * g);
		}
	}
	return a;
}
#endif

double masse_einzeln(){
	double mass = 0;
	for(unsigned int j = 0; j < menge; j++){	
		mass += korp[j].m;
	}
	return mass;
}

int main(int argc, char* argv[]){
	
	cout << "Version: " << ver << endl;
	int startT = time(NULL);	 
	int Time = startT;
	// int seed = startT;
	int seed = 1312858090;
	
	step = 0;			// Simulations relevate Parameter
	displim = 0;
	stepmax = 0;
	long startN = 0;
	long zeit = 0;
	short modus  = MODUS;
	bool calcV = true;	
	bool newStars = true;
	
	switch(modus){							// initialisiere startparameter
		case 0: step = 1 ; 
			displim = 60*60*6*4  / step  ; //6*4 stunden * 7 tage -> wöchentlich ein punkt ausgeben
			zeit = 24 *  365 * 0.24 ;  // simulationszeit // in Stunden
			stepmax = (zeit * 60 * 60) / step;
			//stepmax = 365 * 24 ;
			calcV = false;
			break;
		case 1:step = 60*60*24*30*365 * 1e1;	// in sekunden
			displim = 5 * 1e2; // alle 
			stepmax = 10000;
			break;
	}
	
	display = displim-1;
	periods = 9999; // perioden anzahl
	period = 0;
		
	//int maxdist = 5 * 1e4;			//generierungs relevante Parameter
	//int distance = maxdist - 1;
	//double kritdist  = 1e12;		//
	

	
	long int nummer= 0;
	int erg = 0;
	
	if (argc){ // --------------------------wennn daten dann hier einlesen
		switch (argc){
			//1: cout << "\n\t ich habe nicht genug information\n"; break;
			case 2:	 // neeue Simulation
				menge = atoi(argv[1]);
				if (menge < 1) menge = 2;
				//korp = new mapu[menge];
				cout << "\n\t Ich habe die Anzahl der Sterne auf '" << menge << "' gesetzt.\n";
				break;
			case 3:  // alte simulation fortsetzen
				calcV = false;
				newStars = false;
				menge = 1;
				period = atof(argv[1]);
				nummer = atof(argv[2]);
				cout << "\n\t";
				
				if(period == 0 && nummer == 0){
					cout << "Lese die letzte notierte Datei ein\n";
					erg = readData(period, nummer,true);
					startN = temp_n+1;
				}else{
					erg = readData(period, nummer, false);
					startN = nummer+1;// + displim;
				}
				
				if(erg != 0){
					cout << "\n\nfehler \"" << erg << "\" beim lesen der Datei, Beende nun\n" << endl;
					return 1;
				}
				cout << "Ich habe Periode auf '" << period << "' und das N '" << startN-1 << "' gesetzt.\n";
				break;
				
			default:
				cout << "\nbitte eine der folgende Argumente verwenden:\n";
				cout << 	"\t1: 1. Menge der Sterne als int.\n";
				cout << 	"\t2: 1. Periode  \t2. Nummer n\n" << endl;
				cout << "ich bekam: " << endl;
				for(int i = 0; i < argc; i++){
					cout << argv[i] << endl;
				} 
				return 1;
				break;
		}
	}
	

	Vektor rSchwer = Vektor();
	srand(seed);
	
	double size = 4.0e20;///sqrt(2);
	double herzA = size/2.;//2.0e20;
	
	
	if(newStars){
		korp = new mapu[menge];
		cout << "neue Sterne\n";
		cout << menge << "\n";
	//		cout << herzU(12,0,-1.1) << endl;
	
		for(unsigned int i = 0; i < menge; i++){
			cout << i << "\t" << flush;
			double mindist = size;
			double solldist =  1e18;
			do{
				mindist = size;
				double dist = pow((myrandom() * size) ,1);
				double height = (myrandom()* myrandom() * 1e20)/exp(1/(size/dist)); ; // *pow(myrandom(), 3.)
				if(rand() % 2 == 1) height = -height;
				double arc = myrandom()*360;
				long double mass =  (3.6e41 / menge );
				korp[i] = mapu(mass,i, Vektor(sin(arc)*dist, cos(arc)*dist ,height),Vektor(0,0,0));
				for(unsigned int j = 0; j < i; j++){
					Vektor dir = (korp[j].r - korp[i].r);
					if(dir.B < mindist){
						mindist = dir.B;
					}
				}
			}while((mindist < solldist)  || !herzO(herzA, korp[i].r.x,korp[i].r.y) || !(herzU(herzA, korp[i].r.x, korp[i].r.y)) );
		
		}
		
		korp[0] = mapu((3.6e41 / menge ) * 100, 1, Vektor(1,1,0), Vektor(0,0,0)); // masse reicher stern in der mitte

	#ifdef HerzRand
		double maxy = 0;
		for(int i = 0; i < menge; i++){
			if(korp[i].r.x >= herzA*1.414){
			//	cout << i << "\t";
				korp[i].r.x = herzA *1.414;
			}else if(korp[i].r.x <= -herzA*1.414){
				korp[i].r.x = -herzA *1.414;
			}
		
			if (rand() % 2){
				korp[i].r.y = herzOy(herzA,korp[i].r.x);
			}else{
				korp[i].r.y = herzUy(herzA,korp[i].r.x);
			}
			if(maxy < korp[i].r.y)maxy = korp[i].r.y;
		}
		cout << endl << endl << "mxy "<< maxy << endl;
	#endif
		

		if(modus == 0){	
			korp[0] = mapu(1.9891e30, 0, Vektor(-3.765071894182533e-03 * AE,-5.903395910592171e-04 * AE, 9.162016647291394e-06 * AE), Vektor(2.962253037163623e-06 * AE / TAG, -5.513660987929432e-06 * AE / TAG, -5.708754274963416e-08 * AE  / TAG));
				//SONNE
		
			korp[1] = mapu(3.302e23, 1, Vektor(1.920233705280145E-01 * AE, -3.925520948245388E-01 * AE, -4.998096686503108E-02 * AE), Vektor(1.954402219573275E-02 * AE / TAG, 1.397105216123319E-02 * AE / TAG, -6.511045382746823E-04 * AE / TAG));
				// Merkur
			korp[2] = mapu(4.8685e24, 2, Vektor(-4.741168289028241E-01 * AE, 5.414065885326997E-01 * AE, 3.458018423820671E-02 * AE), Vektor(-1.534390554136505E-02 * AE / TAG, -1.337041989216014E-02 * AE / TAG, 7.025530088798218E-04 * AE / TAG));
				// Venusversion:
			korp[3] = mapu(5.976e24, 3, Vektor(7.247858314895868E-01 * AE, -7.057806693739082E-01 * AE, 2.733527135095581E-05 * AE), Vektor(1.168144526660584E-02 * AE / TAG, 1.229289468350605E-02 * AE / TAG, -1.071605853694793E-06 * AE / TAG));
				// Erde
			korp[4] = mapu(6.4185e23, 4, Vektor(5.937014324167870E-01 * AE, 1.388159529398866E+00 * AE, 1.443738741981011E-02 * AE), Vektor(-1.232041683536983E-02 * AE / TAG, 6.715849653205144E-03 * AE / TAG, 4.433739284721219E-04 * AE / TAG));
				// Mars
			korp[5] = mapu(1.89813e27, 5, Vektor(4.381635711879301E+00 * AE, 2.302436131331345E+00 * AE, -1.076909128602934E-01 * AE), Vektor(-3.600371058579933E-03 * AE / TAG, 7.040081604391193E-03 * AE / TAG, 5.130652142427629E-05 * AE / TAG));
				// Jupiter
			korp[6] = mapu(5.68319e26, 6, Vektor(-9.179594916081662E+00 * AE, -2.960164071283253E+00 * AE, 4.167800586946884E-01 * AE), Vektor(1.412333709613232E-03 * AE / TAG, -5.322226623409173E-03 * AE / TAG, 3.655671405732734E-05 * AE / TAG));
				// Saturn
			korp[7] = mapu(8.68103e25, 7, Vektor(2.006580484840957E+01 * AE, 6.621095864188307E-01 * AE, -2.575039644445662E-01 * AE),Vektor(-1.584112951047463E-04 * AE / TAG, 3.747572371036042E-03 * AE / TAG, 1.603702440758723E-05 * AE / TAG));
				// Uranus
			korp[8] = mapu(1.0241e26, 8, Vektor(2.578764055529525E+01 * AE, -1.533517327924133E+01 * AE, -2.784958826973319E-01 * AE), Vektor(1.583232809678799E-03 * AE / TAG, 2.716364460599358E-03 * AE / TAG, -9.240843188161279E-05 * AE / TAG));
				// Neptun
			korp[9] = mapu(1.314e22, 9, Vektor(3.488080862557200E+00 * AE, -3.179448446102866E+01 * AE, 2.393223642477471E+00 * AE), Vektor(3.168839694191235E-03 * AE / TAG , -2.820526174322764E-04 * AE / TAG, -8.843589480555806E-04 * AE / TAG));
				//Pluto
		}
	}
	
	cout << "\nberechne Schwerpunkt\n";	
	
	for(unsigned int i = 0; i < menge; i++){   //--------------------------------------------- schwerpunkt
		rSchwer = rSchwer + (korp[i].r * korp[i].m);
	}
	double masse  = 0;
	for(unsigned int i = 0; i < menge; i++){
		masse +=  korp[i].m;
	}
	rSchwer = rSchwer / masse;
	for(unsigned int i = 0; i < menge; i++){
		korp[i].r = korp[i].r - rSchwer;
		//korp[i].v = a/a.B*sqrt(korp[i].r.B * a.B) * 2;
		//korp[i].v = Vektor(korp[i].v.y,-korp[i].v.x,korp[i].v.z);
		korp[i].update();
	}
	
	
	
	
	
	size = maxRB();
	tree baumroot =  tree(1,Vektor(),size );
	#ifdef BarnesHut 
	
	cout << "erstelle Baum\n";		
	
	for(unsigned int i = 0; i < menge; i++){ //------------------------------------------------------------------------- berechne den Baum
		//addstar2(korp[i],&baumroot);
		baumroot.addstar(&korp[i]);
	}
	cout << "berechne Baum\n";
	baumroot.calcpos();
	Baumout(&baumroot, "+");
	
	#endif
	
	
	
	if(calcV){
		cout << "berechne start v\n";	
		for(unsigned int i = 0; i < menge; i++){ //------------------------------------------ start v
			Vektor a = Vektor();
				for(unsigned int j = 0; j < menge; j++){	
					if( i != j){
					
						Vektor dir = (korp[j].r - korp[i].r);
						Vektor rMass = (korp[i].r * korp[i].m - korp[j].r * korp[j].m); 
						dir = rMass;
						if(dir.B != 0){
							// a = (((dir / dir.B) / pow(dir.B,2) ) *  korp[j].m  * g );
							//a = (dir / dir.B) *(g)*korp[j].m / (dir.B * dir.B);
							//a = calca(&baumroot, korp[i]);
							//korp[i].v = korp[i].v + a/a.B*sqrt(a.B * dir.B); // /sqrt(menge);
						
							//korp[i].v = korp[i].v + (dir/dir.B) * sqrt( (g*korp[i].m) / pow(dir.B,1) ) ;
							//a = a + (((dir / dir.B) / pow(dir.B,2) ) *  korp[j].m  * g);
							//korp[i].v = korp[i].v + (dir / dir.B) * sqrt(dir.B * korp[j].m * g);
							//korp[i].v.y = korp[i].v.y + (sqrt((korp[j].m * g) / dir.y));
						}
					}
				}
			
			a = calca(&baumroot,korp[i]); 
			//korp[i].v = 
			//korp[i].v = a/a.B*sqrt(korp[i].r.B * a.B ) ;
			
			
			
			long double masseinnen = 0;
			for(unsigned int j = 0; j < menge; ++j){
				if(i != j && korp[i].r.B > 1e16){
					if(korp[j].r.B < korp[i].r.B){
						masseinnen += korp[j].m;
						//cout << korp[j].m <<"\t";
					}else{
						Vektor dir = (korp[j].r - korp[i].r);
						if(dir.B > 1e16){
					//		korp[i].v = korp[i].v + dir/dir.B * sqrt(g*korp[i].m/ pow(dir.B,1.));
						}
					}
				}
			}
			
			if(korp[i].r.B != 0){
				korp[i].v = (korp[i].r / korp[i].r.B) * sqrt(g*masseinnen/korp[i].r.B) * (-1);
				//korp[i].v = korp[i].v + (korp[i].r / korp[i].r.B) * sqrt(g*masseinnen/korp[i].r.B) * (-1);
				//korp[i].v = korp[i].v + (a / a.B) * sqrt(1*g*masseinnen/pow(korp[i].r.B,1.));
				
				//korp[i].v = (korp[i].r / korp[i].r.B) * sqrt(g*baumroot.m/korp[i].r.B);
			}
			
			//korp[i].v = (a / a.B) * sqrt(a.B * korp[i].r.B);
			//korp[i].v = a/a.B * sqrt( a.B * korp[i].m / rSchwer.B  );
			//cout << korp[i].v.B << "\n";
			korp[i].v = Vektor(korp[i].v.y, -korp[i].v.x, korp[i].v.z);  // rotation
			while( (deltaE(korp[i],i,0) < -1e40) && (korp[i].v.B != 0)){
			//	cout << "erhöhe \t";
			//	if(deltaE(korp[i],i,1) < -1e49){
			//		korp[i].v = korp[i].v * 1.001;
			//	}else{
			//		korp[i].v = korp[i].v * 1.000001;
			//	}
			}
			if(korp[i].v.B != 0){
			//	cout << "erhöhe \n";
			//	korp[i].v = korp[i].v * sqrt(deltaE(korp[i],i,true)); 
			}
			deltaE(korp[i],i,true);
			//cout << korp[i].v.B << endl;
			//korp[i].v = Vektor();
		}
	
		/*
		for(unsigned int i = 0; i < menge; i++){ // start v
			korp[i].v = korp[i].r / korp[i].r.B  * sqrt(g*((masse * korp[i].m) / menge) / pow(korp[i].r.B, 3.26)) ;
		
			korp[i].v = Vektor(korp[i].v.y,-korp[i].v.x,korp[i].v.z);
			//korp[i].v = Vektor();
		}
		*/
	}
	

		
	cout << "masse einzeln: "<< masse_einzeln() << "\nmasse baum: " <<  baumroot.m << endl;



	cout << "initioalisiere threads" << endl; //---------------------------------------------threads einmal durchlaufen lassen
	
	cout << "maximal: " << omp_get_max_threads() << " Threads" << endl;
	unsigned int threads = 1;
	threads = omp_get_max_threads();
	if(threads > menge) threads = menge;	
	if(threads > (unsigned int)omp_get_max_threads())threads = (unsigned int)omp_get_max_threads();
	//threads = 4;
	if(threads > maxthreads) threads = 4;
	//if(threads < 1) threads = 1;
    omp_set_num_threads(threads);
    
    #pragma omp parallel for schedule(guided) 	 	
 	for(int i = 0; i < omp_get_num_threads(); ++i){
 		stringstream s;
 		s << "Hello_World_from_thread_" << omp_get_thread_num() << "_where_i_is:_" << i;
 		string out;
 		s >> out;
		#pragma omp critical 
		{		
		cout << out << endl;
		}
		if( 0 == omp_get_thread_num() )
			printf("Es wird versucht mit %d threads zu rechnen.\n", threads);
	
	}
	

		
	cout << "berechne nun\n";
	
	long double maxr = 0;
	unsigned int imaxr = 0;
	for(unsigned int i = 0; i < menge; i++){
		if (korp[i].r.B > maxr){
			maxr = korp[i].r.B;
			imaxr = i;
		}
	}
	cout << "maximaler Weg (meter):  \t" << maxr*2* M_PI << endl;
	cout << "benötigte Zeit (Simulierte sekunden): \t" << (korp[imaxr].r.B*2*M_PI) / korp[imaxr].v.B  << endl;
	cout << "Timesteps pro periode: \t"  << ((korp[imaxr].r.B*2*M_PI) / korp[imaxr].v.B) / step << endl;
	
	if(newStars){
		stepmax = ((korp[imaxr].r.B*2*M_PI) / korp[imaxr].v.B) / step;
		#if MODUS == 1
			stepmax = 5000;
		#endif
		#if MODUS == 0
			stepmax = (zeit * 60 * 60) / step; 
			cout << "stepmax is " << stepmax << "\n";
		#endif
	}
	
	ofstream seedout;
	seedout.open("seed");
	seedout << seed << "\n";
	seedout.close();

	ofstream createFile;
	createFile.open("plotme.dat");
	createFile.close();
	
	time(NULL);
	
	
	
	for (unsigned long long n = startN; n < stepmax; n++ ){
		
		if((n >= (stepmax - 1)) &&( period < periods)){
			cout << "\n" << "periode beendet: " << period << "\n"; 
			period ++;
			n = -1;
		}
		//cout << "n: " << n << "\n" << flush; //" \t "; 
		if(time(NULL) >= Time){
			Time = time(NULL)+60*1;
			cout << "n: " << n << "\n" << flush; //" \t "; 
			cout << "display : " << display << " of displim: " << displim << "\t\t" << (float) display/displim << "\n" << flush;			
		}
		//Time = time(NULL)+60*10;
		
		//cout << "geloescht\n";
		
		#ifdef BarnesHut 
			//#pragma omp master
			
			size = maxRB();
			tree baumroot =  tree(1,Vektor(),size);
			//#pragma omp parallel for ordered
			for(unsigned int ii = 0; ii < menge; ii++){ //------------------------------------------------------------ berechne den Baum
				if(korp[ii].m != 0){
				//	#pragma omp single
					{
					baumroot.addstar(&korp[ii]);
					}
				}
			}
			baumroot.calcpos();
			
		#endif
		
		display++;
		//cout << clock() << "\t";

		/*if(distance > maxdist ) {
			distance = 0;
		}
		distance++;
		*/
		
		
		
		//threads = 1; // ----------------threads vorbereiten & berechnen der anzahl
		//threads = omp_get_max_threads();
		//if(threads >= menge) threads = menge;	
		//if(threads > (unsigned int)omp_get_max_threads()) threads = (unsigned int)omp_get_max_threads();
	//threads = 4;
		//if(threads > maxthreads) threads = 4;
		//if(threads < 1) threads = 1;
	// omp_set_num_threads(threads);
		
		//#pragma omp master
		#pragma omp parallel for schedule(runtime)
		for(unsigned int i = 0 ; i < menge; i++){ // -------------------------berechnung
			/*
			stringstream s;
	 		s << "Hello_World_from_thread_" << omp_get_thread_num() << "_where_i_is:_" << i;
	 		string out;
	 		s >> out;
	 		cout << out << endl;
			//#pragma omp single
			printf("Es wird versucht mit %d threads zu rechnen.\n", omp_get_num_threads());
	 		*/
	 		
			//cout << omp_get_thread_num() << endl;
			if(korp[i].m != 0 ){// && korp[i].r.B < 6e20){
				if(korp[i].r.B > 1e22){
					korp[i].v = Vektor();
					korp[i].m = 0;
					cout << "reset " <<i << endl;
				}
				Vektor a = Vektor(); // beschleunigungsvektor = 0
						// wenn kaestchen weit weg dann nimm es sonst mach es kleiner
				
					a = calca(&baumroot, korp[i]); 	
				
				//cout << a.x << "\t" << flush;				
				
				 //kollision auskommenteiert da n^2 rechenzunahme
				#if MODUS == 1	// nur im galaxie modus kollisionen
				for(unsigned int j = i+1; j < menge; j++){   //-------------Kollision
					if((korp[j].m != 0) ){
						
						Vektor dir = (korp[j].r - korp[i].r);   // richung von korp i zu korp j
						if(dir.B < 2e17){ // kollisions detektor
							
							if( i != 0){
								korp[i].v = (korp[i].v*korp[i].m + korp[j].v*korp[j].m) / (korp[i].m+korp[j].m);
							 }
							korp[i].m += korp[j].m;
							korp[j].m = 0;
							korp[j] = mapu();
							std::cout << "habe " << j << " gelöscht, da kollision mit " << i << endl;
						}
					}
				
	
					//if( dir.B < distance * kritdist || distance >= maxdist ){
					//		a = a + (((dir / dir.B) / pow(dir.B,2) ) *  korp[j].m  * g); // beschleunigung berechnen
					//}
				}
				if( i == 0){
					a = Vektor();
				}
				#endif
				
				
				//korp[i].v = korp[i].v + (a * step); // geschwindigkeit euler // berechnen
				korp[i].v = korp[i].v + ( a * (3./2) -  korp[i].a * (1./2)) * step; //adams bashford
				korp[i].rneu = korp[i].r + (korp[i].v * (3./2) - korp[i].valt * (1./2) ) * step; // position neu berechnen
				korp[i].update_a(a);
			}
			
		
		} // ---------------------------------------------------------------------ende der Berechnung
		
		//cout << clock() << "\t";
		
		if(display >= displim){ // ausgabe nicht immer 
	//		#pragma omp parallel for schedule(runtime)
			for(unsigned int ii = 0; ii < menge; ii++){ //------------Berechnung und korrektur des schwerpunktes
					rSchwer = rSchwer + (korp[ii].r * korp[ii].m);
			}
			double masse  = baumroot.m;
	//		#pragma omp parallel for schedule(runtime)
	/*		for(unsigned int ii = 0; ii < menge; ii++){
				masse +=  korp[ii].m;
			}*/
			rSchwer = rSchwer / masse;
			#pragma omp parallel for schedule(runtime)
			for(unsigned int ii = 0; ii < menge; ii++){
					korp[ii].r = korp[ii].r - rSchwer;
			}
			
			Energie();
			//cout << "runde: " << n <<  " von " << stepmax <<"     \t";
			//for(int i = 0; i < menge; i++){
				
			//cout << korp[i].r.x << "\t" << korp[i].r.y << "\t" << korp[i].r.z << "\t"; 
		
			char filename[100] = {0};
			sprintf(filename, "./data/results%08u-%08llu",period ,n);
			writeData(filename, n); //---------------------ausgabe
			
			char plotfile[100] = {0};
			sprintf(plotfile, "./data-per/result%08u", period);
			writePlot(plotfile);	
			
						
		  	//}
		
			display = 0;
			//cout << "\n";
		}
		
		
		#pragma omp parallel for schedule(runtime)
		for(unsigned int i = 0; i < menge; i++){
			korp[i].update();
		}
		//cout << "neuer baum\n";
		// --------------------------------------Baum löschen und neu anlegen da sich die sterne bewegten
		
		#ifdef BarnesHut 
			#pragma omp master
			baumroot.loeschen();
		#endif
	}
	baumroot.loeschen();
	delete [] korp;
	korp = NULL;
	return 0;		
}
