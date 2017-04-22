
#ifndef KNOOP_H
#define KNOOP_H

typedef int T;

class Knoop {
	friend class Lijst;
private:
	T sl;
	Knoop* volgende;
public:

	Knoop(const T & _sl) : sl(_sl), /* FOUT: vergeten */ volgende(0) {
	};
	
	~Knoop();
	
	Knoop(const Knoop & andereKnoop);
//	Knoop & operator= (const Knoop & andereKnoop);
//	
//	Knoop(Knoop&& andereKnoop);
//	Knoop & operator= (Knoop&& andereKnoop);
};

#endif /* KNOOP_H */

