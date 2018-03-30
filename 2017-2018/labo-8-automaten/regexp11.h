#ifndef REGEXP11_H
#define REGEXP11_H

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

typedef unsigned char uchar;
// Regexp leest een reguliere expressie in, en zet ze om naar een
// boomstructuur.
/**********************************************************************

   Taalexceptie

***************************************************************************/

class Taalexceptie : public std::exception
{
public:
    Taalexceptie(const std::string& fout) : std::exception(), fout(fout)
    {
    }

    ~Taalexceptie() throw()
    {
    }

    virtual const char* what()
    {
        return fout.c_str();
    }

private:
    std::string fout;
};
/**********************************************************************

   class Rexexp
   parset reguliere expressies met alleen |, * en concatenatie
   Een regexp is ofwel een letter, ofwel twee regexpoperanden verbonden
   met een opcode `of' of `plus', ofwel een regexp met de opcode `ster'
   epsilon wordt beschouwd als een gewone letter.

***************************************************************************/

class Regexp
{
public:
    enum opcode
    {
        ster,
        of,
        plus,
        letter
    };

    Regexp(const std::string& re);
    opcode geefOpcode() const;
    const Regexp* geefEersteOperand() const; // zinloos als opcode letter is.
    const Regexp* geefTweedeOperand() const; // zinloos als opcode letter of ster is.
    uchar geefLetter() const; // zinloos als opcode niet letter is.
    friend std::ostream& operator<<(std::ostream& os, const Regexp& re);

private:
    // parsen is gemakkelijker als de + expliciet vermeld is
    // maar dit mag van buitenuit niet gezien zijn.
    Regexp(const std::string& re, bool alvoorzienvanplus);

    std::string voorzieVanPlus(const std::string& re) const;
    // zoek meest rechtse voorkomen van operator c die niet tussen haakjes staat.
    int laatste(const std::string& re, const uchar c, std::vector<int>& hniveau);
    void analyseer(const std::string& re);
    void schrijf(std::ostream& os) const;
    std::unique_ptr<Regexp> op1, op2;
    opcode deOpcode;
    uchar deLetter;
};

Regexp::Regexp(const std::string& re)
{
    analyseer(voorzieVanPlus(re));
}

Regexp::opcode Regexp::geefOpcode() const
{
    return deOpcode;
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

std::ostream& operator<<(std::ostream& os, const Regexp& re)
{
    re.schrijf(os);
    return os;
}

Regexp::Regexp(const std::string& re, bool alvoorzienvanplus)
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

std::string Regexp::voorzieVanPlus(const std::string& re) const
{
    std::string metplus;
    metplus += re[0];
    for (int i = 1; i < re.size(); i++)
    {
        if (re[i - 1] != '|' && re[i - 1] != '(' && re[i] != '*' && re[i] != '|' && re[i] != ')')
        {
            metplus += '+';
        }
        metplus += re[i];
    }
    return metplus;
}

int Regexp::laatste(const std::string& re, const uchar c, std::vector<int>& hniveau)
{
    int i = re.size() - 1;
    while (i >= 0 && (re[i] != c || hniveau[i] != 0))
        i--;
    return i;
}

void Regexp::analyseer(const std::string& re)
{
    int n = re.size();
    if (n == 0)
        throw Taalexceptie("leeg operand");
    std::vector<int> hniveau(n);
    int niveau = 0;
    bool nulniveau = false; // is er iets buiten haakjes?
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
    { // alles tussen haakjes
        analyseer(re.substr(1, n - 2));
    }
    else
    {
        int i;
        if (-1 != (i = laatste(re, '|', hniveau)))
        {
            if (i == 0 || i == n - 1)
                throw Taalexceptie("| met operand te weinig.");
            deOpcode = of;
            // Noot:
            //            op1=make_unique<Regexp>(re.substr(0,i),true);
            // wordt niet gebruikt omdat de hier gebruikte constructoren verborgen moeten blijven en dus protected zijn
            op1.reset(new Regexp(re.substr(0, i), true));
            op2.reset(new Regexp(re.substr(i + 1, n - i - 1), true));
        }
        else if (-1 != (i = laatste(re, '+', hniveau)))
        {
            if (i == 0 || i == n - 1)
                throw Taalexceptie("+ met operand te weinig.");
            deOpcode = plus;
            op1.reset(new Regexp(re.substr(0, i), true));
            op2.reset(new Regexp(re.substr(i + 1, n - i - 1), true));
        }
        else if (re[n - 1] == '*')
        {
            if (n == 1)
                throw Taalexceptie("* zonder operand.");
            op1.reset(new Regexp(re.substr(0, n - 1), true));
            deOpcode = ster;
        }
        else
        {
            if (n > 1 || std::string("()|").find(re[0]) != std::string::npos)
            {
                std::string fout = " verloren gelopen ";
                fout += re[0];
                throw Taalexceptie(fout);
            }
            deOpcode = letter;
            deLetter = re[0];
        }
    }
}

void Regexp::schrijf(std::ostream& os) const
{
    switch (deOpcode)
    {
        case letter:
            os << deLetter;
            break;
        case ster:
            os << "[" << *op1 << "]*";
            break;
        case plus:
            os << "[" << *op1 << " + " << *op2 << ']';
            break;
        default:
            os << "[" << *op1 << " | " << *op2 << ']';
            break;
    }
}

#endif
