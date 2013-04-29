// written by FeLUX
#include <math.h>
#include <stdlib.h>

class Vektor{
	public:
		double x,y,z,B;
		
		 Vektor(){x = y = z = B = 0;}
		 
		 Vektor(double a,double b,double c){
		 	x = a; y = b; z = c; 
		 	Betrag();
		 }
		 
		 void Betrag(){ 
		 	B = sqrt(x*x + y*y + z*z);
		 }
		Vektor operator+(const Vektor& b){
			return Vektor(x+b.x, y+b.y, z+b.z);
		}
		Vektor operator-(const Vektor& b){
			return Vektor(x - b.x, y - b.y, z - b.z);
		}
		
		Vektor operator*(const double& b){
			return Vektor(x*b, y*b, z*b);
		}
		Vektor operator/(const double& b){
			return Vektor(x/b, y/b, z/b);
		}
		
		/*bool operator==(const Vektor& b){
			return (x == b.x && y == b.y && z == b.z);
		}*/
		bool operator==(const Vektor& b){
		//	return (abs((int)x - (int)b.x) < 1  && abs((int)(y) - (int)b.y) < 1 && abs((int)(z) - (int)b.z) < 1 );
			return (abs(x - b.x) < 1  && abs(y - b.y) < 1 && abs(z - b.z) < 1 );
		}
/*		Vektor operator%(const Vektor& b){
			return Vektor(x/b.x, y/b.y, z/b.z);
		}
*/
};



class VektorA{
	public:
		double x,y,z;
				VektorA(){x = y = z = 0;}
		 
		VektorA(double a,double b,double c){
			x = a; y = b; z = c;
		}
		 
		long double Betrag(){ 
			return sqrt(x*x + y*y + z*z);
		}
		
		VektorA operator+(const VektorA& b){
			return VektorA(x+b.x, y+b.y, z+b.z);
		}
		VektorA operator+(const Vektor& b){
			return VektorA(x+b.x, y+b.y, z+b.z);
		}
		
		VektorA operator-(const VektorA& b){
			return VektorA(x - b.x, y - b.y, z - b.z);
		}
		VektorA operator-(const Vektor& b){
			return VektorA(x - b.x, y - b.y, z - b.z);
		}
		
		VektorA operator*(const double& b){
			return VektorA(x*b, y*b, z*b);
		}
		
		VektorA operator/(const double& b){
			return VektorA(x/b, y/b, z/b);
		}
		
		/*bool operator==(const Vektor& b){
			return (x == b.x && y == b.y && z == b.z);
		}*/
		bool operator==(const VektorA& b){
		//	return (abs((int)x - (int)b.x) < 1  && abs((int)(y) - (int)b.y) < 1 && abs((int)(z) - (int)b.z) < 1 );
			return (abs(x - b.x) < 1  && abs(y - b.y) < 1 && abs(z - b.z) < 1 );
		}
/*		Vektor operator%(const Vektor& b){
			return Vektor(x/b.x, y/b.y, z/b.z);
		}
*/
};
