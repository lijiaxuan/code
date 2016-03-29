#ifndef CAR_H

struct Car
{
    unsigned char   version;
    unsigned char   path[20];
    void            (*strjoint)(unsigned char * filepath);
    void            (*init)();
    void            (*save)();
};
extern struct Car car;

#define CAR_H
#endif
