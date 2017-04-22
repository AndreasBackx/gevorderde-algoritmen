
#ifndef LIJST_H
#define LIJST_H

#include <ostream>
#include <string>
#include <memory>
#include "Knoop.h"

class Lijst {
public:

    // constructor and destructor

    Lijst() : eerste(nullptr), laatste(nullptr) {
    };

    Lijst(const T & t) : eerste(new Knoop(t)) {
        laatste = eerste.get();
    };
    // ~Lijst();

    // copy

    Lijst(const Lijst & andereLijst) : eerste(new Knoop(*(andereLijst.eerste))) {
        if (eerste) {
            laatste = eerste.get();
            while (laatste->volgende) {
                laatste = laatste->volgende;
            }
        } else {
            laatste = nullptr;
        }
    };

    Lijst & operator=(const Lijst & andereLijst);
    //	
    //	// move
    //	Lijst(Lijst && andereLijst);
    //	Lijst & operator= (Lijst && andereLijst);

    void append(Lijst & andereLijst);
    std::string to_string() const; // handig voor testen
private:
    std::unique_ptr<Knoop> eerste;
    Knoop* laatste;

    void schrijf(std::ostream & out) const;

    friend std::ostream & operator<<(std::ostream & out, const Lijst & l) {
        l.schrijf(out);
        return out;
    }
};

#endif /* LIJST_H */

