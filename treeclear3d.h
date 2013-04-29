/*
 * Copyright (C) 2013 Felix Passenberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Autor:
 * 		Felix Passenberg <fc.passenberg@gmail.com>
*/

#include <math.h>
#include <iostream>


class tree{
	public:
		tree * child[2][2][2]; // aufgeteilt in 4 zellen die 1en stern beinhalten 
		mapu * stern[2][2][2]; // darauf achten entweder 1 stern oder child zellen zu haben.
		bool voll; // wenn Ast leer dann true wenn voll dan flase
		bool tiefer;
		Vektor rm; // masseschwerpunkt
		Vektor r; // Mittelpunkt
		long double width; // breite der kante
		long double m;
		int name;

		tree(Vektor pos, long double breite){ // specifications of this tree / leaf			
			this->child[0][0][0] = NULL;
			this->child[0][0][1] = NULL;
			this->child[0][1][0] = NULL;
			this->child[0][1][1] = NULL;
			this->child[1][0][0] = NULL;
			this->child[1][0][1] = NULL;
			this->child[1][1][0] = NULL;
			this->child[1][1][1] = NULL;
			
			//child[0] = new tree(Vektor(r.x+width/2, r.y+width/2, 0), width/2);
			//child[1] = new tree(Vektor(r.x-width/2, r.y+width/2, 0), width/2);
			//child[2] = new tree(Vektor(r.x-width/2, r.y-width/2, 0), width/2);
			//child[3] = new tree(Vektor(r.x+width/2, r.y-width/2, 0), width/2);
			//child = new tree[4];
			
			this->stern[0][0][0] = NULL;
			this->stern[0][0][1] = NULL;
			this->stern[0][1][0] = NULL;
			this->stern[0][1][1] = NULL;			
			this->stern[1][0][0] = NULL;
			this->stern[1][0][1] = NULL;
			this->stern[1][1][0] = NULL;
			this->stern[1][1][1] = NULL;
			voll = false;
			tiefer = false;
			m = 0;
			r = pos;
			rm = Vektor();
			width = breite; 
			
			//calcpos();
		}
		tree(int name_g, Vektor pos, long double breite ){ // specifications of this tree / leaf
			
			name = name_g;
			
			this->child[0][0][0] = NULL;
			this->child[0][0][1] = NULL;
			this->child[0][1][0] = NULL;
			this->child[0][1][1] = NULL;
			this->child[1][0][0] = NULL;
			this->child[1][0][1] = NULL;
			this->child[1][1][0] = NULL;
			this->child[1][1][1] = NULL;
			
			//child[0] = new tree(Vektor(r.x+width/2, r.y+width/2, 0), width/2);
			//child[1] = new tree(Vektor(r.x-width/2, r.y+width/2, 0), width/2);
			//child[2] = new tree(Vektor(r.x-width/2, r.y-width/2, 0), width/2);
			//child[3] = new tree(Vektor(r.x+width/2, r.y-width/2, 0), width/2);
			
			//child = new tree[4];
			this->stern[0][0][0] = NULL;
			this->stern[0][0][1] = NULL;
			this->stern[0][1][0] = NULL;
			this->stern[0][1][1] = NULL;			
			this->stern[1][0][0] = NULL;
			this->stern[1][0][1] = NULL;
			this->stern[1][1][0] = NULL;
			this->stern[1][1][1] = NULL;

			voll = false;
			tiefer = false;
			m = 0;
			r = pos;
			rm = Vektor();
			width = breite; 
			//std::cout << "test4" << std::endl;
			
			//calcpos();
		}
		/*void addchild(int pos){
			child[pos] = new tree();
			child[pos] = &children;
			calcpos();
		}*/
		void addstar(mapu* star){
			int posx, posy, posz = 0; // 0: 1.quadrant, 1: 2.Quadrant ...
			posx = posy = posz;
			
			if(star->r.x >= this->r.x){ // x
				posx = 1;
			}else{
				posx = 0;
			}

			if(star->r.y >= this->r.y){ // y
				posy = 1;
			}else{
				posy = 0;
			}
			
			if(star->r.z >= this->r.z){ // z
				posz = 1;
			}else{
				posz = 0;
			}
			
			if( (this->child[posx][posy][posz]) || (this->stern[posx][posy][posz]) ){
				if(this->child[posx][posy][posz]){ 		// wenn child vorhanden  
					this->child[posx][posy][posz]->addstar(star);// dann runter gehen
				} else { // wenn kein child
					if((this->stern[posx][posy][posz]) && (this->stern[posx][posy][posz] != star)){ // wenn stern vorhanden
						
						long double rx = 0, ry = 0, rz = 0;
						
						if(posx){
							rx = this->r.x + (width/2.);
						}else{
							rx = this->r.x - (width/2.);
						}
						
						if(posy){
							ry = this->r.y + (width/2.);
						}else{
							ry = this->r.y - (width/2.);
						}
						
						if(posz){
							rz = this->r.z + (width/2.);
						}else{
							rz = this->r.z - (width/2.);
						}
						this->child[posx][posy][posz] = (new tree(Vektor(rx,ry,rz), width/2.)); 
						
						
						//child[pos] = (new tree(Vektor(r.x+width/2, r.y+width/2, 0), width/2)); 	// sonst neu erstellen und
						//std::cout << "neu" << std::endl;
						/*if(this->width < 1e17){
							std::cout << stern[pos] << " <- stern | star -> " << star << std::endl;
							std::cout << stern[pos]->r.x << " <- x stern y -> " << stern[pos]->r.y << std::endl;
							std::cout << star->r.x << " <- x star  y -> " << star->r.y << std::endl;
							std::cout << this->r.x << " <- x feld y -> " << this->r.y <<  " width -> " << width << "\n";
							sleep(1);
						}*/
						//else
						{
						mapu * tmp = stern[posx][posy][posz];
						
						addstar(star);
						this->stern[posx][posy][posz] = NULL;
						addstar(tmp);
						tmp = NULL;
						
						// runter gehen
						//std::cout << "runter" << std::endl;
						
						//std::cout << "ende" << std::endl;
						
						}
					}
				}
			}else{ // wenn auch kein stern -> kanz leer
				this->stern[posx][posy][posz] = star;
			}
		}
		
		
		void calcpos(){
	
			Vektor rSchwer = Vektor();
			long double masse  = 0;
			for(int x = 1; x >= 0; --x){
				for(int y = 1; y >= 0; --y){
					for(int z = 1; z >= 0; --z){
						if(child[x][y][z] != 0){
							this->child[x][y][z]->calcpos();
						}
					}
				}
			}
			for(int x = 1; x >= 0; --x){
				for(int y = 1; y >= 0; --y){
					for(int z = 1; z >= 0; --z){//--------------------------------------------- schwerpunkt
						if(child[x][y][z]){
							//rSchwer = rSchwer + ((r-child[x][y][z]->rm) * child[x][y][z]->m); //position * masse (schwere sterne beeinflussen den schwerpunkt
							rSchwer = rSchwer + (child[x][y][z]->rm * child[x][y][z]->m);
							masse +=  this->child[x][y][z]->m;
						}else{
							if(stern[x][y][z]){
								//rSchwer = rSchwer + ((r-stern[x][y][z]->r) * stern[x][y][z]->m);
								rSchwer = rSchwer + (stern[x][y][z]->r * stern[x][y][z]->m);
								masse += this->stern[x][y][z]->m;
							}
						}
					}
				}
			}
			
			m = masse;
			if(m!=0){
				rm = rSchwer / m; // massemittelpunkt
			}
			//std::cout << "masse\n";
		}
		
		void loeschen(void){
			//std::cout << "löschen\n";
			
			for(int x = 1; x >= 0; --x){
				for(int y = 1; y >= 0; --y){
					for(int z = 1; z >= 0; --z){
						if(child[x][y][z] != NULL){
							this->child[x][y][z]->loeschen();
						}
						delete this->child[x][y][z];
						this->stern[x][y][z] = NULL; 
						this->child[x][y][z] = NULL;
						
					}
				}
			}
			
			//delete [] child;			
			
			voll = false, tiefer = false;
			width = 0;
			m = 0;
			r = Vektor();
			rm = Vektor();
			
		}
		
		void clear(void){
			m= 0;
			r = Vektor();
			rm = Vektor();
			width = 0;
			for(int x = 1; x >= 0; --x){
				for(int y = 1; y >= 0; --y){
					for(int z = 1; z >= 0; --z){
						if(child[x][y][z] != 0){
							this->child[x][y][z]->clear();
							this->stern[x][y][z] = NULL; 
						}
					}
				}
			}
			voll = false, tiefer = false;
		}

};

