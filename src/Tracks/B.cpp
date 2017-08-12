// #include "Decay.h"

#include <vector>
#include "../Utilities/Vector3D.h"
#include <cmath>
#include <iostream>

void PositionToGrid(const Position3D * position, int & xindex, int & yindex, int & zindex);
void GridToPosition(Position3D * position, const int & xindex, const int & yindex, const int & zindex);

// B Decay::BGen::operator()(Position3D p) const 
int main()
{
  bool verboso = true;
  using namespace std;
  
  //Campo magnetico
  vector<Vector3D>   fieldLayer2;		//identifica la z una volta note x e y
  vector<Vector3D*>  fieldLayer1;		//identifica la y una volta nota la x
  vector<Vector3D**> field;			//identificaa la x (asse)
  
  //Potenziale vettore
  vector<Vector3D>   potentialLayer2;
  vector<Vector3D*>  potentialLayer1;
  vector<Vector3D**> potential;

  //Correnti
  vector<Vector3D>   currentsLayer2;
  vector<Vector3D*>  currentsLayer1;
  vector<Vector3D**> currents;
  
  
  //impostazione delle dimensioni del sistema
  double xsyst = 100.;
  double ysyst = 100.;
  double zsyst = 100.;
  
  //impostazione delle dimensioni del solenoide (funziona solo con parallelepipedo non ruotato)
  double xsol = 10.;
  double ysol = 10.;
  double zsol = 10.;
  
  //impostazione della griglia, indica in pratica lo step tra due punti della griglia stessa
  double delta = 0.1;
  
  //impostazione della corrente, indica in pratica la corrente che viaggia sul filo usato (si considera costante)
  double current = 10.;	//mA
  
  //impostazione della lunghezza del filo
//   double lenght = 1000. * 1000; //m, non mm, per quello il fattore 1000
  
  //calcolo dei punti per il processo numerico
  int xsteps = int(xsyst/delta); 
  int ysteps = int(ysyst/delta); 
  int zsteps = int(zsyst/delta); 
  
  //creazione delle griglie di lavoro tridimentionali e loro inizializzazione
  cout << "Inizio creazione griglie..." << endl;
  fieldLayer2.resize(xsteps * ysteps * zsteps, {0,0,0});	//contiene tutto il campo
  fieldLayer1.resize(xsteps * ysteps);				//contiene puntatori a un piano orizzontale
  field.resize(xsteps);						//contiene puntatori alla posizione lungo l'asse del solenoide

  potentialLayer2.resize(xsteps * ysteps * zsteps, {0,0,0});	//contiene tutto il campo
  potentialLayer1.resize(xsteps * ysteps);			//contiene puntatori a un piano orizzontale
  potential.resize(xsteps);					//contiene puntatori alla posizione lungo l'asse del solenoide

  currentsLayer2.resize(xsteps * ysteps * zsteps, {0,0,0});	//contiene tutto il campo
  currentsLayer1.resize(xsteps * ysteps);			//contiene puntatori a un piano orizzontale
  currents.resize(xsteps);					//contiene puntatori alla posizione lungo l'asse del solenoide
  
  for(unsigned int i = 0; i < fieldLayer1.size(); i++)	//sistema i puntatori in modo che puntino al posto giusto
  {
    fieldLayer1.at(i) = & fieldLayer2.at(i * ysteps);
    potentialLayer1.at(i) = & potentialLayer2.at(i * ysteps);
    currentsLayer1.at(i) = & currentsLayer2.at(i * ysteps);
  }
  for(unsigned int i = 0; i < field.size(); i++)
  {
    field.at(i) = & fieldLayer1.at(i * xsteps);
    potential.at(i) = & potentialLayer1.at(i * xsteps);
    currents.at(i) = & currentsLayer1.at(i * xsteps);
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  double perimeter = (2 * ysol) + (2 * zsol);	//occhio a x o y 
  double current_density = current/perimeter;	//sarà la densità di corrente lungo x
  
  //inserimento delle correnti
  if (verboso) cout << "Inizio inserimento delle correnti..." << endl;
  int xindex = 0;
  int yindex = 0;
  int zindex = 0;
  Position3D solenoid_starting_point = {-xsol*.5, -ysol*.5, -zsol*.5};
  Position3D walking_point = solenoid_starting_point;
  Vector3D * modify_inloop;
  
  unsigned int debug1 = 0;
  unsigned int debug2 = 0;
  unsigned int debug3 = 0;
  
  while(walking_point.x < xsol*.5)		//ciclo sulla lunghezza del solenoide
  {
    while(walking_point.z < zsol*.5)		//salita
    {
     PositionToGrid(& walking_point, xindex, yindex, zindex);	//trova il valore nella grid della posizione
     modify_inloop = & currents[xindex][yindex][zindex];	//si fissa nel punto desiderato
     * modify_inloop = {current_density,0.,0.};			//cambia la corrente in tale punto
     zindex++;							//avanza lungo la griglia
     GridToPosition(& walking_point, xindex, yindex, zindex);	//passa da griglia a posizione
     debug1++;
     if( debug1 > 100000 && verboso)
     {
       cout << "RIGA 109 PROBLEMA CICLO!" << endl;
       return 0;
     }
    }
    debug1 =0;
    while(walking_point.y < ysol*.5)		//primo tratto orizzontale
    {
      PositionToGrid(& walking_point, xindex, yindex, zindex);
      modify_inloop = & currents[xindex][yindex][zindex];
      * modify_inloop = {current_density,0.,0.};
      yindex++;
      GridToPosition(& walking_point, xindex, yindex, zindex);
      debug1++;
      if( debug1 > 100000 && verboso)
      {
	cout << "RIGA 124 PROBLEMA CICLO!" << endl;
	return 0;
      }
    }
    debug1 = 0;
    while(walking_point.z > zsol*.5)		//discesa
    {
      PositionToGrid(& walking_point, xindex, yindex, zindex);
      modify_inloop = & currents[xindex][yindex][zindex];
      * modify_inloop = {current_density,0.,0.};
      zindex++;
      GridToPosition(& walking_point, xindex, yindex, zindex);
      debug1++;
      if( debug1 > 100000 && verboso)
      {
	cout << "RIGA 139 PROBLEMA CICLO!" << endl;
	return 0;
      }
    }
    debug1 = 0;
    while(walking_point.y > ysol*.5)		//secondo tratto orizzontale
    {
      PositionToGrid(& walking_point, xindex, yindex, zindex);
      modify_inloop = & currents[xindex][yindex][zindex];
      * modify_inloop = {current_density,0.,0.};
      yindex++;
      GridToPosition(& walking_point, xindex, yindex, zindex);
      debug1++;
      if( debug1 > 100000 && verboso)
      {
	cout << "RIGA 154 PROBLEMA CICLO!" << endl;
	return 0;
      }
    }
    xindex++;					//avanza lungo l'asse
    GridToPosition(& walking_point, xindex, yindex, zindex);
    debug2++;
    if( debug2 > 100000 && verboso)
    {
      cout << "RIGA 161 PROBLEMA CICLO!" << endl;
      return 0;
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //da ora si sfrutta la simmetria del problema e si studia solo un quarto del sistema,
  //cioè la parte che va da z = 0 a z = zsol * .5 e da y = 0 a y = ysol *.5

  
  //inizializzazione del potenziale vettore
  if (verboso) cout << "Inizio inizializzazione potenziale vettore..." << endl;
  walking_point  = {0.,0.,0.};		//inizia dal centro del sistema
  double rho = 0;
  double theta = 0;
  
  debug1 = 0;
  debug2 = 0;
  debug3 = 0;
  
  //set del potenziale vettore all'interno del solenoide
  double mu_0 = 1.;
  while(walking_point.x < xsol*.5)		//ciclo sulla lunghezza del solenoide
  {
    while(walking_point.z < zsol*.5)		//ciclo sull'altezza del solenoide
    {
      while(walking_point.y < ysol*.5)		//riempie striscia di larghezza
      {
	PositionToGrid(& walking_point, xindex, yindex, zindex);
	modify_inloop = & potential[xindex][yindex][zindex];
	rho = mu_0 * .5 * sqrt(walking_point.x * walking_point.x + walking_point.y * walking_point.y);
	theta = atan(walking_point.y/walking_point.z);		//ordine giusto? credo di sì, basta mantenere quello sopra qua come quello con seno quando riempio
	* modify_inloop = {0., rho * sin(theta), rho * cos(theta)};
	yindex++;
	GridToPosition(& walking_point, xindex, yindex, zindex);
	debug1++;
	if( debug1 > 100000 && verboso)
	{
	  cout << "RIGA 200 PROBLEMA CICLO!" << endl;
	  return 0;
	}
      }
      zindex++;					//avanza lungo l'altezza
      GridToPosition(& walking_point, xindex, yindex, zindex);
      debug2++;
      if( debug2 > 100000 && verboso)
      {
	cout << "RIGA 209 PROBLEMA CICLO!" << endl;
	return 0;
      }
    }
    xindex++;					//avanza lungo l'asse
    GridToPosition(& walking_point, xindex, yindex, zindex);
    debug1++;
    if( debug3 > 100000 && verboso)
    {
      cout << "RIGA 218 PROBLEMA CICLO!" << endl;
      return 0;
    }
  }
  
  
  debug1 = 0;
  debug2 = 0;
  debug3 = 0;
  
  //set del potenziale vettore all'esterno del solenoide !!SOPRA!!
  if (verboso) cout << "Inizio inizializzazione esterna al poteziale vettore..." << endl;
  walking_point = {0., 0, zsol * .5};	//inizia da sopra il solenoide
  while(walking_point.x < xsol*.5)		//ciclo sulla lunghezza del solenoide
  {
    while(walking_point.z < zsyst*.5)		//ciclo sull'altezza del sistema
    {
      while(walking_point.y < ysol*.5)		//riempie striscia di larghezza del solenoide
      {
	PositionToGrid(& walking_point, xindex, yindex, zindex);
	modify_inloop = & potential[xindex][yindex][zindex];
	rho = mu_0 * .5 * zsol * .5 * zsol * .5 / sqrt(walking_point.x * walking_point.x + walking_point.y * walking_point.y);
	theta = atan(walking_point.y/walking_point.z);		//ordine giusto? credo di sì, basta mantenere quello sopra qua come quello con seno quando riempio
	* modify_inloop = {0., rho * sin(theta), rho * cos(theta)};
	yindex++;
	GridToPosition(& walking_point, xindex, yindex, zindex);
	debug1++;
	if( debug1 > 100000 && verboso)
	{
	  cout << "RIGA 251 PROBLEMA CICLO!" << endl;
	  return 0;
	}
      }
      zindex++;					//avanza lungo l'altezza
      GridToPosition(& walking_point, xindex, yindex, zindex);
      debug2++;
      if( debug2 > 100000 && verboso)
      {
	cout << "RIGA 260 PROBLEMA CICLO!" << endl;
	return 0;
      }
    }
    xindex++;					//avanza lungo l'asse
    GridToPosition(& walking_point, xindex, yindex, zindex);
    debug3++;
    if( debug3 > 100000 && verboso)
    {
      cout << "RIGA 269 PROBLEMA CICLO!" << endl;
      return 0;
    }
  }
  
  debug1 = 0;
  debug2 = 0;
  debug3 = 0;
  
  //set del potenziale vettore all'esterno del solenoide !!DI LATO!!
  walking_point = {0., ysol * .5, 0};		//inizia da di lato al solenoide
  while(walking_point.x < xsol*.5)		//ciclo sulla lunghezza del solenoide
  {
    while(walking_point.z < zsol*.5)		//ciclo sull'altezza del solenoide
    {
      while(walking_point.y < ysyst*.5)		//riempie striscia di larghezza del sistema
      {
	PositionToGrid(& walking_point, xindex, yindex, zindex);
	modify_inloop = & potential[xindex][yindex][zindex];
	rho = mu_0 * .5 * ysol * .5 * ysol * .5 / sqrt(walking_point.x * walking_point.x + walking_point.y * walking_point.y);
	theta = atan(walking_point.y/walking_point.z);		//ordine giusto? credo di sì, basta mantenere quello sopra qua come quello con seno quando riempio
	* modify_inloop = {0., rho * sin(theta), rho * cos(theta)};
	yindex++;
	GridToPosition(& walking_point, xindex, yindex, zindex);
	debug1++;
	if( debug1 > 100000 && verboso)
	{
	  cout << "RIGA 297 PROBLEMA CICLO!" << endl;
	  return 0;
	}
      }
      zindex++;					//avanza lungo l'altezza
      GridToPosition(& walking_point, xindex, yindex, zindex);
      debug2++;
      if( debug2 > 100000 && verboso)
      {
	cout << "RIGA 306 PROBLEMA CICLO!" << endl;
	return 0;
      }
    }
    xindex++;					//avanza lungo l'asse
    GridToPosition(& walking_point, xindex, yindex, zindex);
    debug3++;
    if( debug3 > 100000 && verboso)
    {
      cout << "RIGA 315 PROBLEMA CICLO!" << endl;
      return 0;
    }
  }
  
  debug1 = 0;
  debug2 = 0;
  debug3 = 0;
  
  //set del potenziale vettore all'esterno del solenoide !!ACCANTO!!
  walking_point = {0., ysol * .5, zsol * .5};	//inizia dal'angolino del solenoide
  while(walking_point.x < xsol*.5)		//ciclo sulla lunghezza del solenoide
  {
    while(walking_point.z < zsyst*.5)		//ciclo sull'altezza del sistema
    {
      while(walking_point.y < ysyst*.5)		//riempie striscia di larghezza del sistema
      {
	PositionToGrid(& walking_point, xindex, yindex, zindex);
	modify_inloop = & potential[xindex][yindex][zindex];
	rho = mu_0 * .5 * zsol * .5 * ysol * .5 / sqrt(walking_point.x * walking_point.x + walking_point.y * walking_point.y);	//ibrido tra il sopra e il di lato
	theta = atan(walking_point.y/walking_point.z);		//ordine giusto? credo di sì, basta mantenere quello sopra qua come quello con seno quando riempio
	* modify_inloop = {0., rho * sin(theta), rho * cos(theta)};
	yindex++;
	GridToPosition(& walking_point, xindex, yindex, zindex);
	debug1++;
	if( debug1 > 100000 && verboso)
	{
	  cout << "RIGA 342 PROBLEMA CICLO!" << endl;
	  return 0;
	}
      }
      zindex++;					//avanza lungo l'altezza
      GridToPosition(& walking_point, xindex, yindex, zindex);
      debug2++;
      if( debug2 > 100000 && verboso)
      {
	cout << "RIGA 351 PROBLEMA CICLO!" << endl;
	return 0;
      }
    }
    xindex++;					//avanza lungo l'asse
    GridToPosition(& walking_point, xindex, yindex, zindex);
    debug3++;
    if( debug3 > 100000 && verboso)
    {
      cout << "RIGA 360 PROBLEMA CICLO!" << endl;
      return 0;
    }
  } 
  //il resto del potenziale vettore è alsciato a zero, verrà poi modificato dall'algoritmo numerico.
  
  
  
  
  /*
  
  //calcolo del potenziale vettore utilizzando l'algoritmo di Jacobi, usando i due campi field e potential
  //le condizioni al contorno sono miste: quando il punto è alla fine dell'intero sistema allora il potenziale vettore è fissato a zero,
  //quando invece si è nelle "pareti fittizie" (quelle date dalla divisione per simmetria del problema) allora sono riflettenti.
  long ninteractions = 1000;

//   Position3D numeric_start = {0,0,0};
  

//lavoro nel discreto, ignoro il continuo

PositionToGrid(& walking_point, xindex, yindex, zindex);
int x_border = xindex;
int y_border = yindex;
int z_border = zindex;

double analyzedpoint;
double above; 
double below; 
double left;
double right;
double beyond;
double before;

double zero = 0;
double currentj;

for(unsigned int i = 0; i < ninteractions; i++)
{
  
  xindex = x_border;
  yindex = y_border;
  zindex = z_border;
  //X DEL POTENZIALE VETTORE
  while(xindex < xsteps)
  {
    while(yindex < ysteps)
    {
      while(zindex < zsteps)
      {
	if(xindex == x_border)                     before = potential[xindex+1][yindex][zindex].x;	//riflessione
	else                                       before = potential[xindex-1][yindex][zindex].x;	//bulk
	
	if(xindex == xsteps - 1)                   beyond = zero;						//nullo
	else                                       beyond = potential[xindex+1][yindex][zindex].x;	//bulk
	
	if(yindex == y_border)                     left = potential[xindex][yindex+1][zindex].x;	//riflessione
	else                                       left = potential[xindex][yindex-1][zindex].x;	//bulk
	
	if(yindex == ysteps - 1)                   right = zero;						//nullo
	else                                       right = potential[xindex][yindex+1][zindex].x;	//bulk
	
	if(zindex == z_border)                     below = potential[xindex][yindex][zindex-1].x;	//riflessione
	else                                       below = potential[xindex][yindex][zindex-1].x;	//bulk
	
	if(zindex == zsteps - 1)                   above = zero;						//nullo
	else                                       above = potential[xindex][yindex][zindex+1].x;	//bulk
	
	currentj = currents[xindex][yindex][zindex].x;
	analyzedpoint = (above + below + left + right + beyond + before - (mu_0/currentj))/6.;
	field[xindex][yindex][zindex].x = analyzedpoint;
	
	zindex++;
      }
      yindex++;
    }
    xindex++;
  }
  
  xindex = x_border;
  yindex = y_border;
  zindex = z_border;
  while(xindex < xsteps)
  {
    while(yindex < ysteps)
    {
      while(zindex < zsteps)
      {
	if(xindex == x_border)                     before = field[xindex+1][yindex][zindex].x;	//riflessione
	else                                       before = field[xindex-1][yindex][zindex].x;	//bulk
	
	if(xindex == xsteps - 1)                   beyond = zero;						//nullo
	else                                       beyond = field[xindex+1][yindex][zindex].x;	//bulk
	
	if(yindex == y_border)                     left = field[xindex][yindex+1][zindex].x;	//riflessione
	else                                       left = field[xindex][yindex-1][zindex].x;	//bulk
	
	if(yindex == ysteps - 1)                   right = zero;						//nullo
	else                                       right = field[xindex][yindex+1][zindex].x;	//bulk
	
	if(zindex == z_border)                     below = field[xindex][yindex][zindex-1].x;	//riflessione
	else                                       below = field[xindex][yindex][zindex-1].x;	//bulk
	
	if(zindex == zsteps - 1)                   above = zero;						//nullo
	else                                       above = field[xindex][yindex][zindex+1].x;	//bulk
	
	currentj = currents[xindex][yindex][zindex].x;
	analyzedpoint = (above + below + left + right + beyond + before - (mu_0/currentj))/6.;
	potential[xindex][yindex][zindex].x = analyzedpoint;
	
	zindex++;
      }
      yindex++;
    }
    xindex++;
  }


  //Y POTENZIALE VETTORE
  xindex = x_border;
  yindex = y_border;
  zindex = z_border;
  while(xindex < xsteps)
  {
    while(yindex < ysteps)
    {
      while(zindex < zsteps)
      {
	if(xindex == x_border)                     before = potential[xindex+1][yindex][zindex].y;	//riflessione
	else                                       before = potential[xindex-1][yindex][zindex].y;	//bulk
	
	if(xindex == xsteps - 1)                   beyond = zero;						//nullo
	else                                       beyond = potential[xindex+1][yindex][zindex].y;	//bulk
	
	if(yindex == y_border)                     left = potential[xindex][yindex+1][zindex].y;	//riflessione
	else                                       left = potential[xindex][yindex-1][zindex].y;	//bulk
	
	if(yindex == ysteps - 1)                   right = zero;						//nullo
	else                                       right = potential[xindex][yindex+1][zindex].y;	//bulk
	
	if(zindex == z_border)                     below = potential[xindex][yindex][zindex-1].y;	//riflessione
	else                                       below = potential[xindex][yindex][zindex-1].y;	//bulk
	
	if(zindex == zsteps - 1)                   above = zero;						//nullo
	else                                       above = potential[xindex][yindex][zindex+1].y;	//bulk
	
	currentj = currents[xindex][yindex][zindex].y;
	analyzedpoint = (above + below + left + right + beyond + before - (mu_0/currentj))/6.;
	field[xindex][yindex][zindex].y = analyzedpoint;
	
	zindex++;
      }
      yindex++;
    }
    xindex++;
  }
  
  xindex = x_border;
  yindex = y_border;
  zindex = z_border;
  while(xindex < xsteps)
  {
    while(yindex < ysteps)
    {
      while(zindex < zsteps)
      {
	if(xindex == x_border)                     before = field[xindex+1][yindex][zindex].y;	//riflessione
	else                                       before = field[xindex-1][yindex][zindex].y;	//bulk
	
	if(xindex == xsteps - 1)                   beyond = zero;						//nullo
	else                                       beyond = field[xindex+1][yindex][zindex].y;	//bulk
	
	if(yindex == y_border)                     left = field[xindex][yindex+1][zindex].y;	//riflessione
	else                                       left = field[xindex][yindex-1][zindex].y;	//bulk
	
	if(yindex == ysteps - 1)                   right = zero;						//nullo
	else                                       right = field[xindex][yindex+1][zindex].y;	//bulk
	
	if(zindex == z_border)                     below = field[xindex][yindex][zindex-1].y;	//riflessione
	else                                       below = field[xindex][yindex][zindex-1].y;	//bulk
	
	if(zindex == zsteps - 1)                   above = zero;						//nullo
	else                                       above = field[xindex][yindex][zindex+1].y;	//bulk
	
	currentj = currents[xindex][yindex][zindex].y;
	analyzedpoint = (above + below + left + right + beyond + before - (mu_0/currentj))/6.;
	potential[xindex][yindex][zindex].y = analyzedpoint;
	
	zindex++;
      }
      yindex++;
    }
    xindex++;
  }
  
  
  //Z POTENZIALE VETTORE
  xindex = x_border;
  yindex = y_border;
  zindex = z_border;
  while(xindex < xsteps)
  {
    while(yindex < ysteps)
    {
      while(zindex < zsteps)
      {
	if(xindex == x_border)                     before = potential[xindex+1][yindex][zindex].z;	//riflessione
	else                                       before = potential[xindex-1][yindex][zindex].z;	//bulk
	
	if(xindex == xsteps - 1)                   beyond = zero;						//nullo
	else                                       beyond = potential[xindex+1][yindex][zindex].z;	//bulk
	
	if(yindex == y_border)                     left = potential[xindex][yindex+1][zindex].z;	//riflessione
	else                                       left = potential[xindex][yindex-1][zindex].z;	//bulk
	
	if(yindex == ysteps - 1)                   right = zero;						//nullo
	else                                       right = potential[xindex][yindex+1][zindex].z;	//bulk
	
	if(zindex == z_border)                     below = potential[xindex][yindex][zindex-1].z;	//riflessione
	else                                       below = potential[xindex][yindex][zindex-1].z;	//bulk
	
	if(zindex == zsteps - 1)                   above = zero;						//nullo
	else                                       above = potential[xindex][yindex][zindex+1].z;	//bulk
	
	currentj = currents[xindex][yindex][zindex].z;
	analyzedpoint = (above + below + left + right + beyond + before - (mu_0/currentj))/6.;
	field[xindex][yindex][zindex].z = analyzedpoint;
	
	zindex++;
      }
      yindex++;
    }
    xindex++;
  }
  
  xindex = x_border;
  yindex = y_border;
  zindex = z_border;
  while(xindex < xsteps)
  {
    while(yindex < ysteps)
    {
      while(zindex < zsteps)
      {
	if(xindex == x_border)                     before = field[xindex+1][yindex][zindex].z;	//riflessione
	else                                       before = field[xindex-1][yindex][zindex].z;	//bulk
	
	if(xindex == xsteps - 1)                   beyond = zero;						//nullo
	else                                       beyond = field[xindex+1][yindex][zindex].z;	//bulk
	
	if(yindex == y_border)                     left = field[xindex][yindex+1][zindex].z;	//riflessione
	else                                       left = field[xindex][yindex-1][zindex].z;	//bulk
	
	if(yindex == ysteps - 1)                   right = zero;						//nullo
	else                                       right = field[xindex][yindex+1][zindex].z;	//bulk
	
	if(zindex == z_border)                     below = field[xindex][yindex][zindex-1].z;	//riflessione
	else                                       below = field[xindex][yindex][zindex-1].z;	//bulk
	
	if(zindex == zsteps - 1)                   above = zero;						//nullo
	else                                       above = field[xindex][yindex][zindex+1].z;	//bulk
	
	currentj = currents[xindex][yindex][zindex].z;
	analyzedpoint = (above + below + left + right + beyond + before - (mu_0/currentj))/6.;
	potential[xindex][yindex][zindex].z = analyzedpoint;
	
	zindex++;
      }
      yindex++;
    }
    xindex++;
  }

}


//stima del campo magnetico a partire dal potenziale vettore appena calcolato
double dxy, dxz, dyx, dyz, dzx, dzy;
double beforex, beforey, beforez;
double beyondx, beyondy, beyondz;
double leftx, lefty, leftz;
double rightx, righty, rightz;
double belowx, belowy, belowz;
double abovex, abovey, abovez;

double actualx, actualy, actualz;

xindex = x_border;
yindex = y_border;
zindex = z_border;
while(xindex < xsteps)
{
  while(yindex < ysteps)
  {
    while(zindex < zsteps)
    {
      if(xindex == x_border)                     
      {
	beforex = potential[xindex+1][yindex][zindex].x;	//riflessione
	beforey = potential[xindex+1][yindex][zindex].y;	//riflessione
	beforez = potential[xindex+1][yindex][zindex].z;	//riflessione
      } else
      {
	beforex = potential[xindex-1][yindex][zindex].x;	//bulk
	beforey = potential[xindex-1][yindex][zindex].y;	//bulk
	beforez = potential[xindex-1][yindex][zindex].z;	//bulk
      }
      if(xindex == xsteps - 1)            
      {
	beyondx = zero;						//nullo
	beyondy = zero;						//nullo
	beyondz = zero;						//nullo
      } else 
      {
	beyondx = potential[xindex+1][yindex][zindex].x;	//bulk
	beyondy = potential[xindex+1][yindex][zindex].y;	//bulk
	beyondz = potential[xindex+1][yindex][zindex].z;	//bulk
      }
      if(yindex == y_border)   
      {
	leftx = potential[xindex][yindex+1][zindex].x;		//riflessione
	lefty = potential[xindex][yindex+1][zindex].y;		//riflessione
	leftz = potential[xindex][yindex+1][zindex].z;		//riflessione
      } else
      {
	leftx = potential[xindex][yindex-1][zindex].x;		//bulk
	lefty = potential[xindex][yindex-1][zindex].y;		//bulk
	leftz = potential[xindex][yindex-1][zindex].z;		//bulk
      }
      if(yindex == ysteps - 1)      
      {
	rightx = zero;						//nullo
	righty = zero;						//nullo
	rightz = zero;						//nullo
      } else 
      {
	rightx = potential[xindex][yindex+1][zindex].x;		//bulk
	righty = potential[xindex][yindex+1][zindex].y;		//bulk
	rightz = potential[xindex][yindex+1][zindex].z;		//bulk
      }
      if(zindex == z_border)    
      {
	belowx = potential[xindex][yindex][zindex-1].x;		//riflessione
	belowy = potential[xindex][yindex][zindex-1].y;		//riflessione
	belowz = potential[xindex][yindex][zindex-1].z;		//riflessione
      } else 
      {
	belowx = potential[xindex][yindex][zindex-1].x;		//bulk
	belowy = potential[xindex][yindex][zindex-1].y;		//bulk
	belowz = potential[xindex][yindex][zindex-1].z;		//bulk
      }
      if(zindex == zsteps - 1)      
      {
	abovex = zero;						//nullo
	abovey = zero;						//nullo
	abovez = zero;						//nullo
      } else      
      {
	abovex = potential[xindex][yindex][zindex+1].x;		//bulk
	abovey = potential[xindex][yindex][zindex+1].y;		//bulk
	abovez = potential[xindex][yindex][zindex+1].z;		//bulk
      }
      
      
      dxy = (rightx - leftx)    / (2 * delta);			//calcolo delle derivate
      dxz = (abovex - belowx)   / (2 * delta);
      dyx = (beyondy - beforey) / (2 * delta);
      dyz = (abovey - belowy)   / (2 * delta);
      dzx = (beyondz - belowz)  / (2 * delta);
      dzy = (abovez - belowz)   / (2 * delta);
      
      actualx = dzy - dyz;					//calcolo del rotore
      actualy = dxz - dzx;
      actualz = dyx - dxy;
      
      field[xindex][yindex][zindex].x = actualx;		//riempimento effettivo campo
      field[xindex][yindex][zindex].y = actualy;
      field[xindex][yindex][zindex].z = actualz;
      
      
      zindex++;
    }
    yindex++;
  }
  xindex++;
}


*/

  return 0;
//   return B{1,0,0};
};


void PositionToGrid(const Position3D * position, int & xindex, int & yindex, int & zindex)
{
  double delta = 0.1;		//dovrebbe vederla dal programma andrà implementato nella classe
  xindex = int(position->x/delta);
  yindex = int(position->y/delta);
  zindex = int(position->z/delta);
  return;
}

void GridToPosition(Position3D * position, const int & xindex, const int & yindex, const int & zindex)
{
  double delta = 0.1;
  position->x = xindex * delta;
  position->y = yindex * delta;
  position->z = zindex * delta;
  return;
}
