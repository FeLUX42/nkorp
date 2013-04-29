// written by FeLUX
#include <math.h>


class mapu{
	public:
		long double m;
		Vektor r, rneu;
		Vektor v, valt;
		Vektor a;
		 int name;
		mapu () { m = 0; name = -1; r = rneu = v = valt = a = Vektor();}
		mapu(long double am, int bn, Vektor cr,Vektor dv){
			m = am; //masse
			name = bn; // Name
			r = rneu = cr; // Position
			v = dv; // Geschwindigkeit
		} 
		void update(void){
			r = rneu;
			valt = v; 
		}
		void update_a(Vektor aneu){
			a = aneu;
		}
};


