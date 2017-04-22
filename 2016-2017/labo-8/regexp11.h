
#ifndef __REGEXP
#define __REGEXP

#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <vector>

using std::string;
using std::ostream;
using std::vector;
using std::exception;
using std::unique_ptr;
typedef unsigned char uchar;

//Regexp leest een reguliere expressie in, en zet ze om naar een
//boomstructuur.

/**********************************************************************

   Taalexceptie

 ***************************************************************************/


class Taalexceptie : public exception
{
public:

    Taalexceptie(const string& _fout) :
    exception(), fout(_fout) { };

    ~Taalexceptie() throw () { };

    virtual const char* what()
    {
        return fout.c_str();
    };
private:
    string fout;
};

/**********************************************************************

   class Rexexp
   parset reguliere expressies met alleen |, * en concatenatie
   Een regexp is ofwel een letter, ofwel twee regexpoperanden verbonden
   met een opcode `of' of `plus', ofwel een regexp met de opcode `ster'

 ***************************************************************************/


class Regexp
{
public:

    enum class opcode
    {
        ster, of, plus, letter
    };
    Regexp(const string& re);
    opcode geefOpcode() const;
    const Regexp* geefEersteOperand() const; //zinloos als opcode letter is.
    const Regexp* geefTweedeOperand() const; //zinloos als opcode letter of ster is.
    uchar geefLetter() const; //zinloos als opcode niet letter is.
    friend ostream& operator<<(ostream& os, const Regexp& re);
private:
    //parsen is gemakkelijker als de + expliciet vermeld is
    //maar dit mag van buitenuit niet gezien zijn.
    Regexp(const string& re, bool alvoorzienvanplus);

    string voorzieVanPlus(const string& re) const;
    //zoek meest rechtse voorkomen van operator c die niet tussen haakjes staat.
    int laatste(const string& re, const uchar c, vector<int>& hniveau);
    void analyseer(const string& re);
    void schrijf(ostream& os) const;
    unique_ptr<Regexp> op1, op2;
    opcode code;
    uchar deLetter;
};

Regexp::Regexp(const string& re)
{
    analyseer(voorzieVanPlus(re));
}

Regexp::opcode Regexp::geefOpcode() const
{
    return code;
}

const Regexp* Regexp::geefEersteOperand() const
{
    return op1.get();
}

const Regexp* Regexp::geefTweedeOperand() const
{
    return op2.get();
}

uchar Regexp::geefLetter() const
{
    return deLetter;
}

ostream& operator<<(ostream& os, const Regexp& re)
{
    re.schrijf(os);
    return os;
}

Regexp::Regexp(const string& re, bool alvoorzienvanplus)
{
    if (alvoorzienvanplus || re.size() <= 1)
    {
        analyseer(re);
    }
    else
    {
        analyseer(voorzieVanPlus(re));
    }
}

string Regexp::voorzieVanPlus(const string& re) const
{
    string metplus;
    metplus += re[0];
    for (int i = 1; i < re.size(); i++)
    {
        if (re[i - 1] != '|' && re[i - 1] != '('
            && re[i] != '*' && re[i] != '|' && re[i] != ')')
        {
            metplus += '+';
        }
        metplus += re[i];
    }
    return metplus;
}

int Regexp::laatste(const string& re, const uchar c, vector<int>& hniveau)
{
    int i = re.size() - 1;
    while (i >= 0 && (re[i] != c || hniveau[i] != 0))
        i--;
    return i;
}

void Regexp::analyseer(const string& re)
{
    int n = re.size();
    if (n == 0)
        throw Taalexceptie("leeg operand");
    vector<int> hniveau(n);
    int niveau = 0;
    bool nulniveau = false; //is er iets buiten haakjes?
    for (int i = 0; i < n; i++)
    {
        if (re[i] == '(')
            niveau++;
        if (niveau == 0)
        {
            nulniveau = true;
        }
        hniveau[i] = niveau;
        if (re[i] == ')')
        {
            niveau--;
        }
    }
    if (!nulniveau)
    {//alles tussen haakjes
        analyseer(re.substr(1, n - 2));
    }
    else
    {
        int i;
        if (-1 != (i = laatste(re, '|', hniveau)))
        {
            if (i == 0 || i == n - 1)
                throw Taalexceptie("| met operand te weinig.");
            code = Regexp::opcode::of;
            op1.reset(new Regexp(re.substr(0, i), true));
            op2.reset(new Regexp(re.substr(i + 1, n - i - 1), true));
        }
        else if (-1 != (i = laatste(re, '+', hniveau)))
        {
            if (i == 0 || i == n - 1)
                throw Taalexceptie("+ met operand te weinig.");
            code = Regexp::opcode::plus;
            op1.reset(new Regexp(re.substr(0, i), true));
            op2.reset(new Regexp(re.substr(i + 1, n - i - 1), true));
        }
        else if (re[n - 1] == '*')
        {
            if (n == 1)
                throw Taalexceptie("* zonder operand.");
            op1.reset(new Regexp(re.substr(0, n - 1), true));
            code = Regexp::opcode::ster;
        }
        else
        {
            if (n > 1 || string("()|").find(re[0]) != string::npos)
            {
                string fout = " verloren gelopen ";
                fout += re[0];
                throw Taalexceptie(fout);
            }
            code = Regexp::opcode::letter;
            deLetter = re[0];
        }
    }
}

void Regexp::schrijf(ostream& os) const
{
    switch (code)
    {
        case Regexp::opcode::letter:
            os << deLetter;
            break;
        case Regexp::opcode::ster:
            os << "[" << *op1 << "]*";
            break;
        case Regexp::opcode::plus:
            os << "[" << *op1 << " + " << *op2 << ']';
            break;
        default:
            os << "[" << *op1 << " | " << *op2 << ']';
            break;
    }
}

#endif
