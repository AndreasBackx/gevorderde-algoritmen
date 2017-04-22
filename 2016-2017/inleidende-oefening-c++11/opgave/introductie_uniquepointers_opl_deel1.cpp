// DEEL 1
// oplossing van deel 1 van de vraag

    // De testen moeten zeker aangevuld worden!
	// De 'gewone' zaken worden wel getest, maar wat NIET getest werd:
	//   1. samenvoegen van/met lege lijsten
	//   2. kopiëren van lijsten		
	//   3. a.append(a); --> dat zou geen effect mogen hebben OF zou a
	//                       moeten verdubbelen (er werd voor dat laatste gekozen)   
	//   4. zijn de deletes in orde?

#include <iostream>
#include <sstream>
using namespace std;

typedef int T;

class Knoop;

class Lijst{
	public:
	    Lijst();
		Lijst(const T &);		
		~Lijst();            
		Lijst(const Lijst & andereLijst); //+/ toe te voegen; deel van the big three
		Lijst & operator=(const Lijst& andereLijst); //+/ toe te voegen
	
		void append(Lijst & andereLijst);
		string to_string() const;
			
	private:
		Knoop* eerste;
		Knoop* laatste;	
		void schrijf(ostream & out)const;	
			
		void initialiseer_laatste();	
			
	friend ostream & operator<<(ostream & out, const Lijst & l){
		l.schrijf(out);
		return out;
	}	
	
};

class Knoop{
	friend class Lijst;
	private:
		T sl;
		Knoop* volgende; 
	public:
		Knoop(const T & _sl):sl(_sl),volgende(0){};  //+/ pointer initialiseren
		~Knoop();             //+/ toe te voegen
		Knoop(const Knoop & andereKnoop); //+/ toe te voegen
};


////////////////////////////////////////////////////////
//////////  implementatie Knoop
////////////////////////////////////////////////////////

Knoop::~Knoop(){
	delete volgende;
}

Knoop::Knoop(const Knoop & andereKnoop){
	sl = andereKnoop.sl;
	if(andereKnoop.volgende != 0){
		volgende = new Knoop(*andereKnoop.volgende); // recursief
	}
	else{
		volgende = 0;
	}
}
		
////////////////////////////////////////////////////////
//////////  implementatie Lijst
////////////////////////////////////////////////////////

Lijst::Lijst(){
	eerste = 0;
	laatste = 0;
}

Lijst::Lijst(const T & t){
	eerste = new Knoop(t);
	laatste = eerste;	
}

Lijst::~Lijst(){    //+/ maar dan ook nodig bij Knoop!
	delete eerste;
}

//+/ toe te voegen
Lijst::Lijst(const Lijst & andereLijst){
	//+/ je moet twee dataleden initialiseren: 
	//+/ 'eerste' en 'laatste'.
	//+/ Oppassen met die laatste! Wat gebeurt er als je het, naïef, zoals hieronder doet???
	//+/ Ga na!!
	//+/ laatste = andereLijst.laatste; 
	if(andereLijst.eerste != 0){	
		eerste = new Knoop(*andereLijst.eerste); //+/ maar dan ook nodig bij Knoop!
	}
	else{
		eerste = 0;
	}
	
	initialiseer_laatste();
}

void Lijst::initialiseer_laatste(){
	Knoop * loper = eerste;
	Knoop * vorige = 0;
	while(loper != 0){
		vorige = loper;
		loper = loper->volgende;
	}
	laatste = vorige;	
}

Lijst & Lijst::operator=(const Lijst & andereLijst){
    if(this != &andereLijst){
    	delete eerste; // haalt via destructor van knoop ook alle andere knopen weg
    	if(andereLijst.eerste != 0){
    		eerste = new Knoop(*andereLijst.eerste);
		}
		else{
			eerste = 0;
		}
		initialiseer_laatste();
	}	
}

//+/ 1. aan te passen voor lege lijsten
//+/ 2. aan te passen voor a.append(a); in dat geval lijst verdubbelen
void Lijst::append(Lijst & andereLijst){
	if(this == &andereLijst){
		Lijst echt_andere(*this);
		append(echt_andere);
		return;
	}
	//+/ onderstaande fout/vergetelheid had je op zicht kunnen zien:
	//+/ als er "laatste->..." staat, moet je zeker zijn dat "laatste"
	//+/ niet de nullpointer is!
	if(laatste != 0){	    	            
		laatste->volgende = andereLijst.eerste;
	}
	else{
		eerste = andereLijst.eerste;
	}
	laatste = andereLijst.laatste;
	andereLijst.laatste = 0;
	andereLijst.eerste = 0;
}

void Lijst::schrijf(ostream & out)const{
	out<<to_string();
}

string Lijst::to_string()const{
	string str="";
	Knoop* loper = eerste;
	while(loper!=0){
		str = str + std::to_string(loper->sl) + "-";  // to_string(int) is nieuw voor C++11
		loper = loper->volgende;
	}
	str += "X";
	return str;
}

////////////////////////////////////////////////////////
//////////  test-functies
////////////////////////////////////////////////////////

void test_append(Lijst & a, Lijst & b, const string & naam_a, const string & naam_b, const string & verwachte_uitkomst_a, const string & verwachte_uitkomst_b){	
	cout<<endl<<"    Voor append: \n    "<<naam_a<<": "<<a<<"\n    "<<naam_b<<": "<<b<<endl;	
	a.append(b);		
	cout<<endl<<"    Na append: \n    "<<naam_a<<": "<<a<<"\n    "<<naam_b<<": "<<b<<endl;
	if(a.to_string() == verwachte_uitkomst_a && b.to_string() == verwachte_uitkomst_b){
		cout<<"ok"<<endl;	
	}
	else{
		cout<<"niet ok"<<endl;
	}
}

void test_gelijkheid(const Lijst & a, const string & verwachte_voorstelling){
	if(a.to_string() == verwachte_voorstelling){
		cout<<"ok"<<endl;	
	}
	else{
		cout<<"niet ok"<<endl;
	}	
}

void test_basic(){	
	cout<<endl<<"BASIC TEST"<<endl;
	Lijst a(5);
	Lijst b(10);

	test_append(a,b,"a","b","5-10-X","X");
	
	Lijst x(0);
	for(int i=1; i<=10; i++){
		Lijst y(i);
		x.append(y);
	}
	cout<<endl<<"    x: "<<x<<endl;
	test_gelijkheid(x,"0-1-2-3-4-5-6-7-8-9-10-X");
}


void test_lege_lijsten(){		
	cout<<endl<<"TEST VOOR LEGE LIJSTEN"<<endl;
	Lijst a;
	Lijst b(10);
	test_append(a,b,"a","b","10-X","X");
	
	Lijst c,d;	
	test_append(c,d,"c","d","X","X");
	
	Lijst e(3);
	Lijst f;	
	test_append(e,f,"e","f","3-X","X");
}

void test_kopieren(){	
	cout<<endl<<"TEST VOOR COPY-OPERATOR"<<endl;
	Lijst x(0);
	for(int i=1; i<=5; i++){
		Lijst y(i);
		x.append(y);
	}
	Lijst y(x);
	cout<<"\n    Ik copieerde x naar y \n    x: "<<x<<endl<<"    y: "<<y<<endl;	
	test_gelijkheid(x,"0-1-2-3-4-5-X");
	test_gelijkheid(y,"0-1-2-3-4-5-X");
	
	Lijst a(999);
	y.append(a);
	cout<<"\n    Bewijs dat het om een diepe copie gaat; ik verleng y (niet x) met 999\n";	
	test_gelijkheid(x,"0-1-2-3-4-5-X");	
	test_gelijkheid(y,"0-1-2-3-4-5-999-X");
	
	a=x;
	cout<<"\n    Nu maakte ik nog een copie van x, met operator=\n";
	test_gelijkheid(x,"0-1-2-3-4-5-X");	
	test_gelijkheid(y,"0-1-2-3-4-5-999-X");
	test_gelijkheid(a,"0-1-2-3-4-5-X");
	
	a=a;
	cout<<"\n    Operator= werkt ook met 'a=a'\n";
	test_gelijkheid(a,"0-1-2-3-4-5-X");	
		
}

void test_append_met_zichzelf(){
	cout<<endl<<"TEST VOOR a.APPEND(a)"<<endl;
	cout<<"\n    Poging om a.append(a) te doen, zal lijst verdubbelen";
	Lijst a(5);
	test_append(a,a,"a","a","5-5-X","5-5-X");
}

//+/ merk op: hier laat je alles lopen, maar binnen een afgeschermde scope (let op de accolades)
//+/ Raakt het programma zonder problemen BUITEN die scope, dan liep er niets mis bij het deleten.
//+/ ALS er uiteraard gedeleted werd...
//+/ (om hier zeker van te zijn, zet je de vier bewuste regels in een for-lus en laat het dikwijls genoeg lopen)
void test_deletes(){
	cout<<endl<<"_________________________________\n----------------------------";
	cout<<endl<<"HIER STARTEN DE TESTEN DIE NODIG ZIJN VOOR CONTROLE VAN GEHEUGENGEBRUIK\n";
	{ 
		test_basic(); 
		test_append_met_zichzelf();
		test_kopieren();
		test_lege_lijsten();
	}
	cout<<"\nok\n    Als dit niet crasht, zijn de voorgaande testen ook qua deletes in orde\n    - of voorzichtiger: er wordt niets teveel gedeleted.\n";
	cout<<"    Nog wel na te gaan of er niets te weinig weer vrijgegeven werd....\n";
}

int main(){

    // De testen moeten zeker aangevuld worden!
	// De 'gewone' zaken worden wel getest, maar wat NIET getest werd:
	//   1. samenvoegen van/met lege lijsten
	//   2. kopiëren van lijsten		
	//   3. a.append(a); --> dat zou geen effect mogen hebben 
	//                       (of, indien gewenst, verdubbeling van a)
	//   4. zijn de deletes in orde?

    // Je zorgt er bij het testen ook best voor dat je ONMIDDELLIJK 
	// ziet of de test gelukt is.
    // Dat kan op verschillende manieren, maar laat het in het oog springen! 
	
	test_basic();
	test_lege_lijsten();
	test_kopieren();
	test_append_met_zichzelf();
	test_deletes();
	return 0;
}





