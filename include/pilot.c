//
// Created by Bib's on 22-07-25.
//
#include "pilot.h"

#define PILOT_H



const GrandPrix GP_LIST[] = {
    {"Bahrain_GP", "Bahrain International Circuit", 57, 0, 0},
    {"Saudi_Arabian_GP", "Jeddah Corniche Circuit", 50, 0, 0},
    {"Australian_GP", "Albert Park Circuit", 58, 0, 0},
    {"Japanese_GP", "Suzuka", 53, 0, 0},
    {"Chinese_GP", "Shanghai International Circuit", 56, 0, 0},
    {"Miami_GP", "Miami International Autodrome", 57, 0, 0},
    {"Emilia-Romagna_GP", "Autodromo Enzo e Dino Ferrari", 63, 1, 17},
    {"Monaco_GP", "Circuit de Monaco", 78, 0, 0},
    {"Canadian_GP", "Circuit Gilles Villeneuve", 70, 0, 0},
    {"Spanish_GP", "Circuit de Barcelona-Catalunya", 66, 0, 0},
    {"Austrian_GP", "Red Bull Ring", 71, 1, 23},
    {"British_GP", "Silverstone", 52, 0, 0},
    {"Hungarian_GP", "Hungaroring", 70, 0, 0},
    {"Belgian_GP", "Spa-Francorchamps", 44, 1, 15},
    {"Dutch_GP", "Zandvoort", 72, 0, 0},
    {"Italian_GP", "Monza", 53, 0, 0},
    {"Azerbaijan_GP", "Baku City Circuit", 51, 1, 17},
    {"Singapore_GP", "Marina Bay", 62, 0, 0},
    {"United States_GP", "Circuit of the Americas", 56, 1, 19},
    {"Mexico_GP", "Autodromo Hermanos Rodríguez", 71, 0, 0},
    {"São_Paulo_GP", "Interlagos", 71, 1, 24},
    {"Las_Vegas_GP", "Las Vegas Strip Circuit", 50, 0, 0},
    {"Qatar_GP", "Losail International Circuit", 57, 0, 0},
    {"Abu_Dhabi_GP", "Yas Marina Circuit", 58, 0, 0}
};




const Driver DRIVER_LIST[] = {
    {1, "Max Verstappen", "Red Bull Racing"},
    {2, "Logan Sargeant", "Williams Racing"},
    {3, "Daniel Ricciardo", "Scuderia AlphaTauri"},
    {4, "Lando Norris", "McLaren F1 Team"},
    {10, "Pierre Gasly", "BWT Alpine F1 Team"},
    {11, "Sergio Perez", "Red Bull Racing"},
    {14, "Fernando Alonso", "Aston Martin Aramco Cognizant F1 Team"},
    {16, "Charles Leclerc", "Scuderia Ferrari"},
    {18, "Lance Stroll", "Aston Martin Aramco Cognizant F1 Team"},
    {20, "Kevin Magnussen", "Haas F1 Team"},
    {22, "Yuki Tsunoda", "Scuderia AlphaTauri"},
    {23, "Alexander Albon", "Williams Racing"},
    {24, "Zhou Guanyu", "Scuderia Ferrari"},
    {27, "Nico Hulkenberg", "Haas F1 Team"},
    {31, "Esteban Ocon", "BWT Alpine F1 Team"},
    {44, "Lewis Hamilton", "Mercedes-AMG Petronas Formula One Team"},
    {55, "Carlos Sainz", "Scuderia Ferrari"},
    {63, "George Russell", "Mercedes-AMG Petronas Formula One Team"},
    {77, "Valtteri Bottas", "Alfa Romeo F1 Team Stake"},
    {81, "Oscar Piastri", "McLaren F1 Team"}
};



const Driver DRIVER_LIST_CRASH[] = {
    {1, "Crash Bandicoot", "Red Bull Racing"},
    {2, "Coco Bandicoot", "Williams Racing"},
    {3, "Dr. Neo Cortex", "Scuderia AlphaTauri"},
    {4, "Dr. N. Gin", "McLaren F1 Team"},
    {10, "N. Tropy", "BWT Alpine F1 Team"},
    {11, "Tiny Tiger", "Red Bull Racing"},
    {14, "Dingodile", "Aston Martin Aramco Cognizant F1 Team"},
    {16, "Ripper Roo", "Scuderia Ferrari"},
    {18, "Papu Papu", "Aston Martin Aramco Cognizant F1 Team"},
    {20, "Pinstripe", "Haas F1 Team"},
    {22, "Polar", "Scuderia AlphaTauri"},
    {23, "Pura", "Williams Racing"},
    {24, "Fake Crash", "Scuderia Ferrari"},
    {27, "Nitros Oxide", "Haas F1 Team"},
    {31, "Nina Cortex", "BWT Alpine F1 Team"},
    {44, "Komodo Joe", "Mercedes-AMG Petronas Formula One Team"},
    {55, "Komodo Moe", "Scuderia Ferrari"},
    {63, "Geary", "Mercedes-AMG Petronas Formula One Team"},
    {77, "Krunk", "Alfa Romeo F1 Team Stake"},
    {81, "Velo the XXVII", "McLaren F1 Team"}
};


const Driver DRIVER_TROPEZ[] = {
    {1,  "Ludovic Cruchot",             "Red Bull Racing"},
    {2,  "Adjudant Gerber",             "Williams Racing"},
    {3,  "Fougasse",                    "Scuderia AlphaTauri"},
    {4,  "Tricard",                     "McLaren F1 Team"},
    {10, "Merlot",                      "BWT Alpine F1 Team"},
    {11, "Beaupied",                    "Red Bull Racing"},
    {14, "Berlicot",                    "Aston Martin Aramco Cognizant F1 Team"},
    {16, "Adjudante Gerber",            "Scuderia Ferrari"},
    {18, "Le voleur de poule",          "Aston Martin Aramco Cognizant F1 Team"},
    {20, "Soeur Clotilde",              "Haas F1 Team"},
    {22, "Nicole",                      "Scuderia AlphaTauri"},
    {23, "Le Colonel",                  "Williams Racing"},
    {24, "Josepha Cruchot",              "Scuderia Ferrari"},
    {27, "Poussin bleu",                 "Haas F1 Team"},
    {31, "Le Cure",                      "BWT Alpine F1 Team"},
    {44, "Le Sundeck",                   "Mercedes-AMG Petronas Formula One Team"},
    {55, "Le T80",      "Scuderia Ferrari"},
    {63, "Yo Macumba",                   "Mercedes-AMG Petronas Formula One Team"},
    {77, "Christine Rocourt",            "Alfa Romeo F1 Team Stake"},
    {81, "Cherubin",                     "McLaren F1 Team"}
};

