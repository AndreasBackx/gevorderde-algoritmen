
#ifndef REGEXNAARGRAMMATICA_H
#define REGEXNAARGRAMMATICA_H

#include "regexp11.h"
#include "symbool.h"

#include <map>
#include <string>
#include <vector>
#include <sstream>

/* TODO
 * - kijken of ik de methodes can refactoren voor minder replicatie (ze zijn allemaal nogal gelijkaardig)
 *       Strategy pattern?  
 * - hoe starten met S 
 * - stringstream is niet efficient (maar wel leesbaar)
 */

class RegexNaarGrammatica
{
private:
    std::vector<std::string> regels;
    std::map<std::string, Symbool> symbool_map;
    Symbool s;

    Symbool parse_plus(const Regexp* r);
    Symbool parse_of(const Regexp* r);
    Symbool parse_ster(const Regexp* r);
    Symbool parse_letter(const Regexp* r);

public:
    Symbool parse(const Regexp* r);
    std::map<std::string, Symbool> get_symbool_map() const;
    std::vector<std::string> get_regels() const;
    std::string to_string() const;
};

Symbool RegexNaarGrammatica::parse_plus(const Regexp* r)
{
    Symbool eerste_operand = parse(r->geefEersteOperand());
    Symbool tweede_operand = parse(r->geefTweedeOperand());

    std::stringstream regel_beschrijving;
    regel_beschrijving << "+;" << eerste_operand << ";" << tweede_operand;

    std::map<std::string, Symbool>::iterator iter_symbool_map = symbool_map.find(regel_beschrijving.str());
    if (iter_symbool_map != symbool_map.end())
    {
        return iter_symbool_map->second;
    }

    Symbool regel_hoofd = ++s;
    symbool_map[regel_beschrijving.str()] = regel_hoofd;

    std::stringstream regel;
    regel << "<" << regel_hoofd << "> ::= <" << eerste_operand << "><" << tweede_operand << ">";
    regels.push_back(regel.str());

    return regel_hoofd;
}

Symbool RegexNaarGrammatica::parse_of(const Regexp* r)
{
    Symbool eerste_operand = parse(r->geefEersteOperand());
    Symbool tweede_operand = parse(r->geefTweedeOperand());

    std::stringstream regel_beschrijving;
    regel_beschrijving << "|;" << eerste_operand << ";" << tweede_operand;

    std::map<std::string, Symbool>::iterator iter_symbool_map = symbool_map.find(regel_beschrijving.str());
    if (iter_symbool_map != symbool_map.end())
    {
        return iter_symbool_map->second;
    }

    Symbool regel_hoofd = ++s;
    symbool_map[regel_beschrijving.str()] = regel_hoofd;

    std::stringstream regel;
    regel << "<" << regel_hoofd << "> ::= <" << eerste_operand << ">|<" << tweede_operand << ">";
    regels.push_back(regel.str());

    return regel_hoofd;
}

Symbool RegexNaarGrammatica::parse_ster(const Regexp* r)
{
    Symbool eerste_operand = parse(r->geefEersteOperand());

    std::stringstream regel_beschrijving;
    regel_beschrijving << "*;" << eerste_operand;

    std::map<std::string, Symbool>::iterator iter_symbool_map = symbool_map.find(regel_beschrijving.str());
    if (iter_symbool_map != symbool_map.end())
    {
        return iter_symbool_map->second;
    }

    Symbool regel_hoofd = ++s;
    symbool_map[regel_beschrijving.str()] = regel_hoofd;

    std::stringstream regel;
    regel << "<" << regel_hoofd << "> ::= <" << eerste_operand << "><" << regel_hoofd << ">|e";
    regels.push_back(regel.str());

    return regel_hoofd;
}

Symbool RegexNaarGrammatica::parse_letter(const Regexp* r)
{
    uchar letter = r->geefLetter();

    std::stringstream regel_beschrijving;
    regel_beschrijving << ";" << letter;

    std::map<std::string, Symbool>::iterator iter_symbool_map = symbool_map.find(regel_beschrijving.str());
    if (iter_symbool_map != symbool_map.end())
    {
        return iter_symbool_map->second;
    }

    Symbool regel_hoofd = ++s;
    symbool_map[regel_beschrijving.str()] = regel_hoofd;

    std::stringstream regel;
    regel << "<" << regel_hoofd << "> ::= " << letter;
    regels.push_back(regel.str());

    return regel_hoofd;
}

Symbool RegexNaarGrammatica::parse(const Regexp* r)
{
    Symbool regel_hoofd = s;

    if (r->geefOpcode() == Regexp::opcode::plus)
    {
        regel_hoofd = parse_plus(r);
    }
    else if (r->geefOpcode() == Regexp::opcode::of)
    {
        regel_hoofd = parse_of(r);
    }
    else if (r->geefOpcode() == Regexp::opcode::ster)
    {
        regel_hoofd = parse_ster(r);
    }
    else if (r->geefOpcode() == Regexp::opcode::letter)
    {
        regel_hoofd = parse_letter(r);
    }
    else
    {
        throw "Unsupported operation in regexp";
    }

    return regel_hoofd;
}

std::map<std::string, Symbool> RegexNaarGrammatica::get_symbool_map() const
{
    return symbool_map;
}

std::vector<std::string> RegexNaarGrammatica::get_regels() const
{
    return regels;
}

std::string RegexNaarGrammatica::to_string() const
{
    std::stringstream ss;

    ss << "Symbool map: " << std::endl;
    for (const std::pair<std::string, Symbool>& p : symbool_map)
    {
        ss << p.first << " -> " << p.second << std::endl;
    }

    ss << std::endl << "Regels: " << std::endl;
    for (int i = regels.size() - 1; i >= 0; i--)
    {
        ss << regels[i] << std::endl;
    }

    return ss.str();
}

#endif

