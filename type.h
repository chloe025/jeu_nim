#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED
#define VMIN  5
#define VMAX 30

//type structure qui permet de donner les coordonnées d'une case
struct Case
    {
    int lig;
    int col;
    };

//on renomme
typedef struct Case T_Case;
// on crée T_Tab_Case qui contiendra des types T_Case
typedef T_Case T_Tab_Case;


#endif // TYPE_H_INCLUDED
